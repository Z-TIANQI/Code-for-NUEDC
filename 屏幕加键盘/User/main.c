/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   OLED测试（软件模拟I2C）
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
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
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    char CanShu1[3];
    char CanShu2[3];
    uint16_t An_Jian=0;
	
	USART_Config();        /* 串口初始化 */
	
	SysTick_Init();        /*初始化延迟函数*/
	i2c_CfgGpio();				 /*I2C总线的GPIO初始化*/
	OLED_Init();					 /* OLED初始化 */

    OLED_Fill(0xFF);//全屏点亮
	Delay_s(2);
		
	OLED_Fill(0x00);//全屏灭

	while(1)
	{
		if(KEYPAD_Scan()==16)
		{
			An_Jian = An_Jian;
		}
		if(KEYPAD_Scan()==12) // 模式1
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
			OLED_ShowStr(0,3,(unsigned char*)CanShu1,2);  // 距离显示
			
			Delay_ms(100);

			key = KEYPAD_Scan();
			while (key==10 || key==11 || key==12 || key==13 || key==14 || key==15 || key==16)
			{
				key = KEYPAD_Scan();
			}
			An_Jian = key * 10 + An_Jian;
			sprintf( CanShu1, "Distence = %u", An_Jian );
			OLED_ShowStr(0,3,(unsigned char*)CanShu1,2);  // 距离显示
			
			Delay_ms(100);
			
			key = KEYPAD_Scan();
			while (key==10 || key==11 || key==12 || key==13 || key==14 || key==15 || key==16)
			{
				key = KEYPAD_Scan();
			}
			An_Jian = key + An_Jian;
			sprintf( CanShu1, "Distence = %u", An_Jian );
			OLED_ShowStr(0,3,(unsigned char*)CanShu1,2);  // 距离显示
			
			Delay_ms(100);
			
			key = KEYPAD_Scan();
			while (key!=10)
			{
				key = KEYPAD_Scan();
			}            // 点击确定
			sprintf( CanShu1, "Distence = %u", An_Jian );
			OLED_ShowStr(0,3,(unsigned char*)CanShu1,2);  // 距离显示
			
			Delay_ms(100);

			An_Jian = 0;

			key = KEYPAD_Scan();
			while (key==10 || key==11 || key==12 || key==13 || key==14 || key==15 || key==16)
			{
				key = KEYPAD_Scan();
			}
			An_Jian = key * 10 + An_Jian;
			sprintf( CanShu2, "Distence = %u", An_Jian );
			OLED_ShowStr(0,5,(unsigned char*)CanShu2,2);  // 距离显示
			
			Delay_ms(100);

			key = KEYPAD_Scan();
			while (key==10 || key==11 || key==12 || key==13 || key==14 || key==15 || key==16)
			{
				key = KEYPAD_Scan();
			}
			An_Jian = key + An_Jian;
			sprintf( CanShu2, "Distence = %u", An_Jian );
			OLED_ShowStr(0,5,(unsigned char*)CanShu2,2);  // 距离显示
			
			Delay_ms(100);

			key = KEYPAD_Scan();
			while (key!=10)
			{
				key = KEYPAD_Scan();
			}            // 点击确定
			sprintf( CanShu2, "Distence = %u", An_Jian );
			OLED_ShowStr(0,5,(unsigned char*)CanShu2,2);  // 距离显示
			
			An_Jian=0;

			// 串口输出
		}
		if(KEYPAD_Scan()==13)
		{
			OLED_CLS();
			OLED_ShowStr(5,0,(unsigned char*)"Mode 2",2);
			// 串口输出
		}
		if(KEYPAD_Scan()==14)
		{
			OLED_CLS();
			OLED_ShowStr(5,0,(unsigned char*)"Mode 3",2);
			//
		}
		//Delay_ms(10);
		//OLED_CLS();//清屏
	
		
	}
}
/*********************************************END OF FILE**********************/
