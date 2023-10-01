#ifndef MCP3008_H_
#define MCP3008_H_

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

spi_dual_inst mcp3008_init_hardware(spi_inst_t *spi, uint baudrate, spi_pinout_t *pinout);
uint16_t mcp3008_internal_do_adc(spi_dual_inst *inst, uint8_t channel, bool differential);

#endif /* MCP3008_H_ */