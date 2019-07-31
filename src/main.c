#include "stm32f4xx.h"
#include "stm32f411e_discovery.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void vTaskLedVerde( void *pvParameters );
void vTaskLedNaranja( void *pvParameters );
void vTaskLedRojo( void *pvParameters );
void vTaskLedAzul( void *pvParameters );
void vTaskSW_Superior( void *pvParameters );
void vTaskSW_Inferior( void *pvParameters );

const TickType_t xPeriodo = pdMS_TO_TICKS( 150 );

Led_TypeDef leds_superiores [4] = {LED3, LED5, LED6, LED4}; // 3:N - 5:R - 6:A - 4:V
Led_TypeDef leds_inferiores [8] = {LED0, LED1, LED2, LED7, LED8, LED9, LED10, LED11};
Button_TypeDef sw [5] = {SW0, SW1, SW2, SW3, SW4};
SemaphoreHandle_t xBinarySemaphore1;
SemaphoreHandle_t xBinarySemaphore2;
SemaphoreHandle_t xBinarySemaphore3;
SemaphoreHandle_t xBinarySemaphore4;
SemaphoreHandle_t xMutex;

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

	xBinarySemaphore1 = xSemaphoreCreateBinary();
	xBinarySemaphore2 = xSemaphoreCreateBinary();
	xBinarySemaphore3 = xSemaphoreCreateBinary();
	xBinarySemaphore4 = xSemaphoreCreateBinary();
	xMutex = xSemaphoreCreateMutex();

	xTaskCreate(vTaskLedVerde, "vTaskLedVerde", 100, NULL, 1, NULL);
	xTaskCreate(vTaskLedNaranja, "vTaskLedNaranja", 100, NULL, 1, NULL);
	xTaskCreate(vTaskLedRojo, "vTaskLedRojo", 100, NULL, 1, NULL);
	xTaskCreate(vTaskLedAzul, "vTaskLedAzul", 100, NULL, 1, NULL);
	xTaskCreate(vTaskSW_Superior, "vTaskSW_Superior", 100, (void* const)&sw[0], 1, NULL);
	xTaskCreate(vTaskSW_Inferior, "vTaskSW_izq", 100, (void* const)&sw[3], 1, NULL);
	xTaskCreate(vTaskSW_Inferior, "vTaskSW_der", 100, (void* const)&sw[4], 1, NULL);
	vTaskStartScheduler();

	for(;;);
}

void vTaskLedVerde(void *pvParameters) {
	while(1) {
		xSemaphoreTake(xBinarySemaphore1, portMAX_DELAY);
		BSP_LED_On(leds_superiores[3]);
		vTaskDelay(xPeriodo * 15);
		BSP_LED_Off(leds_superiores[3]);
		xSemaphoreGive(xBinarySemaphore2);
	}
}

void vTaskLedNaranja(void *pvParameters) {
	while(1) {
		xSemaphoreTake(xBinarySemaphore2, portMAX_DELAY);
		BSP_LED_On(leds_superiores[0]);
		vTaskDelay(xPeriodo * 15);
		BSP_LED_Off(leds_superiores[0]);
		xSemaphoreGive(xBinarySemaphore3);
	}
}

void vTaskLedRojo(void *pvParameters) {
	while(1) {
		xSemaphoreTake(xBinarySemaphore3, portMAX_DELAY);
		BSP_LED_On(leds_superiores[1]);
		vTaskDelay(xPeriodo * 15);
		BSP_LED_Off(leds_superiores[1]);
		xSemaphoreGive(xBinarySemaphore4);
	}
}

void vTaskLedAzul(void *pvParameters) {
	while(1) {
		xSemaphoreTake(xBinarySemaphore4, portMAX_DELAY);
		BSP_LED_On(leds_superiores[2]);
		vTaskDelay(xPeriodo * 15);
		BSP_LED_Off(leds_superiores[2]);
	}
}

void vTaskSW_Superior(void *pvParameters) {
	Button_TypeDef* sw = (Button_TypeDef*) pvParameters;
	while(1) {
		if (BSP_PB_GetState(*sw) == 1)
			xSemaphoreGive(xBinarySemaphore1);
		vTaskDelay(xPeriodo);
	}
}

void vTaskSW_Inferior(void *pvParameters) {
	Button_TypeDef* sw = (Button_TypeDef*) pvParameters;
	while(1) {
		if (BSP_PB_GetState(*sw) == 0) {
			xSemaphoreTake(xMutex, portMAX_DELAY);
			for (int i = 0 ; i < 8 ; i++) {
				BSP_LED_On(leds_inferiores[i]);
				vTaskDelay(xPeriodo);
				BSP_LED_Off(leds_inferiores[i]);
			}
			xSemaphoreGive(xMutex);
		}
	}
}
