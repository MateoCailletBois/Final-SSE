#ifndef BSP_BSP_H_
#define BSP_BSP_H_

#include "stdint.h"
#define LEDS_PORT GPIOD

typedef enum{LED0_BOAR=4, LED1_BOAR=5, LED2_BOAR=6, LED3_BOAR=7, LED6_BOAR=8,LED7_BOAR=9,LED10_BOAR=10,LED11_BOAR=11
}LED_typedef;

#define EXP_BOARD_POT_PIN				   GPIO_PIN_2
#define EXP_BOARD_POT_PORT                   GPIOC
#define EXP_BOARD_POT_PIN_CLK_ENABLE()           __GPIOC_CLK_ENABLE()
#define EXP_BOARD_POT_ADC_CLK_ENABLE()		__ADC1_CLK_ENABLE()
#define EXP_BOARD_POT_CLK_DISABLE()          __GPIOD_CLK_DISABLE()
#define EXP_BOARD_POT_CHANNEL				   ADC_CHANNEL_12

void led_on(uint8_t led);
void led_off(uint8_t led);
void BSP_Init(void);
void BSP_LED(void);

#endif /* BSP_BSP_H_ */
