#include "record.h"

#include "util.c"

// memset, memcpy
#include "pico/mem_ops.h"

// Get the exact address for the specified record.
uint32_t getAddress(ihexRecord *record) {
    return record->ulba + record->addr;
}

// Converts an Intel hex record in text form to a binary representation.
// Returns non-zero if the text could be parsed successfully
int parseRecord(const char *line, ihexRecord *record)
{
    int success = 0;
    int offset = 0;
    uint8_t value;
    uint8_t data[256 + 5]; // Max payload 256 bytes plus 5 for fields
    uint8_t checksum = 0;

    // again, "" no work
    while (*line && (*line != ':'))
        line++;

    if (*line++ == ':')
    {
        while (parseHex(line, &value) && (offset < sizeof(data)))
        {
            data[offset++] = value;
            checksum += value;
            line += 2;
        }
    }

    // Check the checksum for a record (line)
    // The checksum is a two's complement sum of the previous bytes in the record.
    // https://en.wikipedia.org/wiki/Intel_HEX#:~:text=%C2%A0Checksum-,Checksum%20calculation,-%5Bedit%5D
    // (rhulme): Final checksum should be 0 if everything is okay.
    if ((offset > 0) && (checksum == 0))
    {
        record->count = data[0];
        record->addr = data[2] | (data[1] << 8);
        record->type = data[3];
        memcpy(record->data, &data[4], data[0]);

        // if EXT_LIN_ADDR, set ulba
        if (record->type == IHEX_TYPE_EXT_LIN_ADDR) {
            record->ulba = ((data[4] << 8) + data[5]) << 16;
        }

        success = 1;
    }

    return success;
}