#include <libusb.h>
#include <stdio.h>
#include <string.h>
#include "usb_constants.h"
#include "usb_types.h"

void print_descriptors(libusb_device_handle* handle);
const char* string_descriptor(libusb_device_handle* handle, uint8_t idx);
void print_sub_descriptor(libusb_device_handle* handle, desc_header_t* header);

int main() {
  printf("initializing libusb...\n");
  libusb_init(NULL);

  printf("listing devices...\n");
  libusb_device** list;
  ssize_t num_devices = libusb_get_device_list(NULL, &list);

  printf("analyzing devices...\n");
  for (ssize_t i = 0; i < num_devices; ++i) {
    libusb_device* dev = list[i];
    libusb_device_handle* handle;
    if (libusb_open(dev, &handle)) {
      fprintf(stderr, "error opening device.\n");
      continue;
    }
    print_descriptors(handle);
    libusb_close(handle);
  }

  libusb_free_device_list(list, 1);
  return 0;
}

void print_descriptors(libusb_device_handle* handle) {
  device_desc_t device_desc;
  int res = libusb_control_transfer(
      handle,
      REQUEST_TYPE_DEV_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_TYPE_DEVICE,
      REQUEST_GET_DESCRIPTOR, (DESC_TYPE_DEVICE << 8), 0,
      (unsigned char*)&device_desc, sizeof(device_desc), 0);
  if (res < 0) {
    fprintf(stderr, "error getting device description: %s (%d)\n",
            libusb_error_name(res), res);
    return;
  }
  printf("Device: %s\n", string_descriptor(handle, device_desc.product_index));
  printf(" - manufacturer: %s\n",
         string_descriptor(handle, device_desc.manufacturer_index));
  printf(" - serial: %s\n",
         string_descriptor(handle, device_desc.serial_index));
  printf(" - class: %d\n", (int)device_desc.device_class);
  printf(" - subclass: %d\n", (int)device_desc.device_subclass);

  for (int i = 0; i < (int)device_desc.num_configurations; ++i) {
    unsigned char config_data[1024];
    res = libusb_control_transfer(
        handle,
        REQUEST_TYPE_DEV_TO_HOST | REQUEST_TYPE_STANDARD | REQUEST_TYPE_DEVICE,
        REQUEST_GET_DESCRIPTOR, (DESC_TYPE_CONFIG << 8) | (uint16_t)i, 0,
        config_data, sizeof(config_data), 0);
    if (res < 0) {
      fprintf(stderr, "error getting configuration description: %s (%d)\n",
              libusb_error_name(res), res);
      continue;
    }
    int offset = 0;
    while (offset + 2 < res) {
      desc_header_t* header = (desc_header_t*)&config_data[offset];
      if (header->length + offset > res) {
        fprintf(stderr, "buffer underflow\n");
        break;
      }
      print_sub_descriptor(handle, header);
      offset += header->length;
    }
  }
}

const char* string_descriptor(libusb_device_handle* handle, uint8_t idx) {
  static char result[512];
  bzero(result, sizeof(result));
  libusb_get_string_descriptor_ascii(handle, idx, (unsigned char*)result,
                                     sizeof(result));
  return result;
}

void print_sub_descriptor(libusb_device_handle* handle, desc_header_t* header) {
  if (header->desc_type == DESC_TYPE_CONFIG) {
    if (header->length < sizeof(config_desc_t)) {
      return;
    }
    config_desc_t* desc = (config_desc_t*)header;
    printf("   | Configuration: %s\n",
           string_descriptor(handle, desc->configuration_index));
    printf("     - max power: %d\n", desc->max_power);
    printf("     - attributes: %d\n", desc->attributes);
  } else if (header->desc_type == DESC_TYPE_INTERFACE) {
    if (header->length < sizeof(interface_desc_t)) {
      return;
    }
    interface_desc_t* desc = (interface_desc_t*)header;
    printf("     | Interface: %d\n", desc->interface_number);
    printf("       - name: %s\n",
           string_descriptor(handle, desc->interface_index));
    printf("       - alternate: %d\n", desc->alternate_setting);
    printf("       - class: %d\n", desc->interface_class);
    printf("       - subclass: %d\n", desc->interface_subclass);
  } else if (header->desc_type == DESC_TYPE_ENDPOINT) {
    if (header->length < sizeof(endpoint_desc_t)) {
      return;
    }
    endpoint_desc_t* desc = (endpoint_desc_t*)header;
    printf("       | Endpoint: %d\n", desc->address);
    printf("         - max packet size: %d\n", desc->max_packet_size);
    printf("         - interval: %d\n", desc->interval);
    printf("         - attributes: %d\n", desc->attributes);
  } else {
    printf("       | (Unknown descriptor type: %d)\n", header->desc_type);
  }
}
