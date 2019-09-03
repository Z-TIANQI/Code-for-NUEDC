
#include "./relay/bsp_relay.h"   
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void RELAY_GPIO_Config(void)
{		

		GPIO_InitTypeDef GPIO_InitStructure;

	
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = RELAY1_GPIO_PIN;		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(RELAY1_GPIO_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = RELAY2_GPIO_PIN;		
		GPIO_Init(RELAY2_GPIO_PORT, &GPIO_InitStructure);

		GPIO_ResetBits(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN);
		GPIO_ResetBits(RELAY2_GPIO_PORT, RELAY2_GPIO_PIN);
}


