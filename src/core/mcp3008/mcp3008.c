#include "mcp3008.h"

// gpio_set_function
#include "pico/stdlib.h"

spi_dual_inst mcp3008_init_hardware(spi_inst_t *spi, uint baudrate, spi_pinout_t *pinout)
{
    spi_dual_inst inst = {
        .spi_hw = spi,
        //.baudrate = baudrate,
        .pinout = pinout};

    // Init SPIx at specified baudrate
    inst.baudrate = spi_init(inst.spi_hw, baudrate);

    // Assign SPI functions to pins
    gpio_set_function(inst.pinout->rx, GPIO_FUNC_SPI);
    gpio_set_function(inst.pinout->sck, GPIO_FUNC_SPI);
    gpio_set_function(inst.pinout->tx, GPIO_FUNC_SPI);
    // gpio_set_function(5, GPIO_FUNC_SPI);

    gpio_init(5);
    gpio_set_dir(5, GPIO_OUT);
    gpio_put(5, 1);

    // Create an input buffer and initialise it to zero.
    for (uint8_t i = 0; i < BUF_LEN; i++)
    {
        inst.input_buffer[i] = 0;
    }

    return inst;
}

// -- Internal function to handle sending data --
#define OUTPUT_BUFFER_LEN 3
uint16_t mcp3008_internal_do_adc(spi_dual_inst *inst, uint8_t channel, bool differential)
{
    // Create a buffer to sent to the device initialised to zero
    uint8_t output_buffer[OUTPUT_BUFFER_LEN];
    for (uint8_t i = 0; i < OUTPUT_BUFFER_LEN; i++)
    {
        output_buffer[i] = 0;
    }

    // Byte 1, leading zeros ending with a start bit
    output_buffer[0] = 0x01;

    // Byte 2
    // Bit 1, differential signal sleection
    // Bits 2-4 channel select
    // Rest of buffer ignored
    output_buffer[1] = ((differential ? 0 : 1) >> 7 | channel << 4);

    // temp
    uint8_t input_buffer[BUF_LEN];
    for (uint8_t i = 0; i < BUF_LEN; i++)
    {
        input_buffer[i] = 0;
    }

    // Send the SPI command

    gpio_put(5, 0);
    spi_write_read_blocking(spi0, output_buffer, input_buffer, BUF_LEN);
    gpio_put(5, 1);

    printf("RECIEVED DATA");

    uint16_t data = (((uint16_t)(input_buffer[1] & 0x03)) << 8) | input_buffer[2];

    return data;

    // Response
    // 13 bits garbage
    // 1 bit null (0)
    // data, ordered bits 9 to 0
    // mask 0x07 last 3 bits
    // mask 0x03 last 2 bits
    // return (((uint16_t)(input_buffer[1] & 0x03)) << 8) | input_buffer[2];
}