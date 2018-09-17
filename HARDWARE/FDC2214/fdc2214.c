#include "fdc2214.h"
#include "delay.h"
#include "led.h"

u32 Data_FDC;
void FDC_IIC_Delay(void)
{
	delay_us(2);
}

void FDC_GPIO_Init(void)
{
	
}

void FDC_IIC_Init(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//��ʹ������IO PORTCʱ�� 
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);					    //�����趨������ʼ��GPIO 
	
  GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5);				  //PC.4,PC.5 �����	
}

//����IIC��ʼ�ź�
void FDC_IIC_Start(void)
{
	FDC_SDA_OUT();     //sda�����
	FDC_IIC_SDA=1;	  	  
	FDC_IIC_SCL=1;
	FDC_IIC_Delay();
 	FDC_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	FDC_IIC_Delay();
	FDC_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void FDC_IIC_Stop(void)
{
	FDC_SDA_OUT();//sda�����
	FDC_IIC_SCL=0;
	FDC_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	FDC_IIC_Delay();
	FDC_IIC_SCL=1; 
	FDC_IIC_SDA=1;//����I2C���߽����ź�
	FDC_IIC_Delay();							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 FDC_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	FDC_SDA_IN();      //SDA����Ϊ����  
	FDC_IIC_SDA=1;FDC_IIC_Delay();	   
	FDC_IIC_SCL=1;FDC_IIC_Delay();	 
	while(FDC_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			FDC_IIC_Stop();
			return 1;
		}
	}
	FDC_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
}


//����ACKӦ��
//void FDC_IIC_Ack(void)
//{
//	FDC_IIC_SCL=0;
//	FDC_SDA_OUT();
//	FDC_IIC_SDA=0;
//	FDC_IIC_Delay();
//	FDC_IIC_SCL=1;
//	FDC_IIC_Delay();
//	FDC_IIC_SCL=0;
//}

//debug
void FDC_IIC_Ack(void)
{
	FDC_IIC_SCL=0;
	FDC_IIC_SDA=0;
	FDC_IIC_Delay();
	FDC_IIC_SCL=1;
	FDC_IIC_Delay();
	FDC_IIC_SCL=0;
	FDC_IIC_Delay();
	FDC_IIC_SDA=1;
}


//������ACKӦ��		    
void FDC_IIC_NAck(void)
{
	FDC_IIC_SCL=0;
	FDC_SDA_OUT();
	FDC_IIC_SDA=1;
	FDC_IIC_Delay();
	FDC_IIC_SCL=1;
	FDC_IIC_Delay();
	FDC_IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void FDC_IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
	FDC_SDA_OUT(); 	    
    FDC_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        FDC_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		    FDC_IIC_SCL=1;
		    FDC_IIC_Delay(); 
		    FDC_IIC_SCL=0;	
		    FDC_IIC_Delay();
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 FDC_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	FDC_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        FDC_IIC_SCL=0; 
        FDC_IIC_Delay();
		FDC_IIC_SCL=1;
        receive<<=1;
        if(FDC_READ_SDA)receive++;   
		FDC_IIC_Delay(); 
    }				 
    if (!ack)
        FDC_IIC_NAck();//����nACK
    else
        FDC_IIC_Ack(); //����ACK   
    return receive;
}

u8 Set_FDC2214(u8 reg,u8 MSB,u8 LSB) 				 
{ 
    FDC_IIC_Start(); 
	FDC_IIC_Send_Byte((FDC2214_ADDR<<1)|0);//����������ַ+д����	
	if(FDC_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		FDC_IIC_Stop();		 
		return 1;		
	}
    FDC_IIC_Send_Byte(reg);//д�Ĵ�����ַ
    FDC_IIC_Wait_Ack();		 //�ȴ�Ӧ�� 
	FDC_IIC_Send_Byte(MSB);  //��������1
	if(FDC_IIC_Wait_Ack())	 //�ȴ�ACK
	{
		FDC_IIC_Stop();	 
		return 1;		 
	}		 
	FDC_IIC_Send_Byte(LSB);  //��������2
	if(FDC_IIC_Wait_Ack())	 //�ȴ�ACK
	{
		FDC_IIC_Stop();	 
		return 1;		 
	}	
    FDC_IIC_Stop();	 
	return 0;
}

u16 FDC_Read(u8 reg)
{
	u16 res;
   FDC_IIC_Start(); 
	FDC_IIC_Send_Byte((FDC2214_ADDR<<1)|0);//����������ַ+д����	
	FDC_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
    FDC_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    FDC_IIC_Wait_Ack();		//�ȴ�Ӧ��
    FDC_IIC_Start();
	FDC_IIC_Send_Byte((FDC2214_ADDR<<1)|1);//����������ַ+������	
    FDC_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	res=FDC_IIC_Read_Byte(1)<<8;//��ȡ����,����ACK
	
//	FDC_IIC_Ack(); 
	res|=FDC_IIC_Read_Byte(0);//��ȡ����,����nACK
    FDC_IIC_Stop();			//����һ��ֹͣ����
	return res;		
}

