//����
//2018.8.27
//�����ֵ  1300
//�����ֵ  1450
//�����ֵ  1600

#include "sys.h"
#include "my_include.h"



int main(void)
{

		init();			//ϵͳ��ʼ��
		Init_Show();//��ʼ������
		TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
		TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
		
	  while(1)
		{  		
			Boma_OLED();	

			KYE3_SHOW();	
		}
	 
 }
