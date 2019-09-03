#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "uart4.h" 
#include "openmv.h"
#include "lcd.h"

/************************************************
 ALIENTEKս��STM32������ʵ��4
 ����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


 int main(void)
 {		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
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

