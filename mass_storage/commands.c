#include "commands.h"

#define le32(x)                                                           \
  (libusb_cpu_to_le16(1) == 1 ? x                                         \
                              : (libusb_cpu_to_le16((uint16_t)x) << 16) | \
                                    libusb_cpu_to_le16((uint16_t)(x >> 16)))

void cbw_packet_convert_endian(cbw_packet_t* packet) {
  packet->signature = le32(packet->signature);
  packet->tag = le32(packet->tag);
  packet->xfer_length = le32(packet->xfer_length);
}

void csw_packet_convert_endian(csw_packet_t* packet) {
  packet->signature = le32(packet->signature);
  packet->tag = le32(packet->tag);
  packet->residue = le32(packet->residue);
}