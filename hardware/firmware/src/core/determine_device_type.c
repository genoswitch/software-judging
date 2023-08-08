#include <stdio.h>

#include "pico/stdlib.h"

#define BIT_POSITION_0_PIN 22
#define BIT_POSITION_1_PIN 21
#define BIT_POSITION_2_PIN 20

void initGPIO(uint gpio)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
}

// Based on https://www.geeksforgeeks.org/modify-bit-given-position/
uint setBitPosition(uint byte, bool bit, int pos)
{
    int mask = 1 << pos;
    return ((byte & ~mask) | (bit << pos));
}

// 3 bits
uint determineDevice()
{
    initGPIO(BIT_POSITION_0_PIN);
    initGPIO(BIT_POSITION_1_PIN);
    initGPIO(BIT_POSITION_2_PIN);

    uint device = 0;

    device = setBitPosition(device, gpio_get(BIT_POSITION_0_PIN), 0);
    device = setBitPosition(device, gpio_get(BIT_POSITION_1_PIN), 1);
    device = setBitPosition(device, gpio_get(BIT_POSITION_2_PIN), 2);

    return device;
}