/**
  ******************************************************************************
  * @attention
  * 
  * control_data[] = {0xAA,0x55,0xXX,0xXX, , ,0xXX,0x00}
  *
  * openmv发回的数据包中，第三位代表了是什么数据：（control_data[2]）
  * 0x11 : openmv 接收到开启模式1的指令
  * 0x12 : openmv 接收到了stm32发送的距离信息
  * 0x13 : openmv 接收到了stm32发送的角度信息
  * 0x14 : openmv 已经驱动云台转到预定位置,可以充电，开炮\
  * 
  * 0x21 : openmv 接收到了开启模式2的指令
  * 0x22 : openmv 已经驱动云台转到预定位置,可以充电，开炮
  * 
  * 0x31 : openmv 接收到了开启模式3的指令
  * 0x32 : openmv 已经驱动云台转到预定位置,可以充电，开炮
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
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	int i = 1;
	int j = 1;
	USART_Config();              // 串口初始化
//	SysTick_Init();              // 初始化延迟函数
//	i2c_CfgGpio();				 // I2C总线的GPIO初始化
//	OLED_Init();				 // OLED初始化
	RELAY_GPIO_Config();         // 继电器GPIO初始化
//	BASIC_TIM_Init();
	

//	
//	RELAY1_ON;       // 充电继电器
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

//	RELAY2_ON;       // 开炮继电器
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
		if(KEYPAD_Scan()==16)    // 模式0 无动作
		{
			An_Jian = An_Jian;
		}
		if(KEYPAD_Scan()==1)    // 模式1 OLED 显示键盘输入的距离与角度，并发送给openmv，openmv收到信号后，启动继电器
		{
					RELAY1_ON;       // 充电继电器
					Delay_s(1);
					RELAY1_OFF;
			
					Delay_s(3);

					RELAY2_ON;       // 开炮继电器
					Delay_ms(500);
					RELAY2_OFF; 
			break;

		}
		
	}
	
}
///*********************************************END OF FILE**********************/
