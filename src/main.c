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
const portTickType xPeriodTimer = 10;

typedef enum {
	StopWatchStop, 
	StopWatchRun, 
	StopWatchClear, 
} StopWatchState;

StopWatchState xCurState = StopWatchClear;

uint32_t uiTimerCounter = 0;
uint32_t uiTimerCounterBck = 0;

/**
 * This task updates the output depending on the state of the Stop-Watch. 
 * It does not require exact timing. It must not update any data variables.
 */
void vTaskBodyDisplay(void *pvParameter)
{
	while(1) {
		if (xCurState == StopWatchStop) {
			uint8_t ucMS = uiTimerCounter % 100;
			uint8_t ucS = uiTimerCounter / 100 % 60;
			uint8_t ucM = uiTimerCounter / 100 /60;
			printf("Timer stands at %02d:%02d:%02d (mm:ss:ff).\nPress 'ENTER' to clear the timer: ", ucM, ucS, ucMS);
		} else if(xCurState == StopWatchRun) {
			uint8_t ucMS = uiTimerCounter % 100;
			uint8_t ucS = uiTimerCounter / 100 % 60;
			uint8_t ucM = uiTimerCounter / 100 /60;
			printf("%02d:%02d:%02d\n", ucM, ucS, ucMS);
		} else { // xCurState == StopWatchClear
			printf("Press 'ENTER' to start and again to stop the timer: ");
		}

		vTaskSuspend(TaskDisplay);
	}

	vTaskDelete(NULL);
}

void vTaskBodyInput(void *pvParameter)
{
	char pcInput[256];

	while(1) {
		char *pcRes = fgets(pcInput, 256, stdin);
		if (pcRes != NULL) {
			if (xCurState == StopWatchClear) {
				xCurState = StopWatchRun;
				vTaskResume(TaskTimer);
			} else if (xCurState == StopWatchRun) {
				xCurState = StopWatchStop;
				vTaskResume(TaskDisplay);
			} else { // xCurState == StopWatchStop
				xCurState = StopWatchClear;
				vTaskResume(TaskDisplay);
			}
		}
	}

	vTaskDelete(NULL);
}

void vTaskBodyTimer(void *pvParameter)
{
	portTickType xLastWakeTime;

	while(1) {
		xLastWakeTime = xTaskGetTickCount();

		if (xCurState != StopWatchRun) {
			uiTimerCounterBck = uiTimerCounter;
			uiTimerCounter = 0;
			vTaskSuspend(TaskTimer);
		}

		uiTimerCounter++;

		vTaskResume(TaskDisplay);
		vTaskDelayUntil(&xLastWakeTime, xPeriodTimer);
	}

	vTaskDelete(NULL);
}

int main(int argc, char *argv[])
{
	xTaskCreate(
		vTaskBodyDisplay, 
		"Display Task", 
		mainGENERIC_STACK_SIZE * 2, 
		NULL, 
		mainTaskDisplay_PRIORITY, 
		&TaskDisplay
	);
	
	xTaskCreate(
		vTaskBodyInput, 
		"Input Task", 
		mainGENERIC_STACK_SIZE * 2, 
		NULL, 
		mainTaskInput_PRIORITY, 
		&TaskInput
	);

	xTaskCreate(
		vTaskBodyTimer, 
		"Timer Task", 
		mainGENERIC_STACK_SIZE * 2, 
		NULL, 
		mainTaskTimer_PRIORITY, 
		&TaskTimer
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
