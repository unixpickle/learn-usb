#include <inttypes.h>

typedef struct __attribute__((packed)) {
  uint8_t length;
  uint8_t desc_type;
} desc_header_t;

typedef struct __attribute__((packed)) {
  desc_header_t header;
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

typedef struct __attribute__((packed)) {
  desc_header_t header;
  uint16_t total_length;
  uint8_t num_interfaces;
  uint8_t configuration_value;
  uint8_t configuration_index;
  uint8_t attributes;
  uint8_t max_power;
} config_desc_t;

typedef struct __attribute__((packed)) {
  desc_header_t header;
  uint8_t interface_number;
  uint8_t alternate_setting;
  uint8_t num_endpoints;
  uint8_t interface_class;
  uint8_t interface_subclass;
  uint8_t interface_protocol;
  uint8_t interface_index;
} interface_desc_t;

typedef struct __attribute__((packed)) {
  desc_header_t header;
  uint8_t address;
  uint8_t attributes;
  uint16_t max_packet_size;
  uint8_t interval;
} endpoint_desc_t;