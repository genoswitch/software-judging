#include <stdint.h>

enum
{
    IHEX_TYPE_DATA,
    IHEX_TYPE_EOF,
    IHEX_TYPE_END_SEG_ADDR,
    IHEX_TYPE_START_SEG_ADDR,
    IHEX_TYPE_EXT_LIN_ADDR,
    IHEX_TYPE_START_LIN_ADDR
};

typedef struct
{
    uint8_t count;
    uint16_t addr;
    uint8_t type;
    uint8_t data[256]; // rhulme (max size 256bytes)
} ihexRecord;

int parseRecord(const char *line, ihexRecord *record);