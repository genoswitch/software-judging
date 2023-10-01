#ifndef PINOUT_H_
#define PINOUT_H_

typedef struct TU_ATTR_PACKED
{
    int miso;
    int mosi;
    int sck;
    int csn;
} spi_pinout_t;

#endif /* PINOUT_H_ */