
#include "my_include.h"
#include "delay.h"


extern float C_P; //cameraP
extern float C_D; //cameraD
uint16 integer_read=0;//������С�����֣�����flash��
uint16 decimal_read=0;

void init()
{
		SystemInit(); 	 
	  delay_init();
		BEEP_Init();
   	OLED_Init();
	  OLED_Fill(0x00);
	 	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�

		KEY_Init();
		/******** TIM1������������ź� PWM ************/
	 	TIM1_PWM_Init(20000-1,72-1);//��Ƶ/  72M/72=1MHz
	 //��ʱ��
	 	TIM3_Int_Init(1000-1,840-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����100��Ϊ10ms  
	 	//Tout=((arr+1)*(psc+1))/Ft us.

		uart_init(115200);

		//STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)integer_read,sizeof(integer_read));
		//STMFLASH_Read(FLASH_SAVE_ADDR+2*1024,(u16*)decimal_read,sizeof(decimal_read));
	 // C_P=integer_read+((float)decimal_read*0.1);


}		





