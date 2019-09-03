/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : PWM.c
*描述           : STM32103 PWM驱动函数
*实验平台       : STM32F103ZET6最小系统板
*库版本         : V1.1
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 2016.9.27修改，增加了74Mhz以下的频率，占空比可调节精度增加
*备注           : 1.定时器复用重映射引脚表可看数据手册，或本文本中“定时器通道重映射引脚索引表”
*                 2.PB3、PB4、PA15设置为了JTAG接口，会导致定时器2定时器3部分通道不可作为PWM输出引脚，如需使用请在程序中关闭JTAG功能
*                   定时器2部分重映射、完全重映射PA15、PB3不能做为PWM输出
*                   定时器3部分重映射PB4不能做为PWM输出
*                 3.修改频率PWM频率为double类型
*********************************************************************/ 
#include "PWM.h"
/***************时钟标号索引表***************/
TIM_TypeDef  *TIMX[8]={TIM1,TIM2,TIM3,TIM4,TIM5,TIM6,TIM7,TIM8};
/***************定时器通道重映射引脚索引表***************/
//定时器6、7为普通定时器，无PWM输出功能
//定时器1
GPIO TIM1_GPIO_noRemap[4]={{PA,GPIO_Pin_8},{PA,GPIO_Pin_9},{PA,GPIO_Pin_10},{PA,GPIO_Pin_11}};
GPIO TIM1_GPIO_fullRemap[4]={{PE,GPIO_Pin_9},{PE,GPIO_Pin_11},{PE,GPIO_Pin_13},{PE,GPIO_Pin_14}};
//定时器2
GPIO TIM2_GPIO_noRemap[4]={{PA,GPIO_Pin_0},{PA,GPIO_Pin_1},{PA,GPIO_Pin_2},{PA,GPIO_Pin_3}};
GPIO TIM2_GPIO_partialRemap1[4]={{PA,GPIO_Pin_15},{PB,GPIO_Pin_3},{PA,GPIO_Pin_2},{PA,GPIO_Pin_3}};
GPIO TIM2_GPIO_partialRemap2[4]={{PA,GPIO_Pin_0},{PA,GPIO_Pin_1},{PB,GPIO_Pin_10},{PB,GPIO_Pin_11}};
GPIO TIM2_GPIO_fullRemap[4]={{PA,GPIO_Pin_15},{PB,GPIO_Pin_3},{PB,GPIO_Pin_10},{PB,GPIO_Pin_11}};
//定时器3
GPIO TIM3_GPIO_noRemap[4]={{PA,GPIO_Pin_6},{PA,GPIO_Pin_7},{PB,GPIO_Pin_0},{PB,GPIO_Pin_1}};
GPIO TIM3_GPIO_partialRemap[4]={{PB,GPIO_Pin_4},{PB,GPIO_Pin_5},{PB,GPIO_Pin_0},{PB,GPIO_Pin_1}};
GPIO TIM3_GPIO_fullRemap[4]={{PC,GPIO_Pin_6},{PC,GPIO_Pin_7},{PC,GPIO_Pin_8},{PC,GPIO_Pin_9}};
//定时器4
GPIO TIM4_GPIO_noRemap[4]={{PB,GPIO_Pin_6},{PB,GPIO_Pin_7},{PB,GPIO_Pin_8},{PB,GPIO_Pin_9}};
GPIO TIM4_GPIO_fullRemap[4]={{PD,GPIO_Pin_12},{PD,GPIO_Pin_13},{PD,GPIO_Pin_14},{PD,GPIO_Pin_15}};	
//定时器5
GPIO TIM5_GPIO_noRemap[4]={{PA,GPIO_Pin_0},{PA,GPIO_Pin_1},{PA,GPIO_Pin_2},{PA,GPIO_Pin_3}};
//定时器8
GPIO TIM8_GPIO_noRemap[4]={{PC,GPIO_Pin_6},{PC,GPIO_Pin_7},{PC,GPIO_Pin_8},{PC,GPIO_Pin_9}};
/***************定时器通重映射对应GPIO索引表***************/
//如该定时器无重映射，则该位置数组原始为零
//具体有无重映射看数据手册，使用时候应注意
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
/****************定时器时钟使能索引表***************/
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
/****************定时器有无重映射索引表***************/
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

