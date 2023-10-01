#ifndef HARDWARE_MCP3008_HARDWARE_H_
#define HARDWARE_MCP3008_HARDWARE_H_

#include "hardware/spi.h"

#include "../pinout.h"
#include "../mcp3008.h"

spi_dual_inst mcp3008_init_hardware(spi_inst_t *spi, uint baudrate, spi_pinout_t *pinout);
uint16_t mcp3008_internal_do_adc(spi_dual_inst *inst, uint8_t channel, bool differential);

#endif /* HARDWARE_MCP3008_HARDWARE_H_ */