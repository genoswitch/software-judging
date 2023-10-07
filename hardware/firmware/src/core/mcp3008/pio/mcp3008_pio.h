// SPDX-License-Identifier: MIT

#ifndef PIO_MCP3008_PIO_H_
#define PIO_MCP3008_PIO_H_

#include "pico/stdlib.h"
#include "pio_spi.h"

#include "../mcp3008.h"

spi_dual_inst mcp3008_init_pio(PIO pio, uint baudrate, spi_pinout_t *pinout);
uint16_t mcp3008_read_pio(spi_dual_inst *inst, uint8_t channel, bool differential);

#endif /* PIO_MCP3008_PIO_H_ */