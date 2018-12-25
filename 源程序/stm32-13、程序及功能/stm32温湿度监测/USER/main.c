#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include <stdio.h>
#include "timer.h"
#include "lcd1602.h"
#include "dht11.h"

//ALIENTEK Mini STM32开发板范例代码15
//ADC实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司

char dis0[16];	//1602显示第一行
char dis1[16];

int main(void)
 { 
	delay_init();	    	 //延时函数初始化	  
	uart_init(9600);	 	//串口初始化为9600
  TIM3_Int_Init(499,7199);//50ms  	 
	LED_Init();		  		//初始化与LED连接的硬件接口
	 
// 	Adc_Init();		  		//ADC初始化	    

	Lcd_GPIO_init();  //1602接口初始化
	Lcd_Init();	
	fengshan = 0;	 //上电风扇转一下
	delay_ms(200);

	sprintf(dis0,"Temp:%02d C DH:%02d%%",(u16)DHT11.Tem_H,(u16)DHT11.Hum_H);//打印温湿度
	Lcd_Puts(0,1,(unsigned char *)dis0);	//初始化显示 
	Lcd_1Put(7,1,0xdf);
	sprintf(dis1,"My designer !   ");//打印电流
	Lcd_Puts(0,0,(u8 *)dis1);//显示电流
	fengshan = 1;
	 while(1)
	{
			if(DHT11_ReadData())	//是否读取到温湿度
			{	   
				sprintf(dis0,"Temp:%02d C DH:%02d%%",(u16)DHT11.Tem_H,(u16)DHT11.Hum_H);//打印温湿度
	   		Lcd_Puts(0,1,(unsigned char *)dis0);	//显示
				Lcd_1Put(7,1,0xdf);
				printf(dis0);//串口打印数据
				printf("\n");
				if(DHT11.Hum_H>75)			  //对比湿度值
				{
					printf("Shidu High\n");//发送湿度过高
				}
				else
				{
					printf("Shidu Normal\n");//湿度正常
				}
					delay_ms(800);//风扇控制在串口中断中处理
		}
		
	}											    
}	