u32 FCD2214_ReadCH(u8 index) 
{
	u32 result;
	switch(index)
	{
		case 0:
		  result = FDC_Read(DATA_CH0)&0x0FFF;
		  result = (result<<16)|(FDC_Read(DATA_LSB_CH0));
			break;
		case 1:
			result = FDC_Read(DATA_CH1)&0x0FFF;
		  result = (result<<16)|(FDC_Read(DATA_LSB_CH1));
			break;
		case 2:
			result = FDC_Read(DATA_CH2)&0x0FFF;
		  result = (result<<16)|(FDC_Read(DATA_LSB_CH2));
			break;
		case 3:
			result = FDC_Read(DATA_CH3)&0x0FFF;
		  result = (result<<16)|(FDC_Read(DATA_LSB_CH3));
			break;
		default:break;
	}
	result =result&0x0FFFFFFF;
	return result;
}

/*FDC2214��ʼ������
 *����ֵ:0����ʼ������
 *       1��������
 */
u8 FDC2214_Init(void)
{
	u16 res,ID_FDC2214;
	FDC_GPIO_Init();
	FDC_IIC_Init();
	res=FDC_Read(MANUFACTURER_ID);//�����豸��ȡ�Ƿ�����
	ID_FDC2214 = FDC_Read(DEVICE_ID);
	
	printf("DEVICE_ID:0X%x\r\n",ID_FDC2214);
	if(res==0x5449)
	{
		//����Set_FDC2214�Ĵ���
		Set_FDC2214(RCOUNT_CH0,0x34,0xFB);//�ο�����ת�����ʱ�䣨T=(RCOUNT_CH0*16)/Frefx��
		Set_FDC2214(RCOUNT_CH1,0x34,0xFB);
		Set_FDC2214(RCOUNT_CH2,0x34,0xFB);
		Set_FDC2214(RCOUNT_CH3,0x34,0xFB);
		
		Set_FDC2214(SETTLECOUNT_CH0,0x00,0x1B);//ת��֮ǰ���ȶ�ʱ�䣨T=(SETTLECOUNT_CHx*16)/Frefx��
		Set_FDC2214(SETTLECOUNT_CH1,0x00,0x1B);
		Set_FDC2214(SETTLECOUNT_CH2,0x00,0x1B);
		Set_FDC2214(SETTLECOUNT_CH3,0x00,0x1B);
		
		Set_FDC2214(CLOCK_DIVIDERS_C_CH0,0x20,0x02);//ѡ����0.01MHz ~ 10MHz�Ĵ�����Ƶ��
		Set_FDC2214(CLOCK_DIVIDERS_C_CH1,0x20,0x02);//Frefx = Fclk = 43.4MHz/2(2��Ƶ)
		Set_FDC2214(CLOCK_DIVIDERS_C_CH2,0x20,0x02);//CHx_REF_DIVIDER=2;CHx_FIN_SEL=2
		Set_FDC2214(CLOCK_DIVIDERS_C_CH3,0x20,0x02);//CHx_REF_DIVIDER=2;CHx_FIN_SEL=2

		Set_FDC2214(DRIVE_CURRENT_CH0,0x78,0x00);//0.146ma��������ʱ�ӽ���+ת��ʱ�������������
		Set_FDC2214(DRIVE_CURRENT_CH1,0x78,0x00);
		Set_FDC2214(DRIVE_CURRENT_CH2,0x78,0x00);
		Set_FDC2214(DRIVE_CURRENT_CH3,0x78,0x00);
		
		Set_FDC2214(ERROR_CONFIG,0x00,0x00);//ȫ����ֹ����㱨
		
		Set_FDC2214(MUX_CONFIG,0xC2,0x0D);//ͨ��0��1��2 ��3��ѡ��10MhzΪ�����񵴲���Ƶ�ʵ�������ã���ͨ������ͨ��
		
		Set_FDC2214(CONFIG,0x14,0x01);//����ģʽ��ʹ���ڲ��������ο�Ƶ�ʣ�INTB���Ż���״̬�Ĵ������±���λ
	}
	else return 1;
	
	return 0;
}

float Cap_Calculate(u8 chx)
{
	float Cap;
	Data_FDC = FCD2214_ReadCH(chx);
	Cap = 232021045.248/(Data_FDC);
	return (Cap*Cap);
}



