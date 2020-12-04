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
#define mainTask1_PRIORITY (1)
#define mainTask2_PRIORITY (2)
#define mainTask3_PRIORITY (3)

static TaskHandle_t TaskDisplayStopWatch = NULL;
static TaskHandle_t TaskInputStopWatch = NULL;
static TaskHandle_t TaskUpdateStopWatch = NULL;
const portTickType xPeriodDisplayStopWatch = 100;
const portTickType xPeriodInputStopWatch = 100;
const portTickType xPeriodUpdateStopWatch = 10;

unsigned long time_in_ms = 0; // stop watch time in ms
uint64_t last_time = 0; // last time for measurement task

/**
 * Displaying stopwatch task
 * @param pvParameters
 */
_Noreturn void vDisplayStopWatch(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        printf("\r%.1f s\t\t\t", time_in_ms / 1000.0);
        vTaskDelayUntil(&xLastWakeTime, xPeriodDisplayStopWatch);
    }
}

/**
 * Handling user input task
 * @param pvParameters
 */
_Noreturn void vInputStopWatch(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();

        static struct termios oldt, newt;

        // modify stdin to pass input without enter
        tcgetattr( STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON);          
        tcsetattr( STDIN_FILENO, TCSANOW, &newt);

        // get the current input char
        char input = getchar();
        switch(input)
        {
            case 'r':
            {
                last_time = xTaskGetTickCount();
                vTaskResume(TaskDisplayStopWatch); 
                vTaskResume(TaskUpdateStopWatch);
                // clear console for prettier displaying
                printf("%c[2K", 27);
                break;
            }
            case 's':
            { 
                vTaskSuspend(TaskDisplayStopWatch); 
                vTaskSuspend(TaskUpdateStopWatch);
                // clear console and rewrite for prettier displaying
                printf("%c[2K", 27);
                printf("\r%.1f s\t\t\t", time_in_ms / 1000.0);
                break;
            }
            case 'c':
            {
                time_in_ms = 0;
                // clear console and rewrite for prettier displaying
                printf("%c[2K", 27);
                printf("\r%.1f s\t\t\t", time_in_ms / 1000.0);
                break;
            }
        }

        tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

        vTaskDelayUntil(&xLastWakeTime, xPeriodInputStopWatch);
    }
}

/**
 * Time measurement task
 * @param pvParameters
 */
_Noreturn void vUpdateStopWatch(void *pvParameters)
{
    portTickType xLastWakeTime;
    last_time = xTaskGetTickCount();
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        time_in_ms += xTaskGetTickCount() - last_time;
        last_time = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xPeriodUpdateStopWatch);
    }
}

int main(int argc, char *argv[])
{
    xTaskCreate(vDisplayStopWatch, "Task1", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainTask1_PRIORITY, &TaskDisplayStopWatch); 
    xTaskCreate(vInputStopWatch, "Task2", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainTask2_PRIORITY, &TaskInputStopWatch); 
    xTaskCreate(vUpdateStopWatch, "Task3", mainGENERIC_STACK_SIZE * 2, NULL,
                    mainTask3_PRIORITY, &TaskUpdateStopWatch); 
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