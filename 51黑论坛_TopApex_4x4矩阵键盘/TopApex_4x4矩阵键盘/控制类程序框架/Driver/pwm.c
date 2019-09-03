/***************(C)COPYRIGHT 2016 YSU_��ϢѧԺ325������_WXD***************
*�ļ���         : PWM.c
*����           : STM32103 PWM��������
*ʵ��ƽ̨       : STM32F103ZET6��Сϵͳ��
*��汾         : V1.1
*Ƕ��ʽϵͳ     : ��
*����           : ����
*�޸���ʷ       : 2016.9.27�޸ģ�������74Mhz���µ�Ƶ�ʣ�ռ�ձȿɵ��ھ�������
*��ע           : 1.��ʱ��������ӳ�����ű�ɿ������ֲᣬ���ı��С���ʱ��ͨ����ӳ������������
*                 2.PB3��PB4��PA15����Ϊ��JTAG�ӿڣ��ᵼ�¶�ʱ��2��ʱ��3����ͨ��������ΪPWM������ţ�����ʹ�����ڳ����йر�JTAG����
*                   ��ʱ��2������ӳ�䡢��ȫ��ӳ��PA15��PB3������ΪPWM���
*                   ��ʱ��3������ӳ��PB4������ΪPWM���
*                 3.�޸�Ƶ��PWMƵ��Ϊdouble����
*********************************************************************/ 
#include "PWM.h"
/***************ʱ�ӱ��������***************/
TIM_TypeDef  *TIMX[8]={TIM1,TIM2,TIM3,TIM4,TIM5,TIM6,TIM7,TIM8};
/***************��ʱ��ͨ����ӳ������������***************/
//��ʱ��6��7Ϊ��ͨ��ʱ������PWM�������
//��ʱ��1
GPIO TIM1_GPIO_noRemap[4]={{PA,GPIO_Pin_8},{PA,GPIO_Pin_9},{PA,GPIO_Pin_10},{PA,GPIO_Pin_11}};
GPIO TIM1_GPIO_fullRemap[4]={{PE,GPIO_Pin_9},{PE,GPIO_Pin_11},{PE,GPIO_Pin_13},{PE,GPIO_Pin_14}};
//��ʱ��2
GPIO TIM2_GPIO_noRemap[4]={{PA,GPIO_Pin_0},{PA,GPIO_Pin_1},{PA,GPIO_Pin_2},{PA,GPIO_Pin_3}};
GPIO TIM2_GPIO_partialRemap1[4]={{PA,GPIO_Pin_15},{PB,GPIO_Pin_3},{PA,GPIO_Pin_2},{PA,GPIO_Pin_3}};
GPIO TIM2_GPIO_partialRemap2[4]={{PA,GPIO_Pin_0},{PA,GPIO_Pin_1},{PB,GPIO_Pin_10},{PB,GPIO_Pin_11}};
GPIO TIM2_GPIO_fullRemap[4]={{PA,GPIO_Pin_15},{PB,GPIO_Pin_3},{PB,GPIO_Pin_10},{PB,GPIO_Pin_11}};
//��ʱ��3
GPIO TIM3_GPIO_noRemap[4]={{PA,GPIO_Pin_6},{PA,GPIO_Pin_7},{PB,GPIO_Pin_0},{PB,GPIO_Pin_1}};
GPIO TIM3_GPIO_partialRemap[4]={{PB,GPIO_Pin_4},{PB,GPIO_Pin_5},{PB,GPIO_Pin_0},{PB,GPIO_Pin_1}};
GPIO TIM3_GPIO_fullRemap[4]={{PC,GPIO_Pin_6},{PC,GPIO_Pin_7},{PC,GPIO_Pin_8},{PC,GPIO_Pin_9}};
//��ʱ��4
GPIO TIM4_GPIO_noRemap[4]={{PB,GPIO_Pin_6},{PB,GPIO_Pin_7},{PB,GPIO_Pin_8},{PB,GPIO_Pin_9}};
GPIO TIM4_GPIO_fullRemap[4]={{PD,GPIO_Pin_12},{PD,GPIO_Pin_13},{PD,GPIO_Pin_14},{PD,GPIO_Pin_15}};	
//��ʱ��5
GPIO TIM5_GPIO_noRemap[4]={{PA,GPIO_Pin_0},{PA,GPIO_Pin_1},{PA,GPIO_Pin_2},{PA,GPIO_Pin_3}};
//��ʱ��8
GPIO TIM8_GPIO_noRemap[4]={{PC,GPIO_Pin_6},{PC,GPIO_Pin_7},{PC,GPIO_Pin_8},{PC,GPIO_Pin_9}};
/***************��ʱ��ͨ��ӳ���ӦGPIO������***************/
//��ö�ʱ������ӳ�䣬���λ������ԭʼΪ��
//����������ӳ�俴�����ֲᣬʹ��ʱ��Ӧע��
GPIO *TIM_GPIO_Remap[8][4]=
{
	{TIM1_GPIO_noRemap,0,0,TIM1_GPIO_fullRemap},
	{TIM2_GPIO_noRemap,TIM2_GPIO_partialRemap1,TIM2_GPIO_partialRemap2,TIM2_GPIO_fullRemap},
	{TIM3_GPIO_noRemap,TIM3_GPIO_partialRemap,0,TIM3_GPIO_fullRemap},
	{TIM4_GPIO_noRemap,0,0,TIM4_GPIO_fullRemap},
	{TIM5_GPIO_noRemap,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{TIM8_GPIO_noRemap,0,0,0}
};
/****************��ʱ��ʱ��ʹ��������***************/
uint32_t rccTim[8]=
{
	RCC_APB2Periph_TIM1,
	RCC_APB1Periph_TIM2,
	RCC_APB1Periph_TIM3,
	RCC_APB1Periph_TIM4,
	RCC_APB1Periph_TIM5,
	RCC_APB1Periph_TIM6,
	RCC_APB1Periph_TIM7,
	RCC_APB2Periph_TIM8,
};
/****************��ʱ��������ӳ��������***************/
uint32_t TIM_remap[8][4]=
{
	{0,0,0,GPIO_FullRemap_TIM1},
	{0,GPIO_PartialRemap1_TIM2,GPIO_PartialRemap2_TIM2,GPIO_FullRemap_TIM2},
	{0,GPIO_PartialRemap_TIM3,0,GPIO_FullRemap_TIM3},
	{0,0,0,GPIO_Remap_TIM4},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};

/******************��ʱ��ͨ����ʼ������������****************/
void (*OCn_Init[4])(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)=
{
	TIM_OC1Init,
	TIM_OC2Init,
	TIM_OC3Init,
	TIM_OC4Init
};
/******************��ʱ��ͨ��Ԥװ��ʹ�ܺ���������****************/
void (*TIM_CH_enable[4])(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)=
{
	TIM_OC1PreloadConfig,
	TIM_OC2PreloadConfig,
	TIM_OC3PreloadConfig,
	TIM_OC4PreloadConfig
};
/******************��ʱ��ʱ��ʹ�ܺ���������****************/
void (*TIM_clockCmd[8])(uint32_t RCC_APB2Periph, FunctionalState NewState)=	
{
	RCC_APB2PeriphClockCmd,
	RCC_APB1PeriphClockCmd,
	RCC_APB1PeriphClockCmd,
	RCC_APB1PeriphClockCmd,
	RCC_APB1PeriphClockCmd,
	RCC_APB1PeriphClockCmd,
	RCC_APB1PeriphClockCmd,
	RCC_APB2PeriphClockCmd,
};
/******************��ʱ��ͨ��ռ�ձ����ú���������****************/
void (*TIM_setDuty[4])(TIM_TypeDef* TIMx, uint16_t Compare3)=
{
	TIM_SetCompare1,
	TIM_SetCompare2,
	TIM_SetCompare3,
	TIM_SetCompare4
};
/**************************PWM������غ�������*****************************/
/********************************************************************
*��������       : pscCal
*����˵��       : STM32103 72Mϵͳʱ��Ƶ���¶�ʱ����Ƶ��������
*����˵��       : 
                  [IN]
												double   fre      ��Ҫ�����Ƶ��ֵ����72Mhz����дΪ72000000.0
                  [OUT]
                        uint32_t *nowFre 	��Ƶ��ʱ����ǰʱ��Ƶ��											
*��������				: ������ķ�Ƶ����
*�޸�ʱ��				: 
*��ע						: �˺����ɸ�����Ҫ���õ�PWMƵ�ʼ��㶨ʱ��ʱ�ӷ�Ƶ����ʹ���ȴﵽ����
*                 PWMƵ��      ��Ƶ��-1     ��ʱ����ǰʱ��Ƶ��	
                  39~~74         31            2250000Hz
*									75~~150        15            4500000Hz 
*									150~~300   			7            9000000Hz
*                 300~~600        3           18000000Hz
*									600~~12000      1           36000000Hz
*                 12000~~         0           72000000Hz
*����           : ����
*********************************************************************/
uint32_t pscCal(double fre,uint32_t *nowFre)
{
	uint32_t psc;//��Ƶ����
	if(fre>38&&fre<=74)//32��Ƶ
	{
		psc=31;
		*nowFre=2250000;		
	}
	else if(fre>74&&fre<=150)//16��Ƶ
	{
		psc=15;
		*nowFre=4500000;
	}
	else if(fre>150&&fre<=300)//8��Ƶ
	{
		psc=7;
		*nowFre=9000000;
	}
	else if(fre>300&&fre<=600)//4��Ƶ
	{
		psc=3;
		*nowFre=18000000;
	}
	else if(fre>600&&fre<=1200)//2��Ƶ
	{
		psc=1;
		*nowFre=36000000;
	}
	else if(fre>1200&&fre<=72000000)//����Ƶ
	{
		psc=0;
		*nowFre=72000000;
	}
	else
	{
		
	}
	return psc;
}
/********************************************************************
*��������       : TIM_PWM_GpioSet
*����˵��       : STM32103 ��ʱ��PWM�������ӳ����������
*����˵��       : 
                  [IN]
                        TIMEn time                    ��ʱ����
												TIM_GPIO_ramap CH_gpioRemap   ��ʱ��ͨ������ӳ������
                        GPIO_ramap CH_gpioRemap       �ö�ʱ������ӳ�����
                  [OUT]
                        ��												
*��������				: ��
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void TIM_PWM_GpioSet(TIMEn time,CHn ch,GPIO_ramap CH_gpioRemap)
{
	if(CH_gpioRemap)
	{
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(TIM_remap[time][CH_gpioRemap],ENABLE);
	}
	GPIO_init((TIM_GPIO_Remap[time][CH_gpioRemap])[ch],GPIO_Mode_AF_PP);
	
}
/********************************************************************
*��������       : TIM_PWM_init
*����˵��       : STM32103 PWM������ʼ������
*����˵��       :
                  [IN]
												TIMEn time                     ������Ҫʹ�õĶ�ʱ����
												CHn ch                         ��ʱ��ͨ����
												TIM_GPIO_ramap CH_gpioRemap    PWM���������ӳ��ѡ��ʹ��Ĭ����������0�����帴�������뿴�����ֲ�
												double   freg                  PWM�������Ƶ��
												uint32_t duty                  PWM�������ռ�ձȣ���Χ0~~999������ռ�ձ�0%~~99.9%
                  [OUT]
                        ��
*��������				: ��
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void TIM_PWM_init(TIMEn time,CHn ch,GPIO_ramap CH_gpioRemap,double freg,uint32_t duty)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	uint32_t arr;
	uint32_t timClock;
	uint32_t psc;
	
	psc=pscCal(freg,&timClock);
	arr=timClock/freg-1;
	

	
	TIM_clockCmd[time](rccTim[time],ENABLE);//ʹ�ܶ�ʱ��ʱ��
	TIM_PWM_GpioSet(time,ch,CH_gpioRemap);
	
  TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim�����ﲻ̫����������������
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIMX[time], &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	OCn_Init[ch](TIMX[time], &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	
	TIM_CtrlPWMOutputs(TIMX[time],ENABLE);	//MOE �����ʹ��
	
	TIM_CH_enable[ch](TIMX[time], TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIMX[time], ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_setDuty[ch](TIMX[time],arr*(1000-duty)/1000);//����ռ�ձ�
	TIM_Cmd(TIMX[time], ENABLE);  //ʹ��TIM
	
	
	
}
/********************************************************************
*��������       : TIM_PWM_init
*����˵��       : STM32103 ����PWM�������Ƶ��
*����˵��       : 
                  [IN]
												TIMEn  time            ������Ҫʹ�õĶ�ʱ����
                        double freg            PWM�������Ƶ�� 
                  [OUT]
                        ��												
*��������				: ��
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void TIM_PWM_freq(TIMEn time,double freg)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint32_t arr;
	uint32_t timClock;
	uint32_t psc;
	psc=pscCal(freg,&timClock);
	arr=timClock/freg-1;
	TIM_clockCmd[time](rccTim[time],ENABLE);//ʹ�ܶ�ʱ��ʱ��

  TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim�����ﲻ̫����������������
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIMX[time], &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ	
		
	TIM_ARRPreloadConfig(TIMX[time], ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
}

/********************************************************************
*��������       : TIM_PWM_duty
*����˵��       : STM32103 ����PWM�������ռ�ձ�
*����˵��       : 
                  [IN]
												TIMEn time               ������Ҫʹ�õĶ�ʱ����
												CHn ch                   ��ʱ��ͨ����   
                        uint32_t duty            PWM�������ռ�ձ� 1~~1000
                  [OUT]
                        ��												
*��������				: ��
*�޸�ʱ��				: 
*��ע						:
*����           : ����
*********************************************************************/
void TIM_PWM_duty(TIMEn time,CHn ch,uint32_t duty)
{
	uint32_t arr;
	arr=TIMX[time]->ARR;
	TIM_setDuty[ch](TIMX[time],arr*(1000-duty)/1000);//����ռ�ձ�
}
