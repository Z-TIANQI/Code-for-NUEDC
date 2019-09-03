/***************(C)COPYRIGHT 2016 YSU_��ϢѧԺ325������_WXD***************
*�ļ���         : MPU9250.c
*����           : STM32103 MPU9250����
*ʵ��ƽ̨       : STM32F103ZET6��Сϵͳ��
*��汾         : V1.0
*Ƕ��ʽϵͳ     : ��
*����           : ����
*�޸���ʷ       : 2017.4.3
*��ע           : ʹ�������ģ��IIC
*********************************************************************/
#include "iicSoft.h"
#include "MPU9250.h"
#include "delay.h"
/*���Ŷ���*/
GPIO INT={PB,GPIO_Pin_13};
GPIO FSYNC={PB,GPIO_Pin_11};
GPIO AD0={PB,GPIO_Pin_14};
/********************************************************************
*��������       : MPU9250_init
*����˵��       : MPU9250��ʼ������
*����˵��       :
                  [IN]
									     ��											 
                  [OUT]
                       ��
*��������				: ��
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
u8 MPU9250_init(void)
{
	u8 res=0;
	iicSoftInit();     //��ʼ��IIC����
	/*MPU9250�ӿڳ�ʼ��*/
	GPIO_init(INT,GPIO_Mode_Out_PP);
	GPIO_init(FSYNC,GPIO_Mode_Out_PP);
	GPIO_init(AD0,GPIO_Mode_Out_PP);
	GPIO_setPin(INT,0);
	GPIO_setPin(AD0,0);
	GPIO_setPin(FSYNC,1);
	/**/
	iicWriteByte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X80);//��λMPU9250
	delayMs(100);  //��ʱ100ms
	iicWriteByte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X00);//����MPU9250?
	MPU9250_setGyroFer(1);					        	//�����Ǵ�����,��500dps
	MPU9250_setAccelFsr(0);					       	 	//���ٶȴ�����,��2g
	MPU9250_setRate(1000);						       	 	//���ò�����
	iicWriteByte(MPU9250_ADDR,MPU_INT_EN_REG,0X00);   //�ر������ж�
	iicWriteByte(MPU9250_ADDR,MPU_USER_CTRL_REG,0X00);//I2C��ģʽ�ر�
	iicWriteByte(MPU9250_ADDR,MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
	iicWriteByte(MPU9250_ADDR,MPU_INTBP_CFG_REG,0X82);//INT���ŵ͵�ƽ��Ч������bypassģʽ������ֱ�Ӷ�ȡ������
	iicReadByte(MPU9250_ADDR,MPU_DEVICE_ID_REG,&res);  //��ȡMPU6500��ID
	if(res==MPU6500_ID) //����ID��ȷ
	{
			iicWriteByte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X01);  	//����CLKSEL,PLL X��Ϊ�ο�
			iicWriteByte(MPU9250_ADDR,MPU_PWR_MGMT2_REG,0X00);  	//���ٶ��������Ƕ�����
			MPU9250_setRate(1000);
      MPU9250_setAccLpf(42);		
	}else return 1;
	
	iicReadByte(AK8963_ADDR,MAG_WIA,&res);    			//��ȡAK8963 ID   
	if(res==AK8963_ID)
	{

			iicWriteByte(AK8963_ADDR,MAG_CNTL1,0X11);		//����AK8963Ϊ���β���ģʽ
	}else return 1;

	return 0;
}
/********************************************************************
*��������       : MPU9250_setGyroFer
*����˵��       : MPU9250���������Ǵ����������̷�Χ
*����˵��       :
                  [IN]
									    u8 fsr �����Ƿ�Χ�� 0,��250dps;1,��500dps;2,��1000dps;3,��2000dps							 
                  [OUT]
                       ��
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
u8 MPU9250_setGyroFer(u8 fsr)
{                            
	u8 temp;
	temp=fsr<<3;
	return iicWriteByte(MPU9250_ADDR,MPU_GYRO_CFG_REG,temp);/*���������������̷�Χ*/
}
/********************************************************************
*��������       : MPU9250_setAccelFsr
*����˵��       : MPU9250���ü��ٶȴ����������̷�Χ
*����˵��       :
                  [IN]
									    u8 fsr ���ٶȴ����������̷�Χ��0,��2g;1,��4g;2,��8g;3,��16g		 
                  [OUT]
                       ��
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
u8 MPU9250_setAccelFsr(u8 fsr)
{
	u8 temp;
	temp=fsr<<3;
	return iicWriteByte(MPU9250_ADDR,MPU_ACCEL_CFG_REG,temp);	
}
/********************************************************************
*��������       : MPU9250_setLpf
*����˵��       : MPU9250�������ֵ�ͨ�˲���Ƶ��
*����˵��       :
                  [IN]
									    u8 fsr ���ֵ�ͨ�˲�Ƶ�ʣ�Hz��		 
                  [OUT]
                       ��
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
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
	return iicWriteByte(MPU9250_ADDR,MPU_CFG_REG,data);	/*�������ֵ�ͨ�˲���*/
}
/********************************************************************
*��������       : MPU9250_setAccLpf
*����˵��       : MPU9250���ü��ٶȼƵ�ͨ�˲�Ƶ��
*����˵��       :
                  [IN]
									    u8 fsr ���ֵ�ͨ�˲�Ƶ�ʣ�Hz��		 
                  [OUT]
                       ��
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
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
	return iicWriteByte(MPU9250_ADDR,MPU_ACCEL_CFG_REG,data);	/*�������ֵ�ͨ�˲���*/
}
/********************************************************************
*��������       : MPU9250_setRate
*����˵��       : MPU9250����MPU9250������
*����˵��       :
                  [IN]
									    u8 rate ������ 4~1000Hz
                  [OUT]
                       ��
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
*********************************************************************/
u8 MPU9250_setRate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	if(iicWriteByte(MPU9250_ADDR,MPU_SAMPLE_RATE_REG,data) == 1)//�������ֵ�ͨ�˲���
	{
		return MPU9250_setLpf(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��	
	}
	else
	{
		return 1;
	}
 	
}
/********************************************************************
*��������       : MPU9250_getTemperature
*����˵��       : MPU9250��ȡMPU9250���¶�ֵ
*����˵��       :
                  [IN]
									    ��
                  [OUT]
                       ��
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
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
*��������       : MPU9250_getGyroscope
*����˵��       : MPU9250��ȡ������ԭʼֵ
*����˵��       :
                  [IN]
									    ��
                  [OUT]
                       short *gx ������X����ֵ
                       short *gy ������Y����ֵ
                       short *gz ������Z����ֵ
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
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
*��������       : MPU9250_getAccelerometer
*����˵��       : MPU9250��ȡ���ٶ�ԭʼ����
*����˵��       :
                  [IN]
									    ��
                  [OUT]
                       short *gx ���ٶ�X����ֵ
                       short *gy ���ٶ�Y����ֵ
                       short *gz ���ٶ�Z����ֵ
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
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
*��������       : MPU9250_getMagnetmeter
*����˵��       : MPU9250��ȡ������ԭʼ����
*����˵��       :
                  [IN]
									    ��
                  [OUT]
                       short *mx ������X����ֵ
                       short *my ������Y����ֵ
                       short *mz ������Z����ֵ
*��������				: 
*�޸�ʱ��				:
*��ע						:
*����           :����
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
  //iicWriteByte(AK8963_ADDR,MAG_CNTL1,0X11); //AK8963ÿ�ζ����Ժ���Ҫ��������Ϊ���β���ģʽ
  return res;
}


