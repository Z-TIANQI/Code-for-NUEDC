#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "uart4.h" 
#include "openmv.h"
#include "lcd.h"

/************************************************
 ALIENTEK战舰STM32开发板实验4
 串口实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


 int main(void)
 {		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	uart4_Init();
	LCD_Init();
	POINT_COLOR=RED;
  	while(1) 
	{	
		LCD_ShowNum(0,20,200,10,24);
		LCD_ShowNum(0,40,X_black_data,10,24); 
		LCD_ShowNum(0,70,Y_black_data,10,24);
		LCD_ShowNum(0,90,X_red_data,10,24);
 		LCD_ShowNum(0,110,Y_red_data,10,24);		     					 
		LCD_ShowNum(0,130,X_yellow_data,10,24);	  
		LCD_ShowNum(0,150,Y_yellow_data,10,24);	    		
	}  
 }

