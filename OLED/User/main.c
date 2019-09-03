/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   OLED���ԣ����ģ��I2C��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-MINI STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "bsp_i2c_gpio.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h" 
#include "OLED_I2C.h"
#include "./systick/bsp_SysTick.h"


extern const unsigned char BMP1[];

 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    unsigned char i; 
	
		uint8_t huhu = 123;
		char shu[3];
		sprintf(shu,"%u",huhu);
	
    /*��ʼ��LED������*/	
	  LED_GPIO_Config();     /* LED��ʼ�� */
		USART_Config();        /* ���ڳ�ʼ�� */
	
	  SysTick_Init();        /*��ʼ���ӳٺ���*/
		i2c_CfgGpio();				 /*I2C���ߵ�GPIO��ʼ��*/
	  OLED_Init();					 /* OLED��ʼ�� */

	printf("\r\n ��ӭʹ��Ұ�� F103-MINI STM32 ������\r\n");		
	printf("\r\n OLED���ģ��i2c�������� \r\n");		
	
	  /*OLED������*/
	  if(OLED_Test()==0)
		{          
			/* û�м�⵽EEPROM */
			printf("\r\n û�м�⵽OLED��\r\n");
			LED1_ON;//D4��	
		}
		else
		{
			printf("\r\n OLED���ɹ�,OLED���ڹ�����\r\n");
			LED2_ON;//D5��
		}
		
		while(1)
	{
		OLED_Fill(0xFF);//ȫ������
		Delay_s(2);
		
		OLED_Fill(0x00);//ȫ����
		Delay_s(2);
		
		for(i=0;i<4;i++)
		{
			OLED_ShowCN(22+i*16,0,i);									//������ʾ����
		}
		Delay_s(2);
		OLED_ShowStr(0,3,(unsigned char*)"Wildfire Tech",1);				//����6*8�ַ�
		OLED_ShowStr(0,4,(unsigned char*)shu,2);				//����8*16�ַ�
		Delay_s(2);
		OLED_CLS();//����
		OLED_OFF();//����OLED����
		Delay_s(2);
		OLED_ON();//����OLED���ߺ���
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
		Delay_s(20);
	}
}
/*********************************************END OF FILE**********************/
