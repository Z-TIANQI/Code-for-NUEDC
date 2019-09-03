#include "keypad.h"
#include "delay.h"
#include "OLED_I2C.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
 
/*本文件是矩阵按键功能文件，使用方法是：直接调KEYPAD_Scan函数得到一个返回值*/
 
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
 
//函数名：KEYPad_Init1
//参数：无
//功能：行检测初始化
//返回值;无
void KEYPad_Init1(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//开启时钟,使能GPIOB时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出 B0-B3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//下拉输入 B4-B7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
 
//函数名：扫描函数
//入口参数：无
//返回值：键值
//功能：矩阵按键扫描，返回一个键值
uint16_t KEYPAD_Scan(void)
{
	u16 keyvalue = 16;
	u32 readvalue = 0;
	KEYPad_Init1();
	readvalue = GPIO_ReadInputData(GPIOC);
	readvalue &= 0xFF00;
	//return readvalue;
	if(readvalue != 0x0F00)
	{
		Delay_ms(10);//消抖10ms
		if(readvalue != 0x0F00)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_8);
			GPIO_ResetBits(GPIOC, GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11);
			readvalue = GPIO_ReadInputData(GPIOC);//再次读取状态
			readvalue &= 0xFF00;
			switch(readvalue)
			{
				case 0x1100: keyvalue = 1; break;
				case 0x2100: keyvalue = 2; break;
				case 0x4100: keyvalue = 3; break;
				case 0x8100: keyvalue = 12;break;
			}
			Delay_ms(5);

			GPIO_SetBits(GPIOC, GPIO_Pin_9);
			GPIO_ResetBits(GPIOC, GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_11);
			readvalue = GPIO_ReadInputData(GPIOC);//再次读取状态
			readvalue &= 0xFF00;
			switch(readvalue)
			{
				case 0x1200: keyvalue = 4; break;
				case 0x2200: keyvalue = 5; break;
				case 0x4200: keyvalue = 6; break;
				case 0x8200: keyvalue = 13;break;
			}
			Delay_ms(5);

			GPIO_SetBits(GPIOC, GPIO_Pin_10);
			GPIO_ResetBits(GPIOC, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_11);
			readvalue = GPIO_ReadInputData(GPIOC);//再次读取状态
			readvalue &= 0xFF00;
			switch(readvalue)
			{
				case 0x1400: keyvalue = 7; break;
				case 0x2400: keyvalue = 8; break;
				case 0x4400: keyvalue = 9; break;
				case 0x8400: keyvalue = 14;break;
			}
			Delay_ms(5);

			GPIO_SetBits(GPIOC, GPIO_Pin_11);
			GPIO_ResetBits(GPIOC, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
			readvalue = GPIO_ReadInputData(GPIOC);//再次读取状态
			readvalue &= 0xFF00;
			switch(readvalue)
			{
				case 0x1800: keyvalue = 10; break;
				case 0x2800: keyvalue = 0;  break;
				case 0x4800: keyvalue = 11; break;
				case 0x8800: keyvalue = 15; break;
			}
			Delay_ms(5);

			while(1)//检测按键松开
			{
				readvalue = GPIO_ReadInputData(GPIOC);//再次读取状态
				readvalue &= 0xFF00;
				if(readvalue==0x0800)
				{
					Delay_ms(10);
					readvalue = GPIO_ReadInputData(GPIOC);//再次读取状态
					readvalue &= 0xFF00;
					if(readvalue==0x0800)
						break;
				}
			}
			//return readvalue;
		}
	}
	return keyvalue;
}


char CanShu1[3];
char CanShu2[3];
extern uint16_t An_Jian;
extern u8 recv1_data_ok; 
extern u8 control_data[200];


void Mode1_Key(void)
{

		int key = 0;
		OLED_CLS();
		OLED_ShowStr(5,0,(unsigned char*)"Mode 1",2);


		/* 
		 *发射距离 
		 */

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


		/* 
		 *发射角度
		 */

		Delay_ms(100);

		An_Jian = 0;

		key = KEYPAD_Scan();
		while (key==10 || key==11 || key==12 || key==13 || key==14 || key==15 || key==16)
		{
			key = KEYPAD_Scan();
		}
		An_Jian = key * 10 + An_Jian;
		sprintf( CanShu2, "Distence = %u", An_Jian );
		OLED_ShowStr(0,5,(unsigned char*)CanShu2,2);  // 角度显示
			
		Delay_ms(100);

		key = KEYPAD_Scan();
		while (key==10 || key==11 || key==12 || key==13 || key==14 || key==15 || key==16)
		{
			key = KEYPAD_Scan();
		}
		An_Jian = key + An_Jian;
		sprintf( CanShu2, "Distence = %u", An_Jian );
		OLED_ShowStr(0,5,(unsigned char*)CanShu2,2);  // 角度显示
			
		Delay_ms(100);

		key = KEYPAD_Scan();
		while (key!=10)
		{
			key = KEYPAD_Scan();
		}            // 点击确定
		sprintf( CanShu2, "Distence = %u", An_Jian );
		OLED_ShowStr(0,5,(unsigned char*)CanShu2,2);  // 角度显示
			
		An_Jian=0;
}

