#include "stm32f10x.h"
#include "bsp_Advance_tim.h"
#include "delay.h"
#include "bsp_i2c_gpio.h"
#include "bsp_usart.h"
#include "OLED_I2C.h"
#include "keypad.h"
#include "bsp_relay.h"

uint16_t An_Jian=0;

int main(void)
{
	USART_Config();              // 串口初始化
	i2c_CfgGpio();				 // I2C总线的GPIO初始化
	OLED_Init();				 // OLED初始化
	RELAY_GPIO_Config();         // 继电器GPIO初始化
	SysTick_Init();

	// 检测OLED
    OLED_Fill(0xFF);//全屏点亮
	Delay_s(2);
	OLED_Fill(0x00);//全屏灭
	
	while(1)
	{
		if(KEYPAD_Scan()==16)    // 模式0 无动作
		{
			An_Jian = An_Jian;
		}
		if(KEYPAD_Scan()==12)    // 模式1 OLED 显示键盘输入的距离与角度，并发送给openmv，openmv收到信号后，启动继电器
		{
			Mode1_Key();

			RELAY1_ON;       // 充电继电器
			Delay_ms(100);
			RELAY1_OFF;
			
			Delay_s(3);

			RELAY2_ON;       // 开炮继电器
			Delay_ms(100);
			RELAY2_OFF;   

		}

	}
			//

/* 		delay_ms(delay_time);
		TIM_SetCompare1(TIM1, 175);
		delay_ms(delay_time);
		TIM_SetCompare2(TIM1, 175);
		delay_ms(delay_time);
		TIM_SetCompare1(TIM1, 180);
		delay_ms(delay_time);
		TIM_SetCompare2(TIM1, 180);
		delay_ms(delay_time);
		TIM_SetCompare1(TIM1, 185);
		delay_ms(delay_time);
		TIM_SetCompare2(TIM1, 185);
		delay_ms(delay_time);
		TIM_SetCompare1(TIM1, 190);
		delay_ms(delay_time);
		TIM_SetCompare2(TIM1, 190);
		delay_ms(delay_time);
		TIM_SetCompare1(TIM1, 170);
		delay_ms(delay_time);
		TIM_SetCompare2(TIM1, 170); */
}


