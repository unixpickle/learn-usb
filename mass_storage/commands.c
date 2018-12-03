#include "commands.h"

#define le32(x)                                                           \
  (libusb_cpu_to_le16(1) == 1 ? x                                         \
                              : (libusb_cpu_to_le16((uint16_t)x) << 16) | \
                                    libusb_cpu_to_le16((uint16_t)(x >> 16)))

void cbw_packet_convert_endian(cbw_packet_t* packet) {
  packet->signature = le32(packet->signature);
  packet->tag = le32(packet->tag);
  packet->transfer_len = le32(packet->transfer_len);
}

void csw_packet_convert_endian(csw_packet_t* packet) {
  packet->signature = le32(packet->signature);
  packet->tag = le32(packet->tag);
  packet->residue = le32(packet->residue);
}

int ms_dev_in_endpoint(ms_dev_t* dev) {
  for (int i = 0; i < (int)dev->iface->bNumEndpoints; ++i) {
    struct libusb_endpoint_descriptor* desc = &dev->iface->endpoint[i];
    if (desc->bEndpointAddress & 0x80) {
      return desc->bEndpointAddress;
    }
  }
  return -1;
}

int ms_dev_out_endpoint(ms_dev_t* dev) {
  for (int i = 0; i < (int)dev->iface->bNumEndpoints; ++i) {
    struct libusb_endpoint_descriptor* desc = &dev->iface->endpoint[i];
    if (!(desc->bEndpointAddress & 0x80)) {
      return desc->bEndpointAddress;
    }
  }
  return -1;
}

int ms_dev_out(ms_dev_t* dev,
               uint32_t data_len,
               uint8_t command_len,
               const char* command,
               const char* data) {
  cbw_packet_t cbw;
  cbw.signature = 0x43425355;
  cbw.tag = 1337;
  cbw.transfer_len = data_len;
  cbw.flags = 0;
  cbw.lun = dev->lun;
  cbw.command_len = command_len;
  assert(command_len <= sizeof(cbw.command));
  memcpy(cbw.command, command, command_len);
  cbw_packet_convert_endian(&cbw);

  int out_endpoint = ms_dev_out_endpoint(dev);
  int in_endpoint = ms_dev_in_endpoint(dev);
  if (out_endpoint < 0 || in_endpoint < 0) {
    return -LIBUSB_ERROR_PIPE;
  }
  int actual_length = 0;
  int res = libusb_bulk_transfer(dev->handle, out_endpoint, &cbw, sizeof(cbw),
                                 &actual_length, 0);
  if (res) {
    return -res;
  }

  int res = libusb_bulk_transfer(dev->handle, out_endpoint, data, (int)data_len,
                                 &actual_length, 0);
  if (res) {
    return -res;
  }

  csw_packet_t csw;
  int res = libusb_bulk_transfer(dev->handle, in_endpoint, &csw, sizeof(csw),
                                 &actual_length, 0);
  if (res) {
    return -res;
  }
  csw_packet_convert_endian(&csw);

  if (csw.tag != 1337) {
    return -LIBUSB_ERROR_OTHER;
  }
  if (csw.status) {
    return -LIBUSB_ERROR_IO;
  }
  return data_len - csw.residue;
}

int ms_dev_in(ms_dev_t* dev,
              uint32_t data_len,
              uint8_t command_len,
              const char* command,
              const char* data) {
  cbw_packet_t cbw;
  cbw.signature = 0x43425355;
  cbw.tag = 1338;
  cbw.transfer_len = data_len;
  cbw.flags = 0x80;
  cbw.lun = dev->lun;
  cbw.command_len = command_len;
  assert(command_len <= sizeof(cbw.command));
  memcpy(cbw.command, command, command_len);
  cbw_packet_convert_endian(&cbw);

  int out_endpoint = ms_dev_out_endpoint(dev);
  int in_endpoint = ms_dev_in_endpoint(dev);
  if (out_endpoint < 0 || in_endpoint < 0) {
    return -LIBUSB_ERROR_PIPE;
  }
  int actual_length = 0;
  int res = libusb_bulk_transfer(dev->handle, out_endpoint, &cbw, sizeof(cbw),
                                 &actual_length, 0);
  if (res) {
    return -res;
  }

  int res = libusb_bulk_transfer(dev->handle, in_endpoint, data, (int)data_len,
                                 &actual_length, 0);
  if (res) {
    return -res;
  }

  csw_packet_t csw;
  int res = libusb_bulk_transfer(dev->handle, in_endpoint, &csw, sizeof(csw),
                                 &actual_length, 0);
  if (res) {
    return -res;
  }
  csw_packet_convert_endian(&csw);

  if (csw.tag != 1338) {
    return -LIBUSB_ERROR_OTHER;
  }
  if (csw.status) {
    return -LIBUSB_ERROR_IO;
  }
  return data_len - csw.residue;
}