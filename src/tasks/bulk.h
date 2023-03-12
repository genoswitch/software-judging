// FreeRTOS core and queue libraries for QueueHandle_t typedef
#include "FreeRTOS.h"
#include "queue.h"

void pvCreateBulkQueue(void);
void bulk_queue_task(void *param);
QueueHandle_t getQueueHandle(void);

#define MAGIC_BYTE 0xdd24433b // Bear in mind this is little endian so will show up backwards!

struct BaseMessage
{
    uint32_t bMagic;
    uint8_t bLength;
    uint8_t bCode;
} base_message;