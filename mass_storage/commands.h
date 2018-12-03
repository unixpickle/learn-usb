#ifndef __MS_COMMANDS_H__
#define __MS_COMMANDS_H__

#include <inttypes.h>
#include <libusb.h>

typedef struct __attribute__((packed)) {
  uint32_t signature;
  uint32_t tag;
  uint32_t xfer_length;
  uint8_t flags;
  uint8_t lun;
  uint8_t command_len;
  unsigned char command[16];
} cbw_packet_t;

typedef struct __attribute__((packed)) {
  uint32_t signature;
  uint32_t tag;
  uint32_t residue;
  uint8_t status;
} csw_packet_t;

void cbw_packet_convert_endian(cbw_packet_t* packet);
void csw_packet_convert_endian(csw_packet_t* packet);

#endif