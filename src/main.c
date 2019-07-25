#include "stm32f4xx.h"
#include "stm32f411e_discovery.h"
#include "FreeRTOS.h"
#include "task.h"


void vTaskLedPeriodico( void *pvParameters );
void vTaskLedUsuario( void *pvParameters );

const TickType_t xPeriodo = pdMS_TO_TICKS( 1000 );
Led_TypeDef led_naranja = LED3;
Led_TypeDef led_verde = LED4;
Button_TypeDef sw_usuario = BUTTON_KEY;
TaskHandle_t Task1;

int main(void) {
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_PB_Init(sw_usuario, BUTTON_MODE_GPIO);

	xTaskCreate( vTaskLedPeriodico, "TaskLedPeriodico", 100, (void* const)&led_naranja, 1, &Task1 );
	xTaskCreate( vTaskLedUsuario, "TaskLedUsuario", 100, (void* const)&led_verde, 1, &Task1 );
	vTaskStartScheduler();

	for(;;);
}

void vTaskLedPeriodico( void *pvParameters) {
	Led_TypeDef* Led = (Led_TypeDef*) pvParameters;
	while(1) {
		BSP_LED_Toggle(*Led);
		vTaskDelay(xPeriodo);
	}
}

void vTaskLedUsuario( void *pvParameters) {
	Led_TypeDef* Led = (Led_TypeDef*) pvParameters;
	while(1) {
		if (BSP_PB_GetState(sw_usuario) == 1) {
			BSP_LED_On(*Led);
		} else {
			BSP_LED_Off(*Led);
		}
	}
}
