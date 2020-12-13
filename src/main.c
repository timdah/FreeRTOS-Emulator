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

#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)
#define mainTask1_PRIORITY (2)
#define mainTask2_PRIORITY (3)
#define mainTask3_PRIORITY (4)
#define printer_PRIORITY (1)

static TaskHandle_t ProductionLine1 = NULL;
static TaskHandle_t ProductionLine2 = NULL;
static TaskHandle_t ProductionLine3 = NULL;
static TaskHandle_t Printer = NULL;
const portTickType xPeriod1 = 1000 / portTICK_RATE_MS;
const portTickType xPeriod2 = 2000 / portTICK_RATE_MS;
const portTickType xPeriod3 = 3000 / portTICK_RATE_MS;
const portTickType xPeriodPrinter = 500 / portTICK_RATE_MS;

static xQueueHandle printerQueue;

void vTaskPrinter(void *pvParameters)
{
    uint8_t pucBuffer[10];
    BaseType_t ucCounter = 0U;
    BaseType_t xResult = pdFALSE;
    while (1) {
        ucCounter = (ucCounter + 1) % 3;
        if (ucCounter == 0) {
            xResult = xQueueReceiveLow(printerQueue, pucBuffer, xPeriodPrinter);
        } else if (ucCounter == 1) {
            xResult = xQueueReceiveHigh(printerQueue, pucBuffer, xPeriodPrinter);
        } else {
            xResult = xQueuePeekHigh(printerQueue, pucBuffer, xPeriodPrinter);
        }

        if (xResult == pdTRUE) {
            printf(">> printer: %s ", pucBuffer);
            if (ucCounter == 0) {
                printf("LOW\n");
            } else if (ucCounter == 1) {
                printf("HIGH\n");
            } else {
                printf("PEEK HIGH\n");
            }
        } else {
            printf("---------------\n");
        }
    }
}

void vTaskProductionLine1(void *pvParameters)
{
    portTickType xLastWakeTime;

    while (1) {
        xLastWakeTime = xTaskGetTickCount();

        printf("Task 1\n");

        xQueueSendToBack(printerQueue, "Task 1", xPeriod1);
        vTaskDelayUntil(&xLastWakeTime, xPeriod1);
    }
}

void vTaskProductionLine2(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();

        printf("Task 2\n");

        xQueueSendToBack(printerQueue, "Task 2", xPeriod2);
        vTaskDelayUntil(&xLastWakeTime, xPeriod2);
    }
}

void vTaskProductionLine3(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();

        printf("Task 3\n");

        xQueueSendToBack(printerQueue, "Task 3", xPeriod3);
        vTaskDelayUntil(&xLastWakeTime, xPeriod3);
    }
}

int main(int argc, char *argv[])
{
    xTaskCreate(vTaskProductionLine1, "Task1", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainTask1_PRIORITY, &ProductionLine1);
    xTaskCreate(vTaskProductionLine2, "Task2", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainTask2_PRIORITY, &ProductionLine2);
    xTaskCreate(vTaskProductionLine3, "Task3", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainTask3_PRIORITY, &ProductionLine3);
    xTaskCreate(vTaskPrinter, "Printer", mainGENERIC_STACK_SIZE * 2, NULL,
                    printer_PRIORITY, &Printer);

    printerQueue = xQueueCreate(10, 10);
    if (printerQueue == NULL) {
        //error: handle it
    }

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