#ifndef __MS_SCSI_H__
#define __MS_SCSI_H__

#include <inttypes.h>
#include "commands.h"

// All APIs return negative, non-zero values on error.

int scsi_test_unit_ready(ms_dev_t* dev);
int scsi_read_capacity(ms_dev_t* dev,
                       uint64_t* last_block,
                       uint64_t* block_size);
int scsi_read_capacity_10(ms_dev_t* dev,
                          uint32_t* last_block,
                          uint32_t* block_size);
int scsi_read_capacity_16(ms_dev_t* dev,
                          uint64_t* last_block,
                          uint64_t* block_size);
int scsi_read_10(ms_dev_t* dev,
                 uint32_t logical_block,
                 uint16_t num_blocks,
                 uint32_t output_size,
                 char* output);

#endif