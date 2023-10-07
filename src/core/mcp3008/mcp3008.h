// SPDX-License-Identifier: MIT

#ifndef MCP3008_MCP3008_H_
#define MCP3008_MCP3008_H_

#include "hardware/spi.h"
#include "pio/pio_spi.h"

#include "pinout.h"

// TODO: can definitely get away with a smaller size
#define BUF_LEN 3

// Datasheet specifies 3.6MHz max
// 8MHz probably works though
// See https://github.com/adafruit/Adafruit_MCP3008/issues/9
#define SPI_CLOCK_SPEED 3600000 // 3.6MHz

typedef struct
{
    spi_inst_t *spi_hw;
    pio_spi_inst_t spi_pio; // if *spi_pio, read/write operations hang.
    uint baudrate;
    spi_pinout_t *pinout;
    uint8_t input_buffer[BUF_LEN];
    bool isHw;
} spi_dual_inst;

// Function definitions
spi_dual_inst mcp3008_init(spi_pinout_t *pinout, bool IshwInstance, bool instance);
uint16_t mcp3008_read(spi_dual_inst *inst, uint8_t channel, bool differential);

#endif /* MCP3008_MCP3008_H_ */