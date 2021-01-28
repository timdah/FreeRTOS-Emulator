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

#define mainGENERIC_STACK_SIZE ((unsigned short)2560)
#define mainTaskDisplay_PRIORITY (2)
#define mainTaskInput_PRIORITY (1)
#define mainTaskTimer_PRIORITY (3)

static TaskHandle_t TaskDisplay = NULL;
static TaskHandle_t TaskInput = NULL;
static TaskHandle_t TaskTimer = NULL;
//const portTickType xPeriodDisplay = 10000;
//const portTickType xPeriodInput = 5000;
const portTickType xPeriod = 10;

void vTaskBody(void *pvParameter)
{
	portTickType xLastWakeTime;
	portTickType xCurrentTick;
	portTickType xLastTick;

	while(1) {
		uint16_t uWorkload = 5;
		xLastWakeTime = xTaskGetTickCount();
		xLastTick = xLastWakeTime;

		printf("Task 1\n");

		while (uWorkload != 0) {
			xCurrentTick = xTaskGetTickCount();
			if (xLastTick < xCurrentTick) {
				uWorkload--;
			}
		}

		vTaskDelayUntil(&xLastWakeTime, xPeriod);
	}

	vTaskDelete(NULL);
}

int main(int argc, char *argv[])
{
	xTaskCreate(
		vTaskBody, 
		"Task 1", 
		mainGENERIC_STACK_SIZE * 2, 
		NULL, 
		mainTaskDisplay_PRIORITY, 
		500,
		&TaskDisplay
	);

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
