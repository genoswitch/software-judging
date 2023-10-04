// Accompanying header file
#include "mcp3008.h"

// PIO and hardware mcp3008 functions
#include "./hardware/mcp3008_hardware.h"
#include "./pio/mcp3008_pio.h"

spi_dual_inst mcp3008_init(spi_pinout_t *pinout, bool IshwInstance, bool instance)
{
    if (IshwInstance)
    {
        // Init hardware

        // Determine weather to use spi0 or spi1
        // If 0 / false, SPI 0.
        // If 1 / true, SPI 1.
        spi_inst_t *spi = (instance ? spi1 : spi0);

        return mcp3008_init_hardware(spi, SPI_CLOCK_SPEED, pinout);
    }
    else
    {
        // Init PIO

        // Determine weather to use pio0 or pio1
        // If 0 / false, PIO 0.
        // If 1 / true, PIO 1.
        PIO pio = (instance ? pio1 : pio0);

        return mcp3008_init_pio(pio, SPI_CLOCK_SPEED, pinout);
    }
}

uint16_t mcp3008_read(spi_dual_inst *inst, uint8_t channel, bool differential)
{
    if (inst->isHw)
    {
        return mcp3008_read_hardware(inst, channel, differential);
    }
    else
    {
        return mcp3008_read_pio(inst, channel, differential);
    }
}