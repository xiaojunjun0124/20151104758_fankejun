/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h"
#include "dht11.h"


#define DHT_SetBit()	GPIO_SetBits(DHT_GPIO, DHT_GPIO_PIN)
#define DHT_ResetBit()	GPIO_ResetBits(DHT_GPIO, DHT_GPIO_PIN)
#define DHT_ReadBit()	GPIO_ReadInputDataBit(DHT_GPIO, DHT_GPIO_PIN)

static void DHT_Set_Output(void);			//����Ϊ���ģʽ
static void DHT_Set_Input(void);			//����Ϊ����ģʽ

DHT11_TypeDef DHT11;						//ȫ�ֱ���

void DHT_Set_Output(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(DHT_GPIO, &GPIO_InitStructure);					 					
}	

void DHT_Set_Input(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(DHT_GPIO, &GPIO_InitStructure);					 
}

/**
  * @brief  ��ʼ��IO�ںͲ���
  * @param  none.
  * @retval none.
  */
void DHT11_Init(void)
{
	RCC_APB2PeriphClockCmd(DHT_RCC, ENABLE);
	
	DHT_Set_Output();
		
	DHT11.Tem_H = 0;
	DHT11.Tem_L = 0;
	DHT11.Hum_H = 0;
	DHT11.Hum_L = 0;
}

/**
  * @brief  ��ȡ8bit ����
  * @param  none.
  * @retval none.
  */
int DH21_ReadByte(void)
{
	int data=0;
	char i;
	char cout;
	
	for(i=0; i<8; i++)
	{
		//��ȡ50us�ĵ͵�ƽ
		cout=1;
		while(!DHT_ReadBit() && cout++);
		
		//��ʱ30us֮���ȡIO�ڵ�״̬
		delay_us(30);
		
		//�Ȱ��ϴε�������λ���ٱ��汾�ε�����λ
		data = data << 1;
		
		if(DHT_ReadBit() == Bit_SET)
		{	
			data |= 1;
		}		
		
		//�ȴ�������ǵ͵�ƽ��������һλ���ݽ���
		cout=1;
		while(DHT_ReadBit() && cout++);
	}

	return data;
}

/**
  * @brief  ��ȡ40bit����
  * @param  none.
  * @retval 1 ��ȡ�ɹ���0��ȡʧ��.
  */
int DHT11_ReadData(void)
{
	unsigned int cout = 1;
	unsigned int T_H, T_L, H_H, H_L, Check;

	//����ΪIO�����ģʽ
	DHT_Set_Output();
	
	//1��MCU��ʼ��ʼ�ź�
	DHT_ResetBit();
	delay_ms(25);		//��������18ms
	DHT_SetBit();		
	delay_us(20);		//����20~40us
	
	//����ΪIO������ģʽ
	DHT_Set_Input();
	
	//2����ȡDH21��Ӧ
	if(DHT_ReadBit() == Bit_RESET)
	{
		//�ȴ�80us�ĵ͵�ƽ
		cout = 1;
		while(!DHT_ReadBit() && cout++);
		
		//�ȴ�80us�ĸߵ�ƽ
		cout = 1;
		while(DHT_ReadBit() && cout++);
		
		//��ȡ8bit��ʪ����������
		H_H = DH21_ReadByte();
		
		//��ȡ8bit��ʪ��С������
		H_L = DH21_ReadByte();
		
		//��ȡ8bit���¶���������
		T_H = DH21_ReadByte();
		
		//��ȡ8bit���¶�С������
		T_L = DH21_ReadByte();
		
		//��ȡ8bit��У���
		Check = DH21_ReadByte();
		
		if(Check == (H_H + H_L + T_H + T_L))
		{
			DHT11.Hum_H = H_H;
			DHT11.Hum_L = H_L;
			DHT11.Tem_H = T_H;
			DHT11.Tem_L = T_L;	
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

/**
  * @brief  ��ȡ�¶�
  * @param  none.
  * @retval Temp, �¶�ֵ
  */
int DHT11_GetTem(void)
{
	return (DHT11.Tem_H << 8 | DHT11.Tem_L);
}

/**
  * @brief  ��ȡʪ��
  * @param  none.
  * @retval Hum,ʪ��ֵ
  */
int DHT11_GetHum(void)
{
	return (DHT11.Hum_H << 8 | DHT11.Hum_L);
}



