#ifndef __ms_devS_H__
#define __ms_devS_H__

#include <inttypes.h>
#include <libusb.h>

typedef struct __attribute__((packed)) {
  uint32_t signature;
  uint32_t tag;
  uint32_t transfer_len;
  uint8_t flags;
  uint8_t lun;
  uint8_t command_len;
  char command[16];
} cbw_packet_t;

typedef struct __attribute__((packed)) {
  uint32_t signature;
  uint32_t tag;
  uint32_t residue;
  uint8_t status;
} csw_packet_t;

void cbw_packet_convert_endian(cbw_packet_t* packet);
void csw_packet_convert_endian(csw_packet_t* packet);

typedef struct {
  libusb_device_handle* handle;
  struct libusb_interface_descriptor* iface;
  uint32_t lun;
} ms_dev_t;

int ms_dev_in_endpoint(ms_dev_t* dev);
int ms_dev_out_endpoint(ms_dev_t* dev);

// Transfer data to the device.
// Returns a negative value on failure.
// Otherwise, returns the number of bytes sent.
int ms_dev_out(ms_dev_t* dev,
               uint32_t data_len,
               uint8_t command_len,
               const char* command,
               const char* data);

// Transfer data from the device.
// Returns a negative value on failure.
// Otherwise, returns the number of bytes read.
int ms_dev_in(ms_dev_t* dev,
              uint32_t data_len,
              uint8_t command_len,
              const char* command,
              const char* data);

#endif