#include "gpio.h"
#include "tim.h"
#include "CT1711.h"

float temp = 0;
unsigned long data = 0;

void delay_us(unsigned short int us)  //��ʱ������TIM3 1MHz 1us
{
	__HAL_TIM_SET_COUNTER(TIM_delay_us, 0);//��ʼ��TIM3
	__HAL_TIM_ENABLE(TIM_delay_us);//ʹ��TIM3
	while (__HAL_TIM_GET_COUNTER(TIM_delay_us) < us) //��ʱ
	{
		
	}
	__HAL_TIM_DISABLE(TIM_delay_us);//�ر�TIM3
}


void ct1711_init(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);//��������
delay_us(400);//��ʱ400us
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);//��������,��©�����λ����̬��ȡ��������
}

unsigned long read_ct1711_data(void)
{
  data = 0;
  
	for(int i =0;i<19;i++)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
		delay_us(16);
	
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5) == GPIO_PIN_SET)
    {
		    data<<=1;
		    data+=1;
		}
	 else{
	      data<<=1;
	  }
	 
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
		delay_us(34);
	}
	
	return data;
}


void StartgetTemp(void)
{
    ct1711_init();//CT1711��ʼ��
	  __HAL_TIM_SET_COUNTER(TIM_delay_150ms, 0); //��ʱ����0
	  HAL_TIM_Base_Start_IT(TIM_delay_150ms);//��ʼ��ʱ
}


float getTemp(void) //��ȡ�¶�
{
	HAL_TIM_Base_Stop_IT(TIM_delay_150ms); //��ն�ʱ��
	
unsigned long temp_data = 0;   //����ֲ������¶�����λ
unsigned long temp_vaule = 0;  //����ֲ������¶���ֵ
temp_data =  read_ct1711_data();//��ȡCT1711��19bit����
temp_vaule = temp_data & 0x0000FFFF; //��ȡ17bit����
	
	if((temp_data & 0x00060000) == 0x00000000) //�ж������Ƿ���Ч
		{
	   if ((temp_data & 0x00010000) == 0x00000000)  //�ж���������
			      temp =  temp_vaule  * Sensor_resolution;
		    else
					  temp =  -temp_vaule * Sensor_resolution;
			}

return temp;
}

	
	