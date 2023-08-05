// SPDX-License-Identifier: BSD-3-Clause (rhulme/pico-flashloader)

#ifndef IHEX_PROCESS_H_
#define IHEX_PROCESS_H_

// uint32_t
#include <stdint.h>

// lib/pico-flashloader
#include "flashloader.h"

// ihexRecord
#include "record.h"

// Offset within flash of the new app image to be flashed by the flashloader
static const uint32_t FLASH_IMAGE_OFFSET = 128 * 1024;

void processRecord(ihexRecord *record);

// Current image has around ~67k of binary data. (07.06.2023)
// To futureproof, let's give 96k to store the buffered data.
#define FLASH_BUF_SIZE 65536 + 32768

// (typedef): Buffer to hold the incoming data before flashing
typedef union
{
    tFlashHeader header;
    // If this buffer is overfilled, the USB transfer will fail.
    // (Is easily detectable by the host/sending device)
    // "tud_dfu_finish_flashing(DFU_STATUS_OK);" (dfu.c) will never be called,
    // So the transfer will appear to hang until it times out.
    uint8_t buffer[sizeof(tFlashHeader) + FLASH_BUF_SIZE];
} flashbuf_t;

extern flashbuf_t flashbuf;
extern uint32_t offset;

#endif /* IHEX_PROCESS_H_ */