void pvRegisterMcuTempTask(void);
void mcu_temp_task(void *param);

struct ADCTempMessage // Len: 8
{
    uint32_t bMagic;   // 4 bytes
    uint8_t bLength;   // 1 byte
    uint8_t bCode;     // 1 byte
    uint16_t temp_adc; // 2 bytes
} temp_message;