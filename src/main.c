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
#define TaskLine1_PRIORITY (2)
#define TaskLine2_PRIORITY (3)
#define TaskLine3_PRIORITY (4)
#define TaskPrinter_PRIORITY (1)

static TaskHandle_t TaskLine1 = NULL;
static TaskHandle_t TaskLine2 = NULL;
static TaskHandle_t TaskLine3 = NULL;
static TaskHandle_t TaskPrinter = NULL;
const portTickType xPeriodLine1 = 100;
const portTickType xPeriodLine2 = 200;
const portTickType xPeriodLine3 = 300;

PriorityQueue *DispTaskLine;

_Noreturn void vLine1(void *pvParameters)
{
    portTickType xLastWakeTime;
    char *str;
    str = pvPortMalloc(5 * sizeof(char));

    while (1) {
        sprintf(str, "Line1");
        printf("vLine1: %s\n", str);
        xPriorityQueueGenericSend(DispTaskLine, &str, 0, TaskLine1);

        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xPeriodLine1);
    }
}

_Noreturn void vLine2(void *pvParameters)
{
    portTickType xLastWakeTime;
    char *str;
    str = pvPortMalloc(5 * sizeof(char));

    while (1) {
        sprintf(str, "Line2");
        printf("vLine2: %s\n", str);
        xPriorityQueueGenericSend(DispTaskLine, &str, 0,TaskLine2);

        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xPeriodLine2);
    }
}

_Noreturn void vLine3(void *pvParameters)
{
    portTickType xLastWakeTime;
    char *str;
    str = pvPortMalloc(5 * sizeof(char));

    while (1) {
        sprintf(str, "Line3");
        printf("vLine3: %s\n", str);
        xPriorityQueueGenericSend(DispTaskLine, &str, 0, TaskLine3);

        xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xPeriodLine3);
    }
}

_Noreturn void vPrinter(void *pvParameters)
{
    // portTickType xLastWakeTime;
    char *xMessage;

    while (1) {
        // if (xPriorityQueueGenericReceivePeekHigh(DispTaskLine, &xMessage)) {
        //     printf("PeekHigh: \t%s\n", xMessage);
        // }
        if (xPriorityQueueGenericReceiveHigh(DispTaskLine, &xMessage)) {
            printf("ReceiveHigh: \t%s\n", xMessage);
        }
        // if (xPriorityQueueGenericReceiveLow(DispTaskLine, &xMessage)) {
        //     printf("ReceiveLow: \t%s\n", xMessage);
        // }
        // xLastWakeTime = xTaskGetTickCount();
        // vTaskDelayUntil(&xLastWakeTime, 1);
    }
}

int main(int argc, char *argv[])
{
    DispTaskLine = xPriorityQueueGenericCreate(6, 5 * sizeof(char *));

    xTaskCreate(vLine1, "Line1", mainGENERIC_STACK_SIZE * 2, NULL,
                    TaskLine1_PRIORITY, &TaskLine1); 
    xTaskCreate(vLine2, "Line2", mainGENERIC_STACK_SIZE * 2, NULL,
                    TaskLine2_PRIORITY, &TaskLine2); 
    xTaskCreate(vLine3, "Line3", mainGENERIC_STACK_SIZE * 2, NULL,
                    TaskLine3_PRIORITY, &TaskLine3);
    xTaskCreate(vPrinter, "Printer", mainGENERIC_STACK_SIZE * 2, NULL,
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
