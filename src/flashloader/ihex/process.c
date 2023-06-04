// lib/pico-flashloader
#include "flashloader.h"

#include "record.h"

// breakpoints
#include "pico/stdlib.h"

// memset, memcpy
#include "pico/mem_ops.h"

// Offset within flash of the new app image to be flashed by the flashloader
static const uint32_t FLASH_IMAGE_OFFSET = 128 * 1024;

// Buffer to hold the incoming data before flashing
static union
{
    tFlashHeader header;
    uint8_t buffer[sizeof(tFlashHeader) + 65536];
} flashbuf;

uint32_t offset = 0;
uint32_t count = 0;

// is passed a pointer to rec
void processRecord(ihexRecord *rec)
{
    //__breakpoint();
    switch (rec->type)
    {
    case IHEX_TYPE_DATA:
        printf("ADDR: 0x%08x\n", getAddress(rec));
        // seems sketchy, seems to copy into the buffer underneath it?
        memcpy(&flashbuf.header.data[offset], rec->data, rec->count);
        offset += rec->count;
        offset %= 65536;
        if ((offset % 1024) == 0)
        {
            printf("RHULME RECIEVED BLOCK\n");
        }
        break;
    case IHEX_TYPE_EOF:
        printf("FOUND EOF\n");
        break;
    // skip
    case IHEX_TYPE_EXT_LIN_ADDR:
        printf("EXT LINE\n");
        printf("NEW ADDRESS: 0x%08x\n", rec->ulba);
        break;
    }
}