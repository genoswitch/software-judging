#include "tusb.h"

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

// Called when we recieve a new block from the host.
// (recieved DFU_DNLOAD (wLength>0) following by DFU_GETSTATUS (state=DFU_DNBUSY) requests)
// This callback could be returned before flashing op is complete (async).
// Once finished flashing, application must call tud_dfu_finish_flashing()
void tud_dfu_download_cb(uint8_t alt, uint16_t block_num, uint8_t const *data, uint16_t length)
{
    (void)alt;
    (void)block_num;

    // printf("\r\nReceived Alt %u BlockNum %u of length %u\r\n", alt, wBlockNum, length);

    for (uint16_t i = 0; i < length; i++)
    {
        printf("%c", data[i]);
    }

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
