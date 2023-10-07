// SPDX-License-Identifier: MIT

#ifndef MCP3008_PINOUT_H_
#define MCP3008_PINOUT_H_

typedef struct TU_ATTR_PACKED
{
    int miso;
    int mosi;
    int sck;
    int csn;
} spi_pinout_t;

#endif /* MCP3008_PINOUT_H_ */