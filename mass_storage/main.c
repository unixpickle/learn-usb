#include <libusb.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "commands.h"
#include "find_interfaces.h"
#include "scsi.h"

#define MASS_STORAGE_SUBCLASS_SCSI 6
#define MASS_STORAGE_PROTOCOL_BULK 80

void handle_device(libusb_device_handle* handle,
                   const struct libusb_interface_descriptor* iface);
void measure_entropy(ms_dev_t* dev);

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

  measure_entropy(&dev);
}

void measure_entropy(ms_dev_t* dev) {
  uint64_t last_block;
  uint64_t block_size;
  int res = scsi_read_capacity(dev, &last_block, &block_size);
  if (res) {
    fprintf(stderr, "[ERROR] reading capacity: %s\n", libusb_error_name(res));
    return;
  }
  printf("num_blocks=%llu\n", (unsigned long long)(last_block + 1));
  printf("block_size=%d\n", (int)block_size);

  uint64_t batch_size = 4;
  if ((last_block + 1) % batch_size) {
    batch_size = 1;
  }

  double counts[256];
  bzero(counts, sizeof(counts));
  double total = 0.0;
  unsigned char* block = malloc(block_size * batch_size);

  for (uint64_t i = 0; i <= last_block && i < (1 << 31); i += batch_size) {
    if (i % (10000 * batch_size) == 0) {
      printf("reading block %d...\n", (int)i);
    }
    res = scsi_read_10(dev, (uint32_t)i, (uint16_t)batch_size,
                       (uint32_t)(block_size * batch_size), block);
    if (res < 0) {
      fprintf(stderr, "[ERROR] reading block: %s\n", libusb_error_name(res));
      break;
    }
    for (int j = 0; j < (int)(block_size * batch_size); ++j) {
      counts[block[j]] += 1.0;
      total += 1.0;
    }
  }
  free(block);

  double entropy = 0.0;
  for (int i = 0; i < 256; ++i) {
    if (counts[i] > 0) {
      double freq = counts[i] / total;
      entropy += freq * log2(freq);
    }
  }
  printf("char_entropy=%lf\n", entropy);
}
