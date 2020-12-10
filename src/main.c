#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL_scancode.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

/* #include "TUM_Ball.h" */
/* #include "TUM_Draw.h" */
/* #include "TUM_Font.h" */
/* #include "TUM_Event.h" */
/* #include "TUM_Sound.h" */
/* #include "TUM_Utils.h" */
#include "TUM_FreeRTOS_Utils.h"
/* #include "TUM_Print.h" */

/* #include "AsyncIO.h" */

#include <termios.h>
#include <unistd.h>

#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)
#define TaskLine1_PRIORITY (1)
#define TaskLine2_PRIORITY (1)
#define TaskLine3_PRIORITY (1)
#define TaskPrinter_PRIORITY (1)

static TaskHandle_t TaskLine1 = NULL;
static TaskHandle_t TaskLine2 = NULL;
static TaskHandle_t TaskLine3 = NULL;
static TaskHandle_t TaskPrinter = NULL;
const portTickType xPeriodLine1 = 100;
const portTickType xPeriodLine2 = 200;
const portTickType xPeriodLine3 = 300;
const portTickType xPeriodPrinter = 100;



_Noreturn void vLine1(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xPeriodLine1);
    }
}

_Noreturn void vLine2(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xPeriodLine2);
    }
}

_Noreturn void vLine3(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xPeriodLine2);
    }
}

_Noreturn void vPrinter(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xPeriodPrinter);
    }
}

int main(int argc, char *argv[])
{
    xTaskCreate(vLine1, "Line1", mainGENERIC_STACK_SIZE * 2, NULL,
                    TaskLine1_PRIORITY, &TaskLine1); 
    xTaskCreate(vLine2, "Line2", mainGENERIC_STACK_SIZE * 2, NULL,
                    TaskLine2_PRIORITY, &TaskLine2); 
    xTaskCreate(vLine3, "Line3", mainGENERIC_STACK_SIZE * 2, NULL,
                    TaskLine3_PRIORITY, &TaskLine3);
    xTaskCreate(vPrinter, "Line3", mainGENERIC_STACK_SIZE * 2, NULL,
                    TaskPrinter_PRIORITY, &TaskPrinter); 
    vTaskStartScheduler();

    return EXIT_SUCCESS;
}

// cppcheck-suppress unusedFunction
__attribute__((unused)) void vMainQueueSendPassed(void)
{
    /* This is just an example implementation of the "queue send" trace hook. */
}

// cppcheck-suppress unusedFunction
__attribute__((unused)) void vApplicationIdleHook(void)
{
#ifdef __GCC_POSIX__
    struct timespec xTimeToSleep, xTimeSlept;
    /* Makes the process more agreeable when using the Posix simulator. */
    xTimeToSleep.tv_sec = 1;
    xTimeToSleep.tv_nsec = 0;
    nanosleep(&xTimeToSleep, &xTimeSlept);
#endif
}
