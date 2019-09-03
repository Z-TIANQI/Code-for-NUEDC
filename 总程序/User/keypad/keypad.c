#include "keypad.h"
#include "bsp_SysTick.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "OLED_I2C.h"
#include <stdio.h>
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
 
/*���ļ��Ǿ��󰴼������ļ���ʹ�÷����ǣ�ֱ�ӵ�KEYPAD_Scan�����õ�һ������ֵ*/
 
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
 
//��������KEYPad_Init1
//��������
//���ܣ��м���ʼ��
//����ֵ;��
void KEYPad_Init1(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//����ʱ��,ʹ��GPIOBʱ��
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//������� B0-B3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//�������� B4-B7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
 
//��������ɨ�躯��
//��ڲ�������
//����ֵ����ֵ
//���ܣ����󰴼�ɨ�裬����һ����ֵ
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
		Delay_ms(10);//����10ms
		if(readvalue != 0x0F00)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_8);
			GPIO_ResetBits(GPIOC, GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11);
			readvalue = GPIO_ReadInputData(GPIOC);//�ٴζ�ȡ״̬
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
			readvalue = GPIO_ReadInputData(GPIOC);//�ٴζ�ȡ״̬
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
			readvalue = GPIO_ReadInputData(GPIOC);//�ٴζ�ȡ״̬
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
			readvalue = GPIO_ReadInputData(GPIOC);//�ٴζ�ȡ״̬
			readvalue &= 0xFF00;
			switch(readvalue)
			{
				case 0x1800: keyvalue = 10; break;
				case 0x2800: keyvalue = 0;  break;
				case 0x4800: keyvalue = 11; break;
				case 0x8800: keyvalue = 15; break;
			}
			Delay_ms(5);

			while(1)//��ⰴ���ɿ�
			{
				readvalue = GPIO_ReadInputData(GPIOC);//�ٴζ�ȡ״̬
				readvalue &= 0xFF00;
				if(readvalue==0x0800)
				{
					Delay_ms(10);
					readvalue = GPIO_ReadInputData(GPIOC);//�ٴζ�ȡ״̬
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


void Mode1_Key(void)
{

		int key = 0;
		OLED_CLS();
		OLED_ShowStr(5,0,(unsigned char*)"Mode 1",2);


		/* 
		 *������� 
		 */

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


		/* 
		 *����Ƕ�
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
		OLED_ShowStr(0,5,(unsigned char*)CanShu2,2);  // �Ƕ���ʾ
			
		Delay_ms(100);

		key = KEYPAD_Scan();
		while (key==10 || key==11 || key==12 || key==13 || key==14 || key==15 || key==16)
		{
			key = KEYPAD_Scan();
		}
		An_Jian = key + An_Jian;
		sprintf( CanShu2, "Distence = %u", An_Jian );
		OLED_ShowStr(0,5,(unsigned char*)CanShu2,2);  // �Ƕ���ʾ
			
		Delay_ms(100);

		key = KEYPAD_Scan();
		while (key!=10)
		{
			key = KEYPAD_Scan();
		}            // ���ȷ��
		sprintf( CanShu2, "Distence = %u", An_Jian );
		OLED_ShowStr(0,5,(unsigned char*)CanShu2,2);  // �Ƕ���ʾ


			
		An_Jian=0;
}


