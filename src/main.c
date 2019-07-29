#include "stm32f4xx.h"
#include "stm32f411e_discovery.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdbool.h>

void vTaskIniciador(void *pvParameters);
void vTaskLed( void *pvParameters );
void vTaskSW( void *pvParameters );

const TickType_t xPeriodo = pdMS_TO_TICKS( 1000 );

Led_TypeDef leds_superiores [4] = {LED3, LED5, LED6, LED4}; // 3:N - 5:R - 6:R - 4:V
Led_TypeDef leds_inferiores [8] = {LED0, LED1, LED2, LED7, LED8, LED9, LED10, LED11};
Button_TypeDef sw [5] = {SW0, SW1, SW2, SW3, SW4};
TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;

int main(void) {

	for (int i = 0 ; i < 4 ; i++)
		BSP_LED_Init(leds_superiores[i]);

	for (int i = 0 ; i < 8 ; i++)
		BSP_LED_Init(leds_inferiores[i]);

	for (int i = 0 ; i < 5 ;  i++) {
		if (i == 0)
			BSP_PB_Init(sw[i], BUTTON_MODE_GPIO_PULLDOWN);
		else {
			BSP_PB_Init(sw[i], BUTTON_MODE_GPIO_PULLUP);
		}
	}

	xTaskCreate( vTaskIniciador, "TaskIniciador", 100, NULL, 5, NULL);
//	xTaskCreate( vTaskSW, "vTaskSW", 100, NULL, 1, &Task3 );
	vTaskStartScheduler();

	for(;;);
}

void vTaskIniciador(void *pvParameters) {
	xTaskCreate( vTaskLed, "TaskLedVerde", 100, (void* const)&leds_superiores[3], 0, NULL);
	vTaskDelay(xPeriodo);
	xTaskCreate( vTaskLed, "TaskLedNaranja", 100, (void* const)&leds_superiores[0], 1, NULL);
	vTaskDelay(xPeriodo);
	xTaskCreate( vTaskLed, "TaskLedRojo", 100, (void* const)&leds_superiores[1], 2, NULL);
	vTaskDelay(xPeriodo);
	xTaskCreate( vTaskLed, "TaskLedAzul", 100, (void* const)&leds_superiores[2], 3, NULL);
	vTaskDelay(xPeriodo);
	vTaskDelete(NULL);
}

void vTaskLed( void *pvParameters) {
	Led_TypeDef* led = (Led_TypeDef*) pvParameters;
	uint16_t contador = 0;
	while(1) {
		while (contador < 40) {
			BSP_LED_Toggle(*led);
			HAL_Delay(50);
			contador++;
		}
		contador = 0;
		vTaskDelay(xPeriodo * 8);
	}
}

void vApplicationIdleHook( void ) {
	uint16_t i = 0;
	bool ascender;
	while(1) {
		if (i == 0)
			ascender = true;
		else if (i == 7)
			ascender = false;

		BSP_LED_On(leds_inferiores[i]);
		HAL_Delay(25);
		BSP_LED_Off(leds_inferiores[i]);

		if (ascender) i++;
		else i--;
	}
}

void vTaskSW( void *pvParameters) {
	while(1) {
		if (BSP_PB_GetState(sw[0]) == 1) BSP_LED_On(leds_inferiores[0]);
		else BSP_LED_Off(leds_inferiores[0]);

		if (BSP_PB_GetState(sw[1]) == 0) BSP_LED_On(leds_inferiores[1]);
		else BSP_LED_Off(leds_inferiores[1]);

		if (BSP_PB_GetState(sw[2]) == 0) BSP_LED_On(leds_inferiores[2]);
		else BSP_LED_Off(leds_inferiores[2]);

		if (BSP_PB_GetState(sw[3]) == 0) BSP_LED_On(leds_inferiores[3]);
		else BSP_LED_Off(leds_inferiores[3]);

		if (BSP_PB_GetState(sw[4]) == 0) BSP_LED_On(leds_inferiores[4]);
		else BSP_LED_Off(leds_inferiores[4]);
	}
}

