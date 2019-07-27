#include "stm32f4xx.h"
#include "stm32f411e_discovery.h"
#include "FreeRTOS.h"
#include "task.h"

void vTaskLedSetSuperiorBarrido( void *pvParameters );
void vTaskLedSetInferiorBarrido( void *pvParameters );
void vTaskSW( void *pvParameters );

const TickType_t xPeriodo = pdMS_TO_TICKS( 150 );

Led_TypeDef leds_superiores [4] = {LED3, LED5, LED6, LED4}; // 3:N - 5:R - 6:A - 4:V
Led_TypeDef leds_inferiores [8] = {LED0, LED1, LED2, LED7, LED8, LED9, LED10, LED11};
Button_TypeDef sw0 = SW0;
Button_TypeDef sw1 = SW1;
Button_TypeDef sw2 = SW2;
Button_TypeDef sw3 = SW3;
Button_TypeDef sw4 = SW4;
TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;

int main(void) {

	for (int i = 0 ; i < 4 ; i++)
		BSP_LED_Init(leds_superiores[i]);

	for (int i = 0 ; i < 8 ; i++)
		BSP_LED_Init(leds_inferiores[i]);

	BSP_PB_Init(sw0, BUTTON_MODE_GPIO_PULLDOWN);
	BSP_PB_Init(sw1, BUTTON_MODE_GPIO_PULLUP);
	BSP_PB_Init(sw2, BUTTON_MODE_GPIO_PULLUP);
	BSP_PB_Init(sw3, BUTTON_MODE_GPIO_PULLUP);
	BSP_PB_Init(sw4, BUTTON_MODE_GPIO_PULLUP);

//	xTaskCreate( vTaskLedSetSuperiorBarrido, "TaskLedSetSuperiorBarrido", 100, NULL, 1, &Task1 );
//	xTaskCreate( vTaskLedSetInferiorBarrido, "TaskLedSetInferiorBarrido", 100, NULL, 1, &Task2 );
	xTaskCreate( vTaskSW, "vTaskSW", 100, NULL, 1, &Task3 );
	vTaskStartScheduler();

	for(;;);
}

void vTaskLedSetSuperiorBarrido( void *pvParameters) {
	uint16_t contador = 0;
	while(1) {
		if (contador < 4) {
			BSP_LED_Toggle(leds_superiores[contador]);
			vTaskDelay(xPeriodo);
			BSP_LED_Toggle(leds_superiores[contador++]);
			BSP_LED_Toggle(leds_superiores[contador]);
		} else
			contador = 0;
		vTaskDelay(xPeriodo);
	}
}

void vTaskLedSetInferiorBarrido( void *pvParameters) {
	uint16_t contador = 0;
	while(1) {
		if (contador < 8)
			BSP_LED_Toggle(leds_inferiores[contador++]);
		else
			contador = 0;
		vTaskDelay(xPeriodo);
	}
}

void vTaskSW( void *pvParameters) {
	while(1) {
		if (BSP_PB_GetState(sw0) == 1) BSP_LED_On(leds_inferiores[0]);
		else BSP_LED_Off(leds_inferiores[0]);

		if (BSP_PB_GetState(sw1) == 0) BSP_LED_On(leds_inferiores[1]);
		else BSP_LED_Off(leds_inferiores[1]);

		if (BSP_PB_GetState(sw2) == 0) BSP_LED_On(leds_inferiores[2]);
		else BSP_LED_Off(leds_inferiores[2]);

		if (BSP_PB_GetState(sw3) == 0) BSP_LED_On(leds_inferiores[3]);
		else BSP_LED_Off(leds_inferiores[3]);

		if (BSP_PB_GetState(sw4) == 0) BSP_LED_On(leds_inferiores[4]);
		else BSP_LED_Off(leds_inferiores[4]);
	}
}

