#include "stm32f4xx.h"
#include "stm32f411e_discovery.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stdbool.h"

void vTaskLedSetSuperior(void *pvParameters);
void vTaskLedSetInferior(void *pvParameters);
void vTaskLectorSW_Superior(void *pvParameters);
void vTaskLectorSW_Inferior_IZQ(void *pvParameters);
void vTaskLectorSW_Inferior_DER(void *pvParameters);

const TickType_t xPeriodo = pdMS_TO_TICKS(100);

Led_TypeDef leds_superiores [4] = {LED3, LED5, LED6, LED4}; // 3:N - 5:R - 6:A - 4:V
Led_TypeDef leds_inferiores [8] = {LED0, LED1, LED2, LED7, LED8, LED9, LED10, LED11};
Button_TypeDef sw [5] = {SW0, SW1, SW2, SW3, SW4};
QueueHandle_t xQueue;
QueueHandle_t xMailbox;
bool destellar = false;

typedef enum {
	eSW_Izquierdo,
	eSW_Derecho
} DataSource_t;

typedef struct {
	uint16_t usValor;
	DataSource_t eDataSource;
} Data_t;

int main(void) {
	for (int i = 0 ; i < 4 ; i++)
		BSP_LED_Init(leds_superiores[i]);

	for (int i = 0 ; i < 8 ; i++)
		BSP_LED_Init(leds_inferiores[i]);

	for (int i = 0 ; i < 5 ;  i++) {
		if (i == 0)
			BSP_PB_Init(sw[i], BUTTON_MODE_GPIO_PULLDOWN);
		else
			BSP_PB_Init(sw[i], BUTTON_MODE_GPIO_PULLUP);
	}

	xQueue = xQueueCreate(4, sizeof(Data_t));
	xMailbox = xQueueCreate(1, sizeof(destellar));

	xTaskCreate(vTaskLedSetSuperior, "TaskLedSetSuperior1", 100, (void* const)&leds_superiores[0], 1, NULL);
	xTaskCreate(vTaskLedSetSuperior, "TaskLedSetSuperior2", 100, (void* const)&leds_superiores[1], 1, NULL);
	xTaskCreate(vTaskLedSetSuperior, "TaskLedSetSuperior3", 100, (void* const)&leds_superiores[2], 1, NULL);
	xTaskCreate(vTaskLedSetSuperior, "TaskLedSetSuperior4", 100, (void* const)&leds_superiores[3], 1, NULL);
	xTaskCreate(vTaskLedSetInferior, "TaskLedSetInferior", 100, NULL, 2, NULL);
	xTaskCreate(vTaskLectorSW_Inferior_IZQ, "TaskLectorSW_inf_izq", 100, NULL, 1, NULL);
	xTaskCreate(vTaskLectorSW_Inferior_DER, "TaskLectorSW_inf_der", 100, NULL, 1, NULL);
	xTaskCreate(vTaskLectorSW_Superior, "TaskLectorSW_sup", 100, NULL, 1, NULL);
	vTaskStartScheduler();

	for(;;);
}

void vTaskLedSetInferior(void *pvParameters) {
	Data_t xDatoRecibido;
	BaseType_t xEstadoRecepcion;
	while(1) {
		xEstadoRecepcion = xQueueReceive(xQueue, &xDatoRecibido, 100);
		if (xEstadoRecepcion == pdPASS) {
			if(xDatoRecibido.eDataSource == eSW_Izquierdo)
				BSP_LED_Toggle(leds_inferiores[xDatoRecibido.usValor]);
			else if (xDatoRecibido.eDataSource == eSW_Derecho)
				BSP_LED_Toggle(leds_inferiores[xDatoRecibido.usValor + 4]);
		}
	}
}

void vTaskLedSetSuperior(void *pvParameters) {
	Led_TypeDef* led = (Led_TypeDef*) pvParameters;
	bool xDatoRecibido;
	while(1) {
		xQueuePeek(xMailbox, &xDatoRecibido, portMAX_DELAY);

		if(xDatoRecibido)
			BSP_LED_On(*led);
		else
			BSP_LED_Off(*led);
	}
}

void vTaskLectorSW_Inferior_IZQ(void *pvParameters) {
	Data_t datoSW_Izquierdo = {0, eSW_Izquierdo};
	while(1) {
		if (BSP_PB_GetState(sw[3]) == 0) {
			xQueueSendToBack(xQueue, &datoSW_Izquierdo, 0);
			if (datoSW_Izquierdo.usValor < 3)
				datoSW_Izquierdo.usValor++;
			else
				datoSW_Izquierdo.usValor = 0;
		}
		vTaskDelay(xPeriodo);
	}
}

void vTaskLectorSW_Inferior_DER(void *pvParameters) {
	Data_t datoSW_Derecho = {0, eSW_Derecho};
	while(1) {
		if (BSP_PB_GetState(sw[4]) == 0) {
			xQueueSendToBack(xQueue, &datoSW_Derecho, 0);
			if (datoSW_Derecho.usValor < 3)
				datoSW_Derecho.usValor++;
			else
				datoSW_Derecho.usValor = 0;
		}
		vTaskDelay(xPeriodo);
	}
}

void vTaskLectorSW_Superior(void *pvParameters) {
	while(1) {
		if (BSP_PB_GetState(sw[0]) == 1) {
			destellar = !destellar;
			xQueueOverwrite(xMailbox, &destellar);
		}
		vTaskDelay(xPeriodo);
	}
}

