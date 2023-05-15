#include "tusb.h"

#include "ihex/record.h"

#include "ringbuf.h"

#include <string.h>

// DFU Callbacks
// Note that 'alt' is used as the partition number to allow for multiple partitions (FLASH/EEPROM/etc)
// At the moment we only have one "partition".
// Invoked right before tud_dfu_download_cb() (state=DFU_DNBUSY) or tud_dfu_manifest_cb() (state=DFU_MANIFEST)
// Application return timeout in milliseconds (bwPollTimeout) for the next download/manifest operation.
// During this period, USB host won't try to communicate with us.

uint32_t tud_dfu_get_timeout_cb(uint8_t alt, uint8_t state)
{
    if (state == DFU_DNBUSY)
    {
        // For this example
        // - Atl0 Flash is fast : 1   ms
        // - Alt1 EEPROM is slow: 100 ms
        return (alt == 0) ? 1 : 100;
    }
    else if (state == DFU_MANIFEST)
    {
        // since we don't buffer entire image and do any flashing in manifest stage
        return 0;
    }

    return 0;
}

ringbuf_t rb1;

// Called when we recieve a new block from the host.
// (recieved DFU_DNLOAD (wLength>0) following by DFU_GETSTATUS (state=DFU_DNBUSY) requests)
// This callback could be returned before flashing op is complete (async).
// Once finished flashing, application must call tud_dfu_finish_flashing()
void tud_dfu_download_cb(uint8_t alt, uint16_t block_num, uint8_t const *data, uint16_t length)
{
    (void)alt;
    (void)block_num;

    if (block_num == 0)
    {
        printf("Initialized new ringbuffer...\n");
        rb1 = ringbuf_new(256 + 5);
    }

    ringbuf_memcpy_into(rb1, data, length);

    int match;
    // 'thing' works, "thing" does not (returns size)
    match = ringbuf_findchr(rb1, '\n', 0);
    if (match != ringbuf_bytes_used(rb1))
    {
        printf("Found a match at index %i\n", match);

        // Get a buffer of the right size (calloc to zero fill)
        char *buffer = malloc(match + 1);

        // Copy from tail up to and including the match index
        // This increments the tail i think.
        ringbuf_memcpy_from(buffer, rb1, match + 1);

        ihexRecord rec;
        parseRecord(buffer, &rec);
        printf("processed record with length %i\n", rec.count);

        // re-run
        // 'thing' works, "thing" does not (returns size)
        match = ringbuf_findchr(rb1, '\n', 0);
    }

    printf("\nProcessed DFU packet: alt %u, blockNum %u, length %u\n", alt, block_num, length);

    // flashing op for download complete without error
    tud_dfu_finish_flashing(DFU_STATUS_OK);
}

// Called when the download process is complete (new firmware has been sent to the device)
// (received DFU_DNLOAD (wLength=0) following by DFU_GETSTATUS (state=Manifest))
// Here we should do checksumming, or flashing if we previously buffered the image.
// Once we have finished flashing, we must call tud_dfu_finish_flashing()
void tud_dfu_manifest_cb(uint8_t alt)
{
    (void)alt; // Ignore paramater
    printf("Download finished!\n");

    // flashing op for manifest is complete without error
    // Application can perform checksum, should it fail, use appropriate status such as errVERIFY.
    tud_dfu_finish_flashing(DFU_STATUS_OK);
}
