#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "fdc2214.h"
#include "exti.h"
float value[5]={0.0};
float value_=0.0;
int MODE=0;
int flag_i=0;

void Sys_Init(void);

float res0,res1,res2,res3;
float temp0,temp1,temp2,temp3;

int main(void)
{			int i,j;
  Sys_Init();
	while(1)
	{
		res0 = Cap_Calculate(0);//采集数据（原始数据）
	  res1 = Cap_Calculate(1);
  	res2 = Cap_Calculate(2);
	  res3 = Cap_Calculate(3);
		
		res0 = res0-temp0;//电容接口空载减掉初始值
		res1 = res1-temp1;
		res2 = res2-temp2;
		res3 = res3-temp3;
		value_=res0;           
		OLED_P6x8Str(0,1,"CH0:",0);Oled_Printf_Float(22,1,res0,0);		
		OLED_P6x8Str(0,3,"CH1:",0);Oled_Printf_Float(22,3,res1,0);		
//		OLED_P6x8Str(0,5,"CH2:",0);Oled_Printf_Float(22,5,res2,0);		
//		OLED_P6x8Str(0,7,"CH3:",0);Oled_Printf_Float(22,7,res3,0);
//printf("CH0:%3.3f CH1:%3.3f CH2:%3.3f CH3:%3.3f\r\n",res0,res1,res2,res3);
		
/////////////////////////////////////////////////////第一步/////////////////////////////////////////////////		
if(key1==0)																
		{		
		OLED_P6x8Str(67,5,"MODE:1",0);
		OLED_P6x8Str(75,3,"        ",0);
		if(res0>1&&res0<6.1)
			OLED_P6x8Str(75,3,"Scissor!",0);
		if(res0>6.1&&res0<30)
			OLED_P6x8Str(75,3,"Hammer! ",0);
		if(res0>30)
			OLED_P6x8Str(75,3,"Cloth!  ",0);
		
	}

////////////////////////////////////////////////////第二步//////////////////////////////////////////////////	
if(key2==0)
		{		
		OLED_P6x8Str(67,5,"MODE:2",0);
		if(res0<3.5)	
			OLED_P6x8Str(75,3,"NUM:0",0);
		if(res0>3.5&&res0<10.8)	
			OLED_P6x8Str(75,3,"NUM:1",0);
		if(res0>10.8&&res0<17)	
			OLED_P6x8Str(75,3,"NUM:2",0);
		if(res0>17&&res0<23)	
			OLED_P6x8Str(75,3,"NUM:3",0);
		if(res0>23&&res0<31.5)	
			OLED_P6x8Str(75,3,"NUM:4",0);
		if(res0>31.5)	
			OLED_P6x8Str(75,3,"NUM:5",0);
	}
	
////////////////////////////////////////////////////第三步//////////////////////////////////////////////////	
if(key3==0)
		{		
		OLED_P6x8Str(67,5,"MODE:3",0);
		OLED_P6x8Str(75,3,"        ",0);
			
	if(key5==0)                   //学习模式
	{
		MODE=0;	
		OLED_P6x8Str(67,7,"Study! ",0);
	}
	
	if(key6==0)										//判定模式
	{
		MODE=1;	
		OLED_P6x8Str(67,7,"Wolk! ",0);
		if(flag_i==3){
		if(value[0]<value[1]) {float flag=value[0];value[0]=value[1];value[1]=flag;}
		if(value[0]<value[2]) {float flag=value[0];value[0]=value[2];value[2]=flag;}
		if(value[1]<value[2]) {float flag=value[1];value[1]=value[2];value[2]=flag;}
		//printf("value1:%f value2:%f  value3:%f \r\n",value[0],value[1],value[2]);
		if(res0>1.0&&res0<((value[1]-value[2])/2+value[2]))
			OLED_P6x8Str(75,3,"Scissor!",0);
		if(res0>((value[1]-value[2])/2+value[2])&&res0<(value[1]+(value[0]-value[1])/2))
			OLED_P6x8Str(75,3,"Hammer! ",0);
		if(res0>(value[1]+(value[0]-value[1])/2))
			OLED_P6x8Str(75,3,"Cloth!  ",0);
	}
	}
		
	}
		
///////////////////////////////////////////////////第四步/////////////////////////////////////////////////////
if(key4==0)
		{		
		OLED_P6x8Str(67,5,"MODE:4",0);
		OLED_P6x8Str(75,3,"        ",0);
if(key5==0)											//学习模式
	{
		MODE=0;	
		OLED_P6x8Str(67,7,"Study! ",0);
	}
	if(key6==0)										//判定模式
	{
		MODE=1;	
		OLED_P6x8Str(67,7,"Wolk! ",0);
		if(flag_i==5){
		for(i=0;i<4;i++)
		for(j=i+1;j<5;j++)
		if(value[i]>value[j])
		{
			float n=value[i];
			value[i]=value[j];
			value[j]=n;
		}
		//printf("value1:%f value2:%f  value3:%f value4:%f  value5:%f\r\n",value[0],value[1],value[2],value[3],value[4]);
		if(res0>1.0&&res0<(value[0]+(value[1]-value[0])/2))
		OLED_P6x8Str(75,3,"NUM:1",0);
		if(res0>(value[0]+(value[1]-value[0])/2)&&res0<(value[1]+(value[2]-value[1])/2))
		OLED_P6x8Str(75,3,"NUM:2",0);	
		if(res0>(value[1]+(value[2]-value[1])/2)&&res0<(value[2]+(value[3]-value[2])/2))
		OLED_P6x8Str(75,3,"NUM:3",0);
		if(res0>(value[2]+(value[3]-value[2])/2)&&res0<(value[3]+(value[4]-value[3])/2))
		OLED_P6x8Str(75,3,"NUM:4",0);
		if(res0>(value[3]+(value[4]-value[3])/2))
		OLED_P6x8Str(75,3,"NUM:5",0);
	}}
		
	}
		led=~led;
		
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
	}
}

void Sys_Init(void)
{
	delay_init();	 
	NVIC_Configuration();
	uart_init(115200);	 //串口波特率115200
	LED_Init();
	Key_Init();
	EXTIX_Init();
	OLED_Init();
//	OLED_P6x8Str(82,3,"Float",0);
//	OLED_P6x8Str(67,5,"Technology",0);
	while(FDC2214_Init());
	//led=0;
	delay_ms(100);
	temp0 = Cap_Calculate(0);//读取初始值
	temp1 = Cap_Calculate(1);
	temp2 = Cap_Calculate(2);
	temp3 = Cap_Calculate(3);
}

void EXTI0_IRQHandler(void)
{

	delay_ms(10);//消抖
	if(button==0)	 	 //WK_UP按键
	{	if(MODE==0){			 
		value[flag_i++]=value_;
		OLED_P6x8Str(0,5,"VALUE:",0);Oled_Printf_Float(22,5,value_,0);
		delay_ms(500);
		OLED_P6x8Str(0,5,"      ",0);OLED_P6x8Str(22,5,"       ",0);
	}}
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
}
