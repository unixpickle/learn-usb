#include "scsi.h"
#include <strings.h>

static uint32_t fix_endian_32(uint32_t number);
static uint64_t fix_endian_64(uint64_t number);

int scsi_test_unit_ready(ms_dev_t* dev) {
  char request[6];
  bzero(request, sizeof(request));
  return ms_dev_in(dev, 0, 6, request, NULL);
}

int scsi_read_capacity(ms_dev_t* dev,
                       uint64_t* last_block,
                       uint64_t* block_size) {
  uint32_t lb_small;
  uint32_t bs_small;
  int res = scsi_read_capacity_10(dev, &lb_small, &bs_small);
  if (res < 0) {
    return res;
  }
  if (lb_small == 0xffffffff) {
    return scsi_read_capacity_16(dev, last_block, block_size);
  }
  *last_block = (uint64_t)lb_small;
  *block_size = (uint64_t)bs_small;
  return 0;
}

int scsi_read_capacity_10(ms_dev_t* dev,
                          uint32_t* last_block,
                          uint32_t* block_size) {
  char request[10];
  bzero(request, sizeof(request));
  request[0] = 0x25;

  char response[8];
  bzero(response, sizeof(response));
  int res = ms_dev_in(dev, 8, 10, request, response);
  if (res < 0) {
    return res;
  }
  *last_block = fix_endian_32(*(uint32_t*)response);
  *block_size = fix_endian_32(((uint32_t*)response)[1]);
  return 0;
}

int scsi_read_capacity_16(ms_dev_t* dev,
                          uint64_t* last_block,
                          uint64_t* block_size) {
  char request[16];
  bzero(request, sizeof(request));
  request[0] = 0x9e;
  request[1] = 0x10;
  request[13] = 32;

  char response[32];
  bzero(response, sizeof(response));
  int res = ms_dev_in(dev, 32, 16, request, response);
  if (res < 0) {
    return res;
  }
  *last_block = fix_endian_64(*(uint64_t*)response);
  *block_size = fix_endian_64(((uint64_t*)response)[1]);
  return 0;
}

static uint32_t fix_endian_32(uint32_t number) {
  uint32_t x = 1;
  if (!*(char*)&x) {
    return number;
  }
  return (number >> 24) | ((number & 0x00ff0000) >> 8) |
         ((number & 0x0000ff00) << 8) | ((number & 0xff) << 24);
}

static uint64_t fix_endian_64(uint64_t number) {
  uint64_t x = 1;
  if (!*(char*)&x) {
    return number;
  }
  x = number;
  char* num_ptr = (char*)&x;
  for (int i = 0; i < 4; i++) {
    char old = num_ptr[i];
    num_ptr[i] = num_ptr[7 - i];
    num_ptr[7 - i] = old;
  }
  return x;
}