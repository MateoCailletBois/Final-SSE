#include "stm32f4xx.h"
#include "stm32f411e_discovery.h"
#include "FreeRTOS.h"
#include "task.h"


void vTaskLedPeriodico( void *pvParameters );
void vTaskPatron1( void *pvParameters );
void vTaskLeerBoton( void *pvParameters );

const TickType_t xPeriodo = pdMS_TO_TICKS( 1000 );
Led_TypeDef led_naranja = LED3;
Led_TypeDef led_verde = LED4;
Led_TypeDef led_rojo = LED5;
Led_TypeDef led_azul = LED6;
Button_TypeDef sw_usuario = BUTTON_KEY;
TaskHandle_t Task1;
TaskHandle_t Task2;

int main(void) {
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);
	BSP_PB_Init(sw_usuario, BUTTON_MODE_GPIO);

	//xTaskCreate( vTaskLedPeriodico, "TaskLedPeriodico", 100, (void* const)&led_naranja, 1, &Task1 );
	xTaskCreate( vTaskPatron1, "TaskLedBarrido", 100, NULL, 1, &Task1 );
	xTaskCreate( vTaskLeerBoton, "TaskLeerBoton", 100, NULL, 1, &Task2 );
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

void vTaskPatron1( void *pvParameters) {
	while(1) {
		BSP_LED_On(led_naranja);
		BSP_LED_On(led_azul);
		BSP_LED_On(led_rojo);
		BSP_LED_On(led_verde);
		vTaskDelay(xPeriodo);
		BSP_LED_Off(led_naranja);
		BSP_LED_Off(led_azul);
		BSP_LED_Off(led_rojo);
		BSP_LED_Off(led_verde);
		vTaskDelay(xPeriodo);
	}
}

void vTaskLeerBoton( void *pvParameters) {
	Led_TypeDef* Led = (Led_TypeDef*) pvParameters;
	while(1) {
		if (BSP_PB_GetState(sw_usuario) == 1) {
			BSP_LED_On(*Led);
		} else {
			BSP_LED_Off(*Led);
		}
	}
}
