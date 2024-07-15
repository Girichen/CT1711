#include "gpio.h"
#include "tim.h"
#include "CT1711.h"

float temp = 0;
unsigned long data = 0;

void delay_us(unsigned short int us)  //延时函数，TIM3 1MHz 1us
{
	__HAL_TIM_SET_COUNTER(TIM_delay_us, 0);//初始化TIM3
	__HAL_TIM_ENABLE(TIM_delay_us);//使能TIM3
	while (__HAL_TIM_GET_COUNTER(TIM_delay_us) < us) //延时
	{
		
	}
	__HAL_TIM_DISABLE(TIM_delay_us);//关闭TIM3
}


void ct1711_init(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);//拉低总线
	delay_us(400);//延时400us
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);//拉高总线,开漏输出置位高阻态读取引脚数据
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
	ct1711_init();//CT1711初始化
	__HAL_TIM_SET_COUNTER(TIM_delay_150ms, 0); //定时器清0
	HAL_TIM_Base_Start_IT(TIM_delay_150ms);//开始计时
}


float getTemp(void) //获取温度
{
	HAL_TIM_Base_Stop_IT(TIM_delay_150ms); //清空定时器
	unsigned long temp_data = 0;   //定义局部变量温度数据位
	unsigned long temp_vaule = 0;  //定义局部变量温度数值
	temp_data =  read_ct1711_data();//读取CT1711的19bit数据
	temp_vaule = temp_data & 0x0000FFFF; //提取17bit数据
	
	if((temp_data & 0x00060000) == 0x00000000) //判断数据是否有效
	{
		if ((temp_data & 0x00010000) == 0x00000000)  //判断数据正负
			temp =  temp_vaule  * Sensor_resolution;
		else
			temp =  -temp_vaule * Sensor_resolution;
	}
	return temp;
}

	
	
