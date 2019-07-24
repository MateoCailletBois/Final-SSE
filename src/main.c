#include "stm32f4xx.h"
#include "stm32f411e_discovery.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp.h"

TaskHandle_t Task;

void vTaskLed( void *pvParameters );
const TickType_t xDelay250ms = pdMS_TO_TICKS( 250 );

int main(void) {

	BSP_Init();

	xTaskCreate( vTaskLed, "Task 1", 100, NULL, 1, &Task );

	vTaskStartScheduler();

	for (;;);
}

void vTaskLed( void *pvParameters) {

	while(1) {
		led_on(LED1_BOAR);
		vTaskDelay( xDelay250ms );
		led_off(LED1_BOAR);
		vTaskDelay( xDelay250ms );
	}
}
