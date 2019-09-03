/***************(C)COPYRIGHT 2016 YSU_信息学院325工作室_WXD***************
*文件名         : MPU9250.c
*描述           : STM32103 MPU9250函数
*实验平台       : STM32F103ZET6最小系统板
*库版本         : V1.0
*嵌入式系统     : 无
*作者           : 武旭东
*修改历史       : 2017.4.3
*备注           : 使用了软件模拟IIC
*********************************************************************/
#include "iicSoft.h"
#include "MPU9250.h"
#include "delay.h"
/*引脚定义*/
GPIO INT={PB,GPIO_Pin_13};
GPIO FSYNC={PB,GPIO_Pin_11};
GPIO AD0={PB,GPIO_Pin_14};
/********************************************************************
*函数名称       : MPU9250_init
*功能说明       : MPU9250初始化函数
*参数说明       :
                  [IN]
									     无											 
                  [OUT]
                       无
*函数返回				: 无
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
u8 MPU9250_init(void)
{
	u8 res=0;
	iicSoftInit();     //初始化IIC总线
	/*MPU9250接口初始化*/
	GPIO_init(INT,GPIO_Mode_Out_PP);
	GPIO_init(FSYNC,GPIO_Mode_Out_PP);
	GPIO_init(AD0,GPIO_Mode_Out_PP);
	GPIO_setPin(INT,0);
	GPIO_setPin(AD0,0);
	GPIO_setPin(FSYNC,1);
	/**/
	iicWriteByte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X80);//复位MPU9250
	delayMs(100);  //延时100ms
	iicWriteByte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X00);//唤醒MPU9250?
	MPU9250_setGyroFer(1);					        	//陀螺仪传感器,±500dps
	MPU9250_setAccelFsr(0);					       	 	//加速度传感器,±2g
	MPU9250_setRate(1000);						       	 	//设置采样率
	iicWriteByte(MPU9250_ADDR,MPU_INT_EN_REG,0X00);   //关闭所有中断
	iicWriteByte(MPU9250_ADDR,MPU_USER_CTRL_REG,0X00);//I2C主模式关闭
	iicWriteByte(MPU9250_ADDR,MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	iicWriteByte(MPU9250_ADDR,MPU_INTBP_CFG_REG,0X82);//INT引脚低电平有效，开启bypass模式，可以直接读取磁力计
	iicReadByte(MPU9250_ADDR,MPU_DEVICE_ID_REG,&res);  //读取MPU6500的ID
	if(res==MPU6500_ID) //器件ID正确
	{
			iicWriteByte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X01);  	//设置CLKSEL,PLL X轴为参考
			iicWriteByte(MPU9250_ADDR,MPU_PWR_MGMT2_REG,0X00);  	//加速度与陀螺仪都工作
			MPU9250_setRate(1000);
      MPU9250_setAccLpf(42);		
	}else return 1;
	
	iicReadByte(AK8963_ADDR,MAG_WIA,&res);    			//读取AK8963 ID   
	if(res==AK8963_ID)
	{

			iicWriteByte(AK8963_ADDR,MAG_CNTL1,0X11);		//设置AK8963为单次测量模式
	}else return 1;

	return 0;
}
/********************************************************************
*函数名称       : MPU9250_setGyroFer
*功能说明       : MPU9250设置陀螺仪传感器满量程范围
*参数说明       :
                  [IN]
									    u8 fsr 陀螺仪范围， 0,±250dps;1,±500dps;2,±1000dps;3,±2000dps							 
                  [OUT]
                       无
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
u8 MPU9250_setGyroFer(u8 fsr)
{                            
	u8 temp;
	temp=fsr<<3;
	return iicWriteByte(MPU9250_ADDR,MPU_GYRO_CFG_REG,temp);/*设置陀螺仪满量程范围*/
}
/********************************************************************
*函数名称       : MPU9250_setAccelFsr
*功能说明       : MPU9250设置加速度传感器满量程范围
*参数说明       :
                  [IN]
									    u8 fsr 加速度传感器满量程范围，0,±2g;1,±4g;2,±8g;3,±16g		 
                  [OUT]
                       无
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
u8 MPU9250_setAccelFsr(u8 fsr)
{
	u8 temp;
	temp=fsr<<3;
	return iicWriteByte(MPU9250_ADDR,MPU_ACCEL_CFG_REG,temp);	
}
/********************************************************************
*函数名称       : MPU9250_setLpf
*功能说明       : MPU9250设置数字低通滤波器频率
*参数说明       :
                  [IN]
									    u8 fsr 数字低通滤波频率（Hz）		 
                  [OUT]
                       无
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
u8 MPU9250_setLpf(u8 lpf)
{
  u8 data=0;
	if(lpf>=184)data=1;
	else if(lpf>=92)data=2;
	else if(lpf>=41)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return iicWriteByte(MPU9250_ADDR,MPU_CFG_REG,data);	/*设置数字低通滤波器*/
}
/********************************************************************
*函数名称       : MPU9250_setAccLpf
*功能说明       : MPU9250设置加速度计低通滤波频率
*参数说明       :
                  [IN]
									    u8 fsr 数字低通滤波频率（Hz）		 
                  [OUT]
                       无
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
u8 MPU9250_setAccLpf(u8 lpf)
{
  u8 data=0;
	if(lpf>=184)data=1;
	else if(lpf>=92)data=2;
	else if(lpf>=41)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return iicWriteByte(MPU9250_ADDR,MPU_ACCEL_CFG_REG,data);	/*设置数字低通滤波器*/
}
/********************************************************************
*函数名称       : MPU9250_setRate
*功能说明       : MPU9250设置MPU9250采样率
*参数说明       :
                  [IN]
									    u8 rate 采样率 4~1000Hz
                  [OUT]
                       无
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
u8 MPU9250_setRate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	if(iicWriteByte(MPU9250_ADDR,MPU_SAMPLE_RATE_REG,data) == 1)//设置数字低通滤波器
	{
		return MPU9250_setLpf(rate/2);	//自动设置LPF为采样率的一半	
	}
	else
	{
		return 1;
	}
 	
}
/********************************************************************
*函数名称       : MPU9250_getTemperature
*功能说明       : MPU9250获取MPU9250的温度值
*参数说明       :
                  [IN]
									    无
                  [OUT]
                       无
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
short MPU9250_getTemperature(void)
{
  u8 buf[2]; 
  short raw;
	float temp;
  iicReadLen(MPU9250_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
	raw=((u16)buf[0]<<8)|buf[1];  
	temp=21+((double)raw)/333.87;  
	return temp*100;
}
/********************************************************************
*函数名称       : MPU9250_getGyroscope
*功能说明       : MPU9250获取陀螺仪原始值
*参数说明       :
                  [IN]
									    无
                  [OUT]
                       short *gx 陀螺仪X轴数值
                       short *gy 陀螺仪Y轴数值
                       short *gz 陀螺仪Z轴数值
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
u8 MPU9250_getGyroscope(int16_t *gx,int16_t *gy,int16_t *gz)
{
    u8 buf[6];
	u8 res; 
	res=iicReadLen(MPU9250_ADDR,MPU_GYRO_XOUTH_REG,6,buf); 
	if(res==0)
	{
		*gx=(int16_t)(((u16)buf[0]<<8)|buf[1]);  
		*gy=(int16_t)(((u16)buf[2]<<8)|buf[3]);  
		*gz=(int16_t)(((u16)buf[4]<<8)|buf[5]);
	} 	
    return res;	
}
/********************************************************************
*函数名称       : MPU9250_getAccelerometer
*功能说明       : MPU9250获取加速度原始读数
*参数说明       :
                  [IN]
									    无
                  [OUT]
                       short *gx 加速度X轴数值
                       short *gy 加速度Y轴数值
                       short *gz 加速度Z轴数值
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
u8 MPU9250_getAccelerometer(int16_t *ax,int16_t *ay,int16_t *az)
{
  u8 buf[6];
	u8 res;  
	res=iicReadLen(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,6,buf); 
	if(res==0)
	{
		*ax=(int16_t)(((u16)buf[0]<<8)|buf[1]);  
		*ay=(int16_t)(((u16)buf[2]<<8)|buf[3]);  
		*az=(int16_t)(((u16)buf[4]<<8)|buf[5]);
	} 	
    return res;	
}
/********************************************************************
*函数名称       : MPU9250_getMagnetmeter
*功能说明       : MPU9250获取磁力计原始读数
*参数说明       :
                  [IN]
									    无
                  [OUT]
                       short *mx 磁力计X轴数值
                       short *my 磁力计Y轴数值
                       short *mz 磁力计Z轴数值
*函数返回				: 
*修改时间				:
*备注						:
*作者           :武旭东
*********************************************************************/
u8 MPU9250_getMagnetmeter(int16_t *mx,int16_t *my,int16_t *mz)
{
  u8 buf[6];
  u8 res;	

	res=iicReadLen(AK8963_ADDR,MAG_XOUT_L,6,buf); 
	if(res==0)
	{
		*mx=(int16_t)(((u16)buf[1]<<8)|buf[0]);  
		*my=(int16_t)(((u16)buf[3]<<8)|buf[2]);  
		*mz=(int16_t)(((u16)buf[5]<<8)|buf[4]);
	} 
	res=iicReadLen(AK8963_ADDR,MAG_ASAX,3,buf); 
	if(res==0)
	{
		*mx=*mx * (((buf[0]-128)*0.5f)/128.0f+1.0f);
		*my=*my * (((buf[1]-128)*0.5f)/128.0f+1.0f);
		*mz=*mz * (((buf[2]-128)*0.5f)/128.0f+1.0f);
	}  	
  iicWriteByte(AK8963_ADDR,MAG_CNTL1,0X11);	
  //iicWriteByte(AK8963_ADDR,MAG_CNTL1,0X11); //AK8963每次读完以后都需要重新设置为单次测量模式
  return res;
}


