#include "FreeRTOS.h"


static void prvSetupHardware( void );

int main( void )
{
    prvSetupHardware();
    const char *rtos_name;
#if ( portSUPPORT_SMP == 1 )
    rtos_name = "FreeRTOS SMP";
#else
    rtos_name = "FreeRTOS";
#endif

#if ( portSUPPORT_SMP == 1 ) && ( configNUM_CORES == 2 )
    printf("%s on both cores:\n", rtos_name);
#endif

#if ( mainRUN_ON_CORE == 1 )
    printf("%s on core 1:\n", rtos_name);
#else
    printf("%s on core 0:\n", rtos_name);
#endif

    return 0;
}


static void prvSetupHardware( void )
{
    stdio_init_all(); // init the things uwu
}