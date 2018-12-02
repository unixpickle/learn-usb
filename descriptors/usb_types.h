#include <inttypes.h>

typedef struct __attribute__((packed)) {
  uint8_t ength;
  uint8_t desc_type;
  uint16_t usbSpec;
  uint8_t device_class;
  uint8_t device_subclass;
  uint8_t device_protocol;
  uint8_t max_packet_size;
  uint16_t vendor_id;
  uint16_t product_id;
  uint16_t device;
  uint8_t manufacturer_index;
  uint8_t product_index;
  uint8_t serial_index;
  uint8_t num_configurations;
} device_desc_t;