/******************定时器通道初始化函数索引表****************/
void (*OCn_Init[4])(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)=
{
	TIM_OC1Init,
	TIM_OC2Init,
	TIM_OC3Init,
	TIM_OC4Init
};
/******************定时器通道预装载使能函数索引表****************/
void (*TIM_CH_enable[4])(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)=
{
	TIM_OC1PreloadConfig,
	TIM_OC2PreloadConfig,
	TIM_OC3PreloadConfig,
	TIM_OC4PreloadConfig
};
/******************定时器时钟使能函数索引表****************/
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
/******************定时器通道占空比设置函数索引表****************/
void (*TIM_setDuty[4])(TIM_TypeDef* TIMx, uint16_t Compare3)=
{
	TIM_SetCompare1,
	TIM_SetCompare2,
	TIM_SetCompare3,
	TIM_SetCompare4
};
/**************************PWM产生相关函数定义*****************************/
/********************************************************************
*函数名称       : pscCal
*功能说明       : STM32103 72M系统时钟频率下定时器分频参数计算
*参数说明       : 
                  [IN]
												double   fre      需要计算的频率值，如72Mhz，请写为72000000.0
                  [OUT]
                        uint32_t *nowFre 	分频后定时器当前时钟频率											
*函数返回				: 计算出的分频参数
*修改时间				: 
*备注						: 此函数可根据需要设置的PWM频率计算定时器时钟分频数，使精度达到最优
*                 PWM频率      分频数-1     定时器当前时钟频率	
                  39~~74         31            2250000Hz
*									75~~150        15            4500000Hz 
*									150~~300   			7            9000000Hz
*                 300~~600        3           18000000Hz
*									600~~12000      1           36000000Hz
*                 12000~~         0           72000000Hz
*作者           : 武旭东
*********************************************************************/
uint32_t pscCal(double fre,uint32_t *nowFre)
{
	uint32_t psc;//分频参数
	if(fre>38&&fre<=74)//32分频
	{
		psc=31;
		*nowFre=2250000;		
	}
	else if(fre>74&&fre<=150)//16分频
	{
		psc=15;
		*nowFre=4500000;
	}
	else if(fre>150&&fre<=300)//8分频
	{
		psc=7;
		*nowFre=9000000;
	}
	else if(fre>300&&fre<=600)//4分频
	{
		psc=3;
		*nowFre=18000000;
	}
	else if(fre>600&&fre<=1200)//2分频
	{
		psc=1;
		*nowFre=36000000;
	}
	else if(fre>1200&&fre<=72000000)//不分频
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
*函数名称       : TIM_PWM_GpioSet
*功能说明       : STM32103 定时器PWM输出复用映射引脚设置
*参数说明       : 
                  [IN]
                        TIMEn time                    定时器号
												TIM_GPIO_ramap CH_gpioRemap   定时器通道复用映射设置
                        GPIO_ramap CH_gpioRemap       该定时器复用映射情况
                  [OUT]
                        无												
*函数返回				: 无
*修改时间				: 
*备注						:
*作者           : 武旭东
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
*函数名称       : TIM_PWM_init
*功能说明       : STM32103 PWM产生初始化函数
*参数说明       :
                  [IN]
												TIMEn time                     产生需要使用的定时器号
												CHn ch                         定时器通道号
												TIM_GPIO_ramap CH_gpioRemap    PWM输出引脚重映射选择，使用默认引脚输入0，具体复用引脚请看数据手册
												double   freg                  PWM输出脉冲频率
												uint32_t duty                  PWM输出脉冲占空比，范围0~~999，代表占空比0%~~99.9%
                  [OUT]
                        无
*函数返回				: 无
*修改时间				: 
*备注						:
*作者           : 武旭东
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
	

	
	TIM_clockCmd[time](rccTim[time],ENABLE);//使能定时器时钟
	TIM_PWM_GpioSet(time,ch,CH_gpioRemap);
	
  TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim，这里不太懂？？？？？？？
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIMX[time], &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	OCn_Init[ch](TIMX[time], &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	
	TIM_CtrlPWMOutputs(TIMX[time],ENABLE);	//MOE 主输出使能
	
	TIM_CH_enable[ch](TIMX[time], TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIMX[time], ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_setDuty[ch](TIMX[time],arr*(1000-duty)/1000);//设置占空比
	TIM_Cmd(TIMX[time], ENABLE);  //使能TIM
	
	
	
}
/********************************************************************
*函数名称       : TIM_PWM_init
*功能说明       : STM32103 更改PWM输出脉冲频率
*参数说明       : 
                  [IN]
												TIMEn  time            产生需要使用的定时器号
                        double freg            PWM输出脉冲频率 
                  [OUT]
                        无												
*函数返回				: 无
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
void TIM_PWM_freq(TIMEn time,double freg)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint32_t arr;
	uint32_t timClock;
	uint32_t psc;
	psc=pscCal(freg,&timClock);
	arr=timClock/freg-1;
	TIM_clockCmd[time](rccTim[time],ENABLE);//使能定时器时钟

  TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim，这里不太懂？？？？？？？
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIMX[time], &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位	
		
	TIM_ARRPreloadConfig(TIMX[time], ENABLE); //使能TIMx在ARR上的预装载寄存器
}

/********************************************************************
*函数名称       : TIM_PWM_duty
*功能说明       : STM32103 更改PWM输出脉冲占空比
*参数说明       : 
                  [IN]
												TIMEn time               产生需要使用的定时器号
												CHn ch                   定时器通道号   
                        uint32_t duty            PWM输出脉冲占空比 1~~1000
                  [OUT]
                        无												
*函数返回				: 无
*修改时间				: 
*备注						:
*作者           : 武旭东
*********************************************************************/
void TIM_PWM_duty(TIMEn time,CHn ch,uint32_t duty)
{
	uint32_t arr;
	arr=TIMX[time]->ARR;
	TIM_setDuty[ch](TIMX[time],arr*(1000-duty)/1000);//设置占空比
}
