/**
  ******************************************************************************
  * @attention
  * 
  * control_data[] = {0xAA,0x55,0xXX,0xXX, , ,0xXX,0x00}
  *
  * openmv���ص����ݰ��У�����λ��������ʲô���ݣ���control_data[2]��
  * 0x11 : openmv ���յ�����ģʽ1��ָ��
  * 0x12 : openmv ���յ���stm32���͵ľ�����Ϣ
  * 0x13 : openmv ���յ���stm32���͵ĽǶ���Ϣ
  * 0x14 : openmv �Ѿ�������̨ת��Ԥ��λ��,���Գ�磬����\
  * 
  * 0x21 : openmv ���յ��˿���ģʽ2��ָ��
  * 0x22 : openmv �Ѿ�������̨ת��Ԥ��λ��,���Գ�磬����
  * 
  * 0x31 : openmv ���յ��˿���ģʽ3��ָ��
  * 0x32 : openmv �Ѿ�������̨ת��Ԥ��λ��,���Գ�磬����
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
#include "bsp_relay.h"

uint16_t An_Jian=0;


	
 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	int i = 1;
	int j = 1;
	USART_Config();              // ���ڳ�ʼ��
//	SysTick_Init();              // ��ʼ���ӳٺ���
//	i2c_CfgGpio();				 // I2C���ߵ�GPIO��ʼ��
//	OLED_Init();				 // OLED��ʼ��
	RELAY_GPIO_Config();         // �̵���GPIO��ʼ��
//	BASIC_TIM_Init();
	

//	
//	RELAY1_ON;       // ���̵���
//	
//	for(i = 2000 ; i >0 ; i--)
//	{
//		for(j = 2000 ; j >0 ; j--)
//		{
//			;
//		}
//	}
//	
//	RELAY1_OFF;
//			
//		for(i = 5000 ; i >0 ; i--)
//	{
//		for(j = 5000 ; j >0 ; j--)
//		{
//			;
//		}
//	}

//	RELAY2_ON;       // ���ڼ̵���
//	
//		for(i = 2000 ; i >0 ; i--)
//	{
//		for(j = 2000 ; j >0 ; j--)
//		{
//			;
//		}
//	}
//	
//	RELAY2_OFF; 

	while(1)
	{
		if(KEYPAD_Scan()==16)    // ģʽ0 �޶���
		{
			An_Jian = An_Jian;
		}
		if(KEYPAD_Scan()==1)    // ģʽ1 OLED ��ʾ��������ľ�����Ƕȣ������͸�openmv��openmv�յ��źź������̵���
		{
					RELAY1_ON;       // ���̵���
					Delay_s(1);
					RELAY1_OFF;
			
					Delay_s(3);

					RELAY2_ON;       // ���ڼ̵���
					Delay_ms(500);
					RELAY2_OFF; 
			break;

		}
		
	}
	
}
///*********************************************END OF FILE**********************/
