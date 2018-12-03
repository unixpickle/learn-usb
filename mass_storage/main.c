#include <libusb.h>
#include <stdio.h>
#include "commands.h"
#include "find_interfaces.h"
#include "scsi.h"

#define MASS_STORAGE_SUBCLASS_SCSI 6
#define MASS_STORAGE_PROTOCOL_BULK 80

void handle_device(libusb_device_handle* handle,
                   const struct libusb_interface_descriptor* iface);

int main() {
  libusb_init(NULL);
  find_interfaces(LIBUSB_CLASS_MASS_STORAGE, MASS_STORAGE_SUBCLASS_SCSI, 80,
                  handle_device);
  return 0;
}

void handle_device(libusb_device_handle* handle,
                   const struct libusb_interface_descriptor* iface) {
  ms_dev_t dev = {
      .handle = handle,
      .iface = iface,
      .lun = 0,
  };

  int res = scsi_test_unit_ready(&dev);
  if (res < 0) {
    fprintf(stderr, "[ERROR] failed to issue TEST UNIT READY: %s\n",
            libusb_error_name(res));
    return;
  }
  printf("TEST UNIT READY succeeded.\n");

  uint64_t last_block;
  uint64_t block_size;
  res = scsi_read_capacity(&dev, &last_block, &block_size);
  if (res) {
    fprintf(stderr, "[ERROR] reading capacity: %s\n", libusb_error_name(res));
    return;
  }
  printf("capacity=%llu\n",
         (unsigned long long)((last_block + 1) * block_size));
  printf("block_size=%d\n", (int)block_size);
}
