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
#include "bsp_TiMbase.h"
#include "keypad.h"



extern const unsigned char BMP1[];

 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    char CanShu1[3];
    char CanShu2[3];
    uint16_t An_Jian=0;
	
	USART_Config();        /* ���ڳ�ʼ�� */
	
	SysTick_Init();        /*��ʼ���ӳٺ���*/
	i2c_CfgGpio();				 /*I2C���ߵ�GPIO��ʼ��*/
	OLED_Init();					 /* OLED��ʼ�� */

    OLED_Fill(0xFF);//ȫ������
	Delay_s(2);
		
	OLED_Fill(0x00);//ȫ����

	while(1)
	{
		if(KEYPAD_Scan()==16)
		{
			An_Jian = An_Jian;
		}
		if(KEYPAD_Scan()==12) // ģʽ1
		{
			int key = 0;
			OLED_CLS();
			OLED_ShowStr(5,0,(unsigned char*)"Mode 1",2);

			key = KEYPAD_Scan();
			while (key==10 || key==11 || key==12 || key==13 || key==14 || key==15 || key==16)
			{
				key = KEYPAD_Scan();
			}
			An_Jian = key * 100;
			sprintf( CanShu1, "Distence = %u", An_Jian );
			OLED_ShowStr(0,3,(unsigned char*)CanShu1,2);  // ������ʾ
			
			Delay_ms(100);

			key = KEYPAD_Scan();
			while (key==10 || key==11 || key==12 || key==13 || key==14 || key==15 || key==16)
			{
				key = KEYPAD_Scan();
			}
			An_Jian = key * 10 + An_Jian;
			sprintf( CanShu1, "Distence = %u", An_Jian );
			OLED_ShowStr(0,3,(unsigned char*)CanShu1,2);  // ������ʾ
			
			Delay_ms(100);
			
			key = KEYPAD_Scan();
			while (key==10 || key==11 || key==12 || key==13 || key==14 || key==15 || key==16)
			{
				key = KEYPAD_Scan();
			}
			An_Jian = key + An_Jian;
			sprintf( CanShu1, "Distence = %u", An_Jian );
			OLED_ShowStr(0,3,(unsigned char*)CanShu1,2);  // ������ʾ
			
			Delay_ms(100);
			
			key = KEYPAD_Scan();
			while (key!=10)
			{
				key = KEYPAD_Scan();
			}            // ���ȷ��
			sprintf( CanShu1, "Distence = %u", An_Jian );
			OLED_ShowStr(0,3,(unsigned char*)CanShu1,2);  // ������ʾ
			
			Delay_ms(100);

			An_Jian = 0;

			key = KEYPAD_Scan();
			while (key==10 || key==11 || key==12 || key==13 || key==14 || key==15 || key==16)
			{
				key = KEYPAD_Scan();
			}
			An_Jian = key * 10 + An_Jian;
			sprintf( CanShu2, "Distence = %u", An_Jian );
			OLED_ShowStr(0,5,(unsigned char*)CanShu2,2);  // ������ʾ
			
			Delay_ms(100);

			key = KEYPAD_Scan();
			while (key==10 || key==11 || key==12 || key==13 || key==14 || key==15 || key==16)
			{
				key = KEYPAD_Scan();
			}
			An_Jian = key + An_Jian;
			sprintf( CanShu2, "Distence = %u", An_Jian );
			OLED_ShowStr(0,5,(unsigned char*)CanShu2,2);  // ������ʾ
			
			Delay_ms(100);

			key = KEYPAD_Scan();
			while (key!=10)
			{
				key = KEYPAD_Scan();
			}            // ���ȷ��
			sprintf( CanShu2, "Distence = %u", An_Jian );
			OLED_ShowStr(0,5,(unsigned char*)CanShu2,2);  // ������ʾ
			
			An_Jian=0;

			// �������
		}
		if(KEYPAD_Scan()==13)
		{
			OLED_CLS();
			OLED_ShowStr(5,0,(unsigned char*)"Mode 2",2);
			// �������
		}
		if(KEYPAD_Scan()==14)
		{
			OLED_CLS();
			OLED_ShowStr(5,0,(unsigned char*)"Mode 3",2);
			//
		}
		//Delay_ms(10);
		//OLED_CLS();//����
	
		
	}
}
/*********************************************END OF FILE**********************/
