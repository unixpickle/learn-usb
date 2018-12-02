#include <libusb.h>
#include <stdio.h>
#include <string.h>

void find_keyboards();
void handle_keyboard(libusb_device* dev,
                     const struct libusb_interface_descriptor* iface);
int should_use(libusb_device_handle* handle);
int setup_keyboard(libusb_device_handle* handle);

int main() {
  libusb_init(NULL);
  find_keyboards();
  return 0;
}

void find_keyboards() {
  libusb_device** list;
  ssize_t num_devices = libusb_get_device_list(NULL, &list);
  for (ssize_t i = 0; i < num_devices; ++i) {
    libusb_device* dev = list[i];
    struct libusb_config_descriptor* desc;
    if (libusb_get_config_descriptor(dev, 0, &desc)) {
      fprintf(stderr, "error getting descriptor.\n");
      continue;
    }
    for (int i = 0; i < (int)desc->bNumInterfaces; ++i) {
      const struct libusb_interface_descriptor* iface =
          &(&desc->interface[i])->altsetting[0];
      if (iface->bInterfaceClass == LIBUSB_CLASS_HID &&
          iface->bInterfaceSubClass == 1 && iface->bInterfaceProtocol == 1) {
        printf("Found device (bus=%d addr=%d)\n", libusb_get_bus_number(dev),
               libusb_get_device_address(dev));
        handle_keyboard(dev, iface);
      }
    }
    libusb_free_config_descriptor(desc);
  }

  libusb_free_device_list(list, 1);
}

void handle_keyboard(libusb_device* dev,
                     const struct libusb_interface_descriptor* iface) {
  if (iface->bNumEndpoints != 1) {
    fprintf(stderr, "[ERROR] unexpected number of endpoints.\n");
    return;
  }

  libusb_device_handle* handle;
  if (libusb_open(dev, &handle)) {
    fprintf(stderr, "[ERROR] opening device.\n");
    return;
  }

  if (should_use(handle)) {
    libusb_detach_kernel_driver(handle, iface->bInterfaceNumber);
    int res = libusb_claim_interface(handle, iface->bInterfaceNumber);
    if (!res) {
      if (setup_keyboard(handle)) {
        char data[8];
        int actual_length;
        while (1) {
          int res = libusb_interrupt_transfer(
              handle, iface->endpoint[0].bEndpointAddress, data, sizeof(data),
              &actual_length, 0);
          if (res) {
            fprintf(stderr, "[ERROR] %s\n", libusb_error_name(res));
            break;
          }
          printf("read %d bytes.\n", actual_length);
        }
      }
      libusb_release_interface(handle, iface->bInterfaceNumber);
    } else {
      fprintf(stderr, "[ERROR] claiming interface: %s\n",
              libusb_error_name(res));
    }
    libusb_attach_kernel_driver(handle, iface->bInterfaceNumber);
  } else {
    // Useful for fixing messed up drivers.
    libusb_attach_kernel_driver(handle, iface->bInterfaceNumber);
  }

  libusb_close(handle);
}

int should_use(libusb_device_handle* handle) {
  struct libusb_device_descriptor desc;
  if (libusb_get_device_descriptor(libusb_get_device(handle), &desc)) {
    fprintf(stderr, "[ERROR] getting device descriptor.\n");
    return 0;
  }

  unsigned char name[512];
  unsigned char maker[512];
  name[0] = 0;
  maker[0] = 0;
  libusb_get_string_descriptor_ascii(handle, desc.iProduct, name, sizeof(name));
  libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, maker,
                                     sizeof(maker));
  printf("Use name=%s manufacturer=%s? [y/n]: ", name, maker);
  fflush(stdout);

  while (1) {
    char input[512];
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "y\n") == 0) {
      return 1;
    } else if (strcmp(input, "n\n") == 0) {
      return 0;
    } else {
      printf("please type 'y' or 'n': ");
      fflush(stdout);
    }
  }
}

int setup_keyboard(libusb_device_handle* handle) {
  if (libusb_reset_device(handle)) {
    fprintf(stderr, "[ERROR] resetting device.\n");
    return 0;
  }

  // Run a Set_Protocol request.
  int res = libusb_control_transfer(handle, 0b00100001, 0x0b, 0, 0, NULL, 0, 0);
  if (res) {
    fprintf(stderr, "[ERROR] setting protocol.\n");
    return 0;
  }

  return 1;
}
