// SPDX-License-Identifier: BSD-3-Clause (rhulme/pico-flashloader)

#include "process.h"

// lib/pico-flashloader
#include "flashloader.h"

#include "record.h"

// breakpoints
#include "pico/stdlib.h"

// memset, memcpy
#include "pico/mem_ops.h"

flashbuf_t flashbuf;

uint32_t offset = 0;
uint32_t count = 0;

// is passed a pointer to rec
void processRecord(ihexRecord *rec)
{
    //__breakpoint();
    switch (rec->type)
    {
    case UHEX_TYPE_CUSTOM_DATA:
        // Check if the active sectionId is the app section id.
        if (rec->sectionId == SECTION_ID_APP)
        {
            // rhulme's implementation
            // seems sketchy, seems to copy into the buffer underneath it?
            // On the other hand, I have very little C/C++ experience
            memcpy(&flashbuf.header.data[offset], rec->data, rec->count);
            offset += rec->count;
            offset %= FLASH_BUF_SIZE;
            // if ((offset % 1024) == 0)
            //{
            //     printf("Recieved a 1k block of firmware via DFU\n");
            // }
        }
        // Section ID does not match, we can ignore this.
        // eg. flashloader code at the start of the file.
        break;
    case IHEX_TYPE_EOF:
        printf("FOUND EOF\n");
        // Flashing is done in tud_dfu_manifest_cb.
        break;
    // skip
    case IHEX_TYPE_EXT_LIN_ADDR:
        printf("EXT LINE\n");
        printf("NEW ADDRESS: 0x%08x\n", rec->ulba);
        break;
    }
}