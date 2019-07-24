#include "stm32f4xx.h"
#include "stm32f411e_discovery.h"
#include "FreeRTOS.h"
#include "task.h"


void vTaskLed( void *pvParameters );
const TickType_t xDelay250ms = pdMS_TO_TICKS( 1000 );
Led_TypeDef Led1 = LED3;
TaskHandle_t Task1;

int main(void) {
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED6);

	xTaskCreate( vTaskLed, "Task 1", 100, (void* const)&Led1, 1, &Task1 );
	vTaskStartScheduler();

	for(;;);
}

void vTaskLed( void *pvParameters) {

	Led_TypeDef* Led = (Led_TypeDef*) pvParameters;

	while(1) {
		BSP_LED_On(*Led);
		vTaskDelay( xDelay250ms );

		BSP_LED_Off(*Led);
		vTaskDelay( xDelay250ms );
	}
}
