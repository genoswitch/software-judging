#include "hardware/dma.h"

// CRC32 calculation from a pointer using RP2040 DMA.
// (Polynomial 0x4C11DB7, input + output reflection, final XOR)
// Extensively commented with sources and references listed where appropriate.
static uint32_t crc32(void *ptr, uint32_t len)
{
    // Initial write address
    uint32_t dest;

    // Result
    uint32_t crc;

    // Claim a free DMA channel, panic() if one is not available
    int channel = dma_claim_unused_channel(true);

    // Configure the DMA channel
    dma_channel_config config = dma_channel_get_default_config(channel);

    // (conf): Set transfer size
    // pico-examples hello_dma uses DMA_SIZE_8 (8 bits, one byte)
    // [PERMALINK] https://github.com/raspberrypi/pico-examples/blob/1c5d9aa567598e6e3eadf6d7f2d8a9342b44dab4/dma/hello_dma/hello_dma.c#L28
    // However, it appears that using DMA_SIZE_32 (32 bits, 4 bytes) is faster.
    // https://forums.raspberrypi.com/viewtopic.php?t=319315
    // This is also the setting used in usedbytes' RP2040 serial bootloader (similar to picowota)
    // [PERMALINK] https://github.com/usedbytes/rp2040-serial-bootloader/blob/f6f0d61fc03447a1401d3f9d158f35331bf0eb98/main.c#L320
    channel_config_set_transfer_data_size(&config, DMA_SIZE_32);

    // (conf): Enable read incrementing
    // This means that the DMA channel will increment from one address to the
    // next between subsequent transfers.
    // https://vanhunteradams.com/Pico/DAC/DMA_DAC.html
    // [ARCHIVED] <https://web.archive.org/web/20230304013953/https://vanhunteradams.com/Pico/DAC/DMA_DAC.html>
    channel_config_set_read_increment(&config, true);

    // (conf): Disable write incrementing
    // pico-examples hello-dma enables this, but we do not need it at the moment.
    // We are also using a different method to store the resultant value.
    // This means that the DMA channel will always write to the same memory address.
    // (see above sources)
    channel_config_set_write_increment(&config, false);

    // (conf): Allow/enable access to the DMA channel by the sniffer
    // We are using the sniffer to calculate the crc32 hash (?)
    channel_config_set_sniff_enable(&config, true);

    // (conf): Seed the CRC calculation
    // For CRC32, you need to init dma_hw->sniff_data to 0xFFFFFFFF (and XOR with it afterwards)
    // Usedbytes encountered this issue and opened the below issue to solve it.
    // https://github.com/raspberrypi/pico-sdk/issues/576#issuecomment-929587218
    dma_hw->sniff_data = 0xFFFFFFFF;

    // (conf): Setup and enable the sniffer.
    // Using Mode 1, values that are bit-reversed versions of golang's IEEE802.3 implementation
    // Usedbytes' issue where he documented this finding:
    // https://github.com/raspberrypi/pico-sdk/issues/576#issuecomment-929608656
    // Usedbytes' code where other details are commented
    // [PERMALINK] https://github.com/usedbytes/rp2040-serial-bootloader/blob/f6f0d61fc03447a1401d3f9d158f35331bf0eb98/main.c#L328
    dma_sniffer_enable(channel, 0x1, true);
    dma_hw->sniff_ctrl |= DMA_SNIFF_CTRL_OUT_REV_BITS;

    // (conf): Configure the DMA channel itself.
    // Comments sourced from https://github.com/raspberrypi/pico-examples/blob/master/dma/hello_dma/hello_dma.c#L32
    dma_channel_configure(
        channel, // Channel to be configured
        &config, // Channel configuration
        &dest,   // Initial write address
        ptr,     // Initial read address (pointer in this case passed to the func)
        len / 4, // Number of transfers (divide by four as we using DMA_SIZE_32 (four bytes))
        true     // Start immediately? bool
    );

    // Wait for the calculation to finish.
    dma_channel_wait_for_finish_blocking(channel);

    // Read the result (before resetting the channel)
    // Note that we must XOR the result with 0xFFFFFFFF (see above comments)
    crc = dma_hw->sniff_data ^ 0xFFFFFFFF;

    // (cleanup): Disable the sniffer
    dma_sniffer_disable();

    // (cleanup): Unclaim the DMA channel, allowing it to be used by other resources
    dma_channel_unclaim(channel);

    // Return the CRC.
    return crc;
}