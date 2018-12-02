#include <libusb.h>
#include <stdio.h>
#include <string.h>
#include "usb_constants.h"
#include "usb_types.h"

void print_descriptors(libusb_device_handle* handle);
const char* string_descriptor(libusb_device_handle* handle, uint8_t idx);

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
      REQUEST_GET_DESCRIPTOR, DESC_TYPE_DEVICE, 0, (unsigned char*)&device_desc,
      sizeof(device_desc), 0);
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
}

const char* string_descriptor(libusb_device_handle* handle, uint8_t idx) {
  static char result[512];
  bzero(result, sizeof(result));
  libusb_get_string_descriptor_ascii(handle, idx, (unsigned char*)result,
                                     sizeof(result));
  return result;
}