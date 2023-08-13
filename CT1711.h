#ifndef __ct1711_H
#define __ct1711_H

#define TIM_delay_us        &htim3//宏定义延时定时器    1us
#define TIM_delay_150ms        &htim2//宏定义延时定时器 150ms
#define Sensor_resolution    0.00390625

void delay_us(unsigned short int us);
void ct1711_init(void);
float getTemp(void);
unsigned long read_ct1711_data(void);
void StartgetTemp(void);
#endif 

