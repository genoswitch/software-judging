#include "mcp3008_pio.h"

// clk_sys enum
#include "hardware/clocks.h"

spi_dual_inst mcp3008_init_pio(PIO pio, uint baudrate, spi_pinout_t *pinout)
{
    pio_spi_inst_t spi = {
        .pio = pio,                          // pio to use (pio0, pio1)
        .sm = pio_claim_unused_sm(pio, true) // claim first free state machine
    };

    // Based on: https://medium.com/geekculture/raspberry-pico-programming-with-pio-state-machines-e4610e6b0f29
    // *Should* make the spi clock speed correct.
    float clock_divider = (float)clock_get_hz(clk_sys) / SPI_CLOCK_SPEED;
    // float clkdiv = 31.25f; // 1 MHz @ 125 clk_sys

    uint cpha0_prog_offs = pio_add_program(spi.pio, &spi_cpha0_program);

    pio_spi_init(
        spi.pio,         // pio
        spi.sm,          // state machine (num)
        cpha0_prog_offs, // program offsets
        8,               // number of bits per SPI frame
        clock_divider,   // Clock divider
        0,               // cpha
        1,               // cpol
        pinout->sck,     // SCK pin,
        pinout->mosi,    // MOSI pin
        pinout->miso     // MISO pin
    );

    // Setup CS pin
    gpio_init(pinout->csn);
    gpio_set_dir(pinout->csn, GPIO_OUT);
    gpio_put(pinout->csn, 1);

    spi_dual_inst inst = {
        .spi_pio = spi,
        .baudrate = baudrate,
        .pinout = pinout,
        .isHw = false,
    };

    // Create an input buffer and initialise it to zero.
    for (uint8_t i = 0; i < BUF_LEN; i++)
    {
        inst.input_buffer[i] = 0;
    }

    return inst;
}

uint16_t mcp3008_read_pio(spi_dual_inst *inst, uint8_t channel, bool differential)
{
    if (!inst->isHw)
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

        // Send SPI command
        gpio_put(inst->pinout->csn, 0);

        pio_spi_write8_read8_blocking(&(inst->spi_pio), output_buffer, input_buffer, BUF_LEN);
        gpio_put(inst->pinout->csn, 1);

        uint16_t data = (((uint16_t)(input_buffer[1] & 0x03)) << 8) | input_buffer[2];

        return data;
    }
    else
    {
        printf("\n(error): [mcp3008]: HW instance attempted to call PIO handler.");
    }
}