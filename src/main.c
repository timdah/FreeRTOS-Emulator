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
const portTickType xPeriod1 = 1000;
const portTickType xPeriod2 = 100;
const portTickType xPeriod3 = 10;

unsigned long time_in_ms = 0; // stop watch time in ms

void vDisplayStopWatch(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        printf("%f s\n", time_in_ms / 1000.0);
        vTaskDelayUntil(&xLastWakeTime, xPeriod1);
    }
}

void vInputStopWatch(void *pvParameters)
{
    portTickType xLastWakeTime;
    while (1) {
        xLastWakeTime = xTaskGetTickCount();

        static struct termios oldt, newt;

        /*tcgetattr gets the parameters of the current terminal
        STDIN_FILENO will tell tcgetattr that it should write the settings
        of stdin to oldt*/
        tcgetattr( STDIN_FILENO, &oldt);
        /*now the settings will be copied*/
        newt = oldt;

        /*ICANON normally takes care that one line at a time will be processed
        that means it will return if it sees a "\n" or an EOF or an EOL*/
        newt.c_lflag &= ~(ICANON);          

        /*Those new settings will be set to STDIN
        TCSANOW tells tcsetattr to change attributes immediately. */
        tcsetattr( STDIN_FILENO, TCSANOW, &newt);

        char input = getchar();
        switch(input)
        {
            case 'r':
            { 
                vTaskResume(TaskDisplayStopWatch); 
                vTaskResume(TaskUpdateStopWatch);
                printf("\n");
                break;
            }
            case 's':
            { 
                vTaskSuspend(TaskDisplayStopWatch); 
                vTaskSuspend(TaskUpdateStopWatch);
                printf("\n");
                break;
            }
            case 'c':
            {
                time_in_ms = 0;
                printf("\n");
                break;
            }
        }

        tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

        vTaskDelayUntil(&xLastWakeTime, xPeriod2);
    }
}

void vUpdateStopWatch(void *pvParameters)
{
    portTickType xLastWakeTime;
    uint64_t last_time = xTaskGetTickCount();
    while (1) {
        xLastWakeTime = xTaskGetTickCount();
        time_in_ms += xTaskGetTickCount() - last_time;
        last_time = xTaskGetTickCount();
        vTaskDelayUntil(&xLastWakeTime, xPeriod3);
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
