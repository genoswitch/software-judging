// FreeRTOS core and task libraries
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "mcu_temperature.h"

#include "bulk.h"

#define MCU_TEMP_STACK_SIZE configMINIMAL_STACK_SIZE

#if configSUPPORT_STATIC_ALLOCATION
StackType_t mcu_temp_stack[MCU_TEMP_STACK_SIZE];
StaticTask_t mcu_temp_taskdef;
#endif

void pvRegisterMcuTempTask(void)
{
#if configSUPPORT_STATIC_ALLOCATION
    xTaskCreateStatic(mcu_temp_task, "tempm", MCU_TEMP_STACK_SIZE, NULL, configMAX_PRIORITIES, mcu_temp_stack, &mcu_temp_taskdef)
#else
    xTaskCreate(mcu_temp_task, "tempp", MCU_TEMP_STACK_SIZE, NULL, configMAX_PRIORITIES, NULL);
#endif
}

void mcu_temp_task(void *param)
{
    struct AMessage *pxPointerToxMessage;
    (void)param;

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    const float conversion_factor = 3.3f / (1 << 12);

    /* Block for 500ms. */
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

    vTaskDelay(xDelay);

    struct AMessage msg;
    msg.ucMessageID = 0x0f;

    pxPointerToxMessage = &msg;

    QueueHandle_t queue = getQueueHandle();
    xQueueSend(queue, (void *)&pxPointerToxMessage, (TickType_t)0);

    while (1)
    {

        uint16_t raw = adc_read();
        float result = raw * conversion_factor;
        float temp = 27 - (result - 0.706) / 0.001721;
        printf("Temp = %f C\n", temp);
        vTaskDelay(xDelay);
    }
}