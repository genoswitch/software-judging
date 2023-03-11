void pvCreateBulkQueue(void);
void bulk_queue_task(void *param);
void pvRegisterBulkQueueTask(void);

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
QueueHandle_t getQueueHandle(void);

struct AMessage
{
    char ucMessageID;
    char ucData[20];
} xMessage;