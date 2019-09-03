#include "my_include.h"
//��ʱ��3�жϷ�����

extern uint8 ball_colcor;
extern uint8 BEEP_ON_OFF;

void TIM3_IRQHandler(void)
{
	static u8 i=0;
	static u8 conut_100ms=0;
  static uint8 change_value=0;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�  5ms�ж�
	{
		i++;
		conut_100ms++;

		if(conut_100ms>=50)
		{
			LED0=!LED0;
			conut_100ms=0;
			//Ѱ��С��� ɫ�ţ�0Ϊ�죬1Ϊ�̣�
				if(change_value!=ball_colcor)
				{
					USART_SendData(USART1,ball_colcor);
				}
				change_value=ball_colcor;
		}
		if(BEEP_ON_OFF==ON)
		{
			buzzer_ring();
		}
					Car_mode_control();
					PD_control();
					FTM_updata();
		
	
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}



//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	//TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}




