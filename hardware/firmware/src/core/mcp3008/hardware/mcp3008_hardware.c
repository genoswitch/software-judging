// SPDX-License-Identifier: MIT

#include "mcp3008_hardware.h"

spi_dual_inst mcp3008_init_hardware(spi_inst_t *spi, uint baudrate, spi_pinout_t *pinout)
{
    spi_dual_inst inst = {
        .spi_hw = spi,
        //.baudrate = baudrate,
        .pinout = pinout,
        .isHw = true,
    };

    // Init SPIx at specified baudrate
    inst.baudrate = spi_init(inst.spi_hw, baudrate);

    // Assign SPI functions to pins
    gpio_set_function(inst.pinout->miso, GPIO_FUNC_SPI);
    gpio_set_function(inst.pinout->sck, GPIO_FUNC_SPI);
    gpio_set_function(inst.pinout->mosi, GPIO_FUNC_SPI);
    // gpio_set_function(5, GPIO_FUNC_SPI);

    gpio_init(inst.pinout->csn);
    gpio_set_dir(inst.pinout->csn, GPIO_OUT);
    gpio_put(inst.pinout->csn, 1);

    // Create an input buffer and initialise it to zero.
    for (uint8_t i = 0; i < BUF_LEN; i++)
    {
        inst.input_buffer[i] = 0;
    }

    return inst;
}

// -- Internal function to handle sending data --
uint16_t mcp3008_read_hardware(spi_dual_inst *inst, uint8_t channel, bool differential)
{
    if (inst->isHw)
    {
        // Create a buffer to sent to the device initialised to zero
        uint8_t output_buffer[BUF_LEN];
        for (uint8_t i = 0; i < BUF_LEN; i++)
        {
            output_buffer[i] = 0;
        }

        // Byte 1, leading zeros ending with a start bit
        output_buffer[0] = 0x01;

        // Byte 2
        // Bit 1, differential signal selection
        // Bits 2-4 channel select
        // Rest of buffer ignored
        output_buffer[1] = ((differential ? 0 : 1) << 7 | channel << 4);

        // temp
        uint8_t input_buffer[BUF_LEN];
        for (uint8_t i = 0; i < BUF_LEN; i++)
        {
            input_buffer[i] = 0;
        }

        // Send the SPI command

        gpio_put(inst->pinout->csn, 0);
        __breakpoint();
        spi_write_read_blocking(inst->spi_hw, output_buffer, input_buffer, BUF_LEN);
        gpio_put(inst->pinout->csn, 1);

        // Response
        // 13 bits garbage
        // 1 bit null (0)
        // data, ordered bits 9 to 0
        // mask 0x07 last 3 bits
        // mask 0x03 last 2 bits
        // return (((uint16_t)(input_buffer[1] & 0x03)) << 8) | input_buffer[2];
        return (((uint16_t)(input_buffer[1] & 0x03)) << 8) | input_buffer[2];
    }
    else
    {
        printf("\n(error) [mcp3008]: PIO instance attempted to call HW handler.");
    }
}