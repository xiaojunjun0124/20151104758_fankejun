#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include <stdio.h>
#include "timer.h"
#include "lcd1602.h"
#include "dht11.h"

//ALIENTEK Mini STM32�����巶������15
//ADCʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾

char dis0[16];	//1602��ʾ��һ��
char dis1[16];

int main(void)
 { 
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
  TIM3_Int_Init(499,7199);//50ms  	 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	 
// 	Adc_Init();		  		//ADC��ʼ��	    

	Lcd_GPIO_init();  //1602�ӿڳ�ʼ��
	Lcd_Init();	
	fengshan = 0;	 //�ϵ����תһ��
	delay_ms(200);

	sprintf(dis0,"Temp:%02d C DH:%02d%%",(u16)DHT11.Tem_H,(u16)DHT11.Hum_H);//��ӡ��ʪ��
	Lcd_Puts(0,1,(unsigned char *)dis0);	//��ʼ����ʾ 
	Lcd_1Put(7,1,0xdf);
	sprintf(dis1,"My designer !   ");//��ӡ����
	Lcd_Puts(0,0,(u8 *)dis1);//��ʾ����
	fengshan = 1;
	 while(1)
	{
			if(DHT11_ReadData())	//�Ƿ��ȡ����ʪ��
			{	   
				sprintf(dis0,"Temp:%02d C DH:%02d%%",(u16)DHT11.Tem_H,(u16)DHT11.Hum_H);//��ӡ��ʪ��
	   		Lcd_Puts(0,1,(unsigned char *)dis0);	//��ʾ
				Lcd_1Put(7,1,0xdf);
				printf(dis0);//���ڴ�ӡ����
				printf("\n");
				if(DHT11.Hum_H>75)			  //�Ա�ʪ��ֵ
				{
					printf("Shidu High\n");//����ʪ�ȹ���
				}
				else
				{
					printf("Shidu Normal\n");//ʪ������
				}
					delay_ms(800);//���ȿ����ڴ����ж��д���
		}
		
	}											    
}	
