#include <libusb.h>
#include <stdio.h>
#include <string.h>
#include "find_interfaces.h"

void handle_keyboard(libusb_device_handle* handle,
                     const struct libusb_interface_descriptor* iface);
int setup_keyboard(libusb_device_handle* handle);
void keyboard_loop(libusb_device_handle* handle,
                   const struct libusb_interface_descriptor* iface);

int main() {
  libusb_init(NULL);
  find_interfaces(LIBUSB_CLASS_HID, 1, 1, handle_keyboard);
  return 0;
}

void handle_keyboard(libusb_device_handle* handle,
                     const struct libusb_interface_descriptor* iface) {
  if (iface->bNumEndpoints != 1) {
    fprintf(stderr, "[ERROR] unexpected number of endpoints.\n");
    return;
  }
  if (setup_keyboard(handle)) {
    keyboard_loop(handle, iface);
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

void keyboard_loop(libusb_device_handle* handle,
                   const struct libusb_interface_descriptor* iface) {
  unsigned char key_mask[255];
  bzero(key_mask, sizeof(key_mask));
  printf("dumping keys (press QWERTY Ctrl+C to quit)...\n");
  int ctrl_c = 0;
  while (!ctrl_c) {
    unsigned char data[8];
    int actual_length;
    int res =
        libusb_interrupt_transfer(handle, iface->endpoint[0].bEndpointAddress,
                                  data, sizeof(data), &actual_length, 0);
    if (res) {
      fprintf(stderr, "[ERROR] %s\n", libusb_error_name(res));
      return;
    }

    // Presses
    for (int i = 2; i < 8; ++i) {
      if (data[i] != 0) {
        if (!key_mask[data[i] - 1]) {
          key_mask[data[i] - 1] = 1;
          printf("modifiers=%d key=%d press=1\n", data[0], data[i]);
          if (data[0] == 1 && data[i] == 6) {
            ctrl_c = 1;
          }
        }
      }
    }

    // Releases
    for (int i = 0; i < sizeof(key_mask); ++i) {
      if (key_mask[i]) {
        int found = 0;
        for (int j = 2; j < 8; ++j) {
          if (data[j] - 1 == i) {
            found = 1;
            break;
          }
        }
        if (!found) {
          printf("modifiers=%d key=%d press=0\n", data[0], i + 1);
          key_mask[i] = 0;
        }
      }
    }
  }
}