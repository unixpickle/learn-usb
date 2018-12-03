#ifndef __FIND_INTERFACES_H__
#define __FIND_INTERFACES_H__

#include <libusb.h>

// Find interfaces with the given numbers.
// Prompts the user for each one, and runs cb for each
// interface the user accepts.
void find_interfaces(
    int class_number,
    int subclass_number,
    int protocol,
    void (*cb)(libusb_device_handle* handle,
               const struct libusb_interface_descriptor* iface));

#endif