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
// #include "TUM_FreeRTOS_Utils.h"
/* #include "TUM_Print.h" */

/* #include "AsyncIO.h" */

#define mainGENERIC_PRIORITY (tskIDLE_PRIORITY)
#define mainGENERIC_STACK_SIZE ((unsigned short)2560)
#define mainTask1_PRIORITY (4)
#define mainTask2_PRIORITY (3)
#define mainTask3_PRIORITY (2)

static TaskHandle_t Task1 = NULL;
static TaskHandle_t Task2 = NULL;
static TaskHandle_t Task3 = NULL;
const portTickType xPeriod1 = 17;
const portTickType xPeriod2 = 20;
const portTickType xPeriod3 = 6;

SemaphoreHandle_t semaphoreCritical;

portTickType vWorkTicks(int uWorkticks, portTickType xLastWakeTime)
{
    portTickType xCurrentTick;
    portTickType xLastTick = xLastWakeTime;
    int uLoad = uWorkticks;
    while (uLoad > 0)
    {
        xCurrentTick = xTaskGetTickCount();
        uLoad -= (xCurrentTick - xLastTick);
        xLastTick = xCurrentTick;
    }
    return xLastTick;
}

void vTaskBody1(void *pvParameters)
{
    portTickType xLastWakeTime;
    portTickType xLastTick;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        // Basic sleep of 1000 milliseconds
        /* vTaskDelay((TickType_t)1000); */
        xLastTick = vWorkTicks(2, xLastWakeTime);
        printf("xSemaphoreTake Task1\n");
        xSemaphoreTake(semaphoreCritical, portMAX_DELAY, Task1);
        // printf("Task 1\n");
        vWorkTicks(2, xLastTick);
        xSemaphoreGive(semaphoreCritical);
        printf("xSemaphoreGive Task1\n");
        vTaskDelayUntil(&xLastWakeTime, xPeriod1);
    }
}

void vTaskBody2(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        // Basic sleep of 1000 milliseconds
        /* vTaskDelay((TickType_t)1000); */
        vWorkTicks(4, xLastWakeTime);
        // printf("Task 2\n");
        /* tumFUtilPrintTaskStateList(); */
        /* tumFUtilPrintTaskUtils(); */
        vTaskDelayUntil(&xLastWakeTime, xPeriod2);
    }
}

void vTaskBody3(void *pvParameters)
{
    portTickType xLastWakeTime;
    portTickType xLastTick;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        // Basic sleep of 1000 milliseconds
        /* vTaskDelay((TickType_t)1000); */
        // printf("Task 3\n");
        xLastTick = vWorkTicks(2, xLastWakeTime);
        printf("xSemaphoreTake Task3\n");
        xSemaphoreTake(semaphoreCritical, portMAX_DELAY, Task3);
        // printf("Task 2\n");
        vWorkTicks(3, xLastTick);
        xSemaphoreGive(semaphoreCritical);
        printf("xSemaphoreGive Task3\n");
        /* tumFUtilPrintTaskStateList(); */
        /* tumFUtilPrintTaskUtils(); */
        // printf("%d\n", xLastWakeTime);
        vTaskDelayUntil(&xLastWakeTime, xPeriod3);
    }
}

int main(int argc, char *argv[])
{
    vSemaphoreCreateBinary(semaphoreCritical);

    xTaskCreate(vTaskBody1, "Task1", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainTask1_PRIORITY, &Task1); 
    xTaskCreate(vTaskBody2, "Task2", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainTask2_PRIORITY, &Task2); 
    xTaskCreate(vTaskBody3, "Task3", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainTask3_PRIORITY, &Task3); 
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
