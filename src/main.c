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
#define mainTask2_PRIORITY (2)
#define mainTask3_PRIORITY (3)

static TaskHandle_t TaskDisplay = NULL;
static TaskHandle_t TaskInput = NULL;
static TaskHandle_t TaskTiming = NULL;
const portTickType xPeriodDisplay = 1000;
const portTickType xPeriodInput = 5000;
const portTickType xPeriodTiming = 10000;

void vTaskDisplay(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        // Basic sleep of 1000 milliseconds
        /* vTaskDelay((TickType_t)1000); */
        printf("Task 1\n");
        vTaskDelayUntil(&xLastWakeTime, xPeriodDisplay);
	vTaskSuspend(TaskInput);
	vTaskSuspend(TaskTiming);
    }
}

void vTaskInput(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        // Basic sleep of 1000 milliseconds
        /* vTaskDelay((TickType_t)1000); */
        printf("Task 2\n");
        /* tumFUtilPrintTaskStateList(); */
        /* tumFUtilPrintTaskUtils(); */
        vTaskDelayUntil(&xLastWakeTime, xPeriodInput);
    }
}

void vTaskTiming(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        // Basic sleep of 1000 milliseconds
        /* vTaskDelay((TickType_t)1000); */
        printf("Task 3\n");
        /* tumFUtilPrintTaskStateList(); */
        /* tumFUtilPrintTaskUtils(); */
        vTaskDelayUntil(&xLastWakeTime, xPeriodTiming);
    }
}



int main(int argc, char *argv[])
{
    xTaskCreate(vTaskDisplay, "TaskDisplay", mainGENERIC_STACK_SIZE * 2, NULL, mainTask1_PRIORITY, &TaskDisplay);
    xTaskCreate(vTaskInput, "TaskInput", mainGENERIC_STACK_SIZE * 2, NULL, mainTask2_PRIORITY, &TaskInput);
    xTaskCreate(vTaskTiming, "TaskTiming", mainGENERIC_STACK_SIZE * 2, NULL, mainTask3_PRIORITY, &TaskTiming);
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
