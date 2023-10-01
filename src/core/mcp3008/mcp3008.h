#ifndef MCP3008_MCP3008_H_
#define MCP3008_MCP3008_H_

#include "hardware/spi.h"
#include "pio/pio_spi.h"

#include "pinout.h"

// TODO: can definitely get away with a smaller size
#define BUF_LEN 3

typedef struct
{
    spi_inst_t *spi_hw;
    pio_spi_inst_t spi_pio; // if *spi_pio, read/write operations hang.
    uint baudrate;
    spi_pinout_t *pinout;
    uint8_t input_buffer[BUF_LEN];
    bool isHw;
} spi_dual_inst;

#endif /* MCP3008_MCP3008_H_ */