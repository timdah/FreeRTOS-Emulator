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
#define mainTask1_PRIORITY (1)
#define mainTask2_PRIORITY (1)
#define mainTask3_PRIORITY (1)

static TaskHandle_t Task1 = NULL;
static TaskHandle_t Task2 = NULL;
static TaskHandle_t Task3 = NULL;
const portTickType xPeriod1 = 4;
const portTickType xPeriod2 = 4;
const portTickType xPeriod3 = 10000;

void vTaskBody1(void *pvParameters)
{
    portTickType xLastWakeTime;
    portTickType xCurrentTick;
    portTickType xLastTick;

    while(1) {
        uint16_t uWorkload = 2;
        xLastWakeTime = xTaskGetTickCount();
        xLastTick = xLastWakeTime;

        printf("\tTask In: Task 1\n");

        while (uWorkload != 0) {
            xCurrentTick = xTaskGetTickCount();
            if (xLastTick < xCurrentTick) {
                xLastTick = xCurrentTick;
                uWorkload--;
            }
        }

        vTaskDelayUntil(&xLastWakeTime, xPeriod1);
    }
}

void vTaskBody2(void *pvParameters)
{
    portTickType xLastWakeTime;
    portTickType xCurrentTick;
    portTickType xLastTick;

    while(1) {
        uint16_t uWorkload = 2;
        xLastWakeTime = xTaskGetTickCount();
        xLastTick = xLastWakeTime;

        printf("\tTask In: Task 2\n");

        while (uWorkload != 0) {
            xCurrentTick = xTaskGetTickCount();
            if (xLastTick < xCurrentTick) {
                xLastTick = xCurrentTick;
                uWorkload--;
            }
        }

        vTaskDelayUntil(&xLastWakeTime, xPeriod2);
    }
}

void vTaskBody3(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        // Basic sleep of 1000 milliseconds
        /* vTaskDelay((TickType_t)1000); */
        printf("Task 3\n");
        /* tumFUtilPrintTaskStateList(); */
        /* tumFUtilPrintTaskUtils(); */
        vTaskDelayUntil(&xLastWakeTime, xPeriod3);
    }
}


int main(int argc, char *argv[])
{
    xTaskCreate(vTaskBody1, "Task1", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainTask1_PRIORITY, &Task1, 0, 0, 4); 
    xTaskCreate(vTaskBody2, "Task2", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainTask2_PRIORITY, &Task2, 0, 0, 4); 
    // xTaskCreate(vTaskBody3, "Task3", mainGENERIC_STACK_SIZE * 2, NULL,
    //                 mainTask3_PRIORITY, &Task3, 2, 5, 5); 
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
