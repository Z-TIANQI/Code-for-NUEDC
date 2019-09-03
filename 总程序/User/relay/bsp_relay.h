#ifndef __RELAY_H
#define	__RELAY_H


#include "stm32f10x.h"
#include "stm32f10x_gpio.h"


#define RELAY1_GPIO_PORT    	GPIOB			              
#define RELAY1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		
#define RELAY1_GPIO_PIN		    GPIO_Pin_14	      

#define RELAY2_GPIO_PORT    	GPIOB			              
#define RELAY2_GPIO_CLK 	    RCC_APB2Periph_GPIOB		
#define RELAY2_GPIO_PIN		    GPIO_Pin_15	 

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define RELAY1_TOGGLE		 digitalToggle(RELAY1_GPIO_PORT,RELAY1_GPIO_PIN)
#define RELAY1_ON		     digitalLo(RELAY1_GPIO_PORT,RELAY1_GPIO_PIN)
#define RELAY1_OFF			 digitalHi(RELAY1_GPIO_PORT,RELAY1_GPIO_PIN)

#define RELAY2_TOGGLE		 digitalToggle(RELAY2_GPIO_PORT,RELAY2_GPIO_PIN)
#define RELAY2_ON		     digitalLo(RELAY2_GPIO_PORT,RELAY2_GPIO_PIN)
#define RELAY2_OFF			 digitalHi(RELAY2_GPIO_PORT,RELAY2_GPIO_PIN)

void RELAY_GPIO_Config(void);

#endif /* __RELAY_H */
