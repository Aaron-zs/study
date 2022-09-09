#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
static uint16_t key4_count;				//记录key4按键被按下，持续低电平的时间
static uint8_t key4_start_flag;			//key4按键第一次按下，置1，开始计时
static uint8_t key4_valid_flag;			//key4按键有效按下，置1，只用一次
static uint8_t key_state;				//记录上一次的按键状态
static uint8_t key_trg_falling;			//下降沿信号
static uint8_t key_trg_rising;			//上升沿信号
static uint8_t key4_trg_falling;			//下降沿信号
static uint8_t key4_trg_rising;			//上升沿信号
static uint8_t short_single_flag;		//单短击
static uint8_t long_single_flag;		//长短击
static uint8_t key4_press_cnt;			//key4_count被按下的次数，用于多击
static uint16_t key4_double_time;		//key4按键的多击间隔

static uint8_t key1_count;				//记录key4按键被按下，持续低电平的时间
static uint8_t key1_start_flag;			//key4按键第一次按下，置1，开始计时
static uint8_t key1_valid_flag;			//key4按键有效按下，置1，只用一次
static uint8_t key1_continue_count;		//key4按键连击按下，持续低电平的时间

void myKey_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t myKey_GetNum(void)
{
	uint8_t key_Num = 0;	
	
	uint8_t key_value = 0xF8;
	key_value |= GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) | \
				(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) << 1) | \
				(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) << 2);
	key_value = key_value ^ 0xFF;
	
	key_trg_falling = key_value & (key_value ^ key_state);
	key_trg_rising = (!key_value) * (key_value ^ key_state);
	key_state = key_value;
		
	if(key4_trg_falling == 0x04)
	{
		
		key4_start_flag = 1;
		key4_count = 0;
		short_single_flag = 0;		
		long_single_flag = 0;	
	}
	else
	{
		if(key4_trg_rising == 0x04)
		{
			key4_start_flag = 0;
			key4_valid_flag = 0;
			key4_count = 0;
			key4_double_time = 0;
		}
		else
		{
			if(key4_start_flag)
			{
				key4_count += 2;
				if(key4_count >= 10)
				{
					if(0 == key4_valid_flag)
					{
						short_single_flag = 1;
						long_single_flag = 0;
						key4_valid_flag = 1;
						key4_press_cnt ++;
					}
					else if(key4_count >= 500)
					{
						short_single_flag = 0;
						long_single_flag = 1;
					}
					else
					{
										//直接返回
					}				
				}
				else
				{
										//直接返回
				}
			}
			else if(key4_press_cnt > 0)
			{
				key4_double_time += 2;
				if(key4_double_time >= 300)
				{
					if(key4_press_cnt == 1)
					{
						if(short_single_flag)
						{
							if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET)
							{
								key_Num = 100;
							}
							else
							{
								key_Num = 1;
							}
							short_single_flag = 0;
						}
						else
						{
							key_Num = 5;
							long_single_flag = 0;
						}
					}
					else if(key4_press_cnt == 2)
					{
						key_Num = 11;
						short_single_flag = 0;
						long_single_flag = 0;
					}
					else
					{
						key_Num = 13;
						short_single_flag = 0;
						long_single_flag = 0;
					}
					key4_press_cnt = 0;
					key4_double_time = 0;
					
				}
				else
				{
								//直接返回
				}
			}
			else
			{
								//直接返回
			}
		}
	}
	return key_Num;
}

uint8_t myKey_GetNum2(void)
{
	uint8_t key_Num = 0;	
	
	uint8_t key_value = 0xF8;
	key_value |= GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) | \
				(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) << 1) | \
				(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) << 2);
	key_value = key_value ^ 0xFF;
	
	key_trg_falling = key_value & (key_value ^ key_state);
	key_trg_rising = (!key_value) * (key_value ^ key_state);
	
	key4_trg_falling = key_value & (key_value ^ key_state);
	key4_trg_rising = (!key_value) * (key_value ^ key_state);
	key_state = key_value;
	
	if(key_trg_falling == 0x01)
	{
		
		key1_start_flag = 1;
		key1_count = 0;	
//		Serial_SendNumber(key_value, 2);
	}
	else
	{
		if(key_trg_rising == 0x01)
		{
			key1_start_flag = 0;
			key1_valid_flag = 0;
			key1_count = 0;
			key1_continue_count = 0;
		}
		else
		{
			if(key1_start_flag)
			{
				if(0 == key1_valid_flag)
				{
					key1_count += 2;
					if(key1_count >= 10)
					{
						key1_count += 2;
						key_Num = 2;
						key1_valid_flag = 1;
						key1_continue_count = 0;
					}
					else
					{
						//直接返回
					}
				}
				
				else
				{
					key1_continue_count += 2;	
					if(key1_continue_count >= 200)
					{
						key_Num = 2;
						key1_continue_count = 0;
					}
					else
					{
								//直接返回
					}
				}
			}
			else
			{
						//直接返回
			}
		}
	}
	
	
	if(key4_trg_falling == 0x04)
	{
		
		key4_start_flag = 1;
		key4_count = 0;
		short_single_flag = 0;		
		long_single_flag = 0;
//		Serial_SendNumber(key_value, 2);		
	}
	else
	{
		if(key4_trg_rising == 0x04)
		{
			key4_start_flag = 0;
			key4_valid_flag = 0;
			key4_count = 0;
			key4_double_time = 0;
		}
		else
		{
			if(key4_start_flag)
			{
				key4_count += 2;
				if(key4_count >= 10)
				{
					if(0 == key4_valid_flag)
					{
						short_single_flag = 1;
						long_single_flag = 0;
						key4_valid_flag = 1;
						key4_press_cnt ++;
					}
					else if(key4_count >= 500)
					{
						short_single_flag = 0;
						long_single_flag = 1;
					}
					else
					{
										//直接返回
					}				
				}
				else
				{
										//直接返回
				}
			}
			else if(key4_press_cnt > 0)
			{
				key4_double_time += 2;
				if(key4_double_time >= 300)
				{
					if(key4_press_cnt == 1)
					{
						if(short_single_flag)
						{
							
							Serial_SendNumber(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1), 1);
							if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1))
							{
								key_Num = 1;
//								Serial_SendNumber(key_Num, 3);
							}
							else
							{
								key_Num = 106;
							}
//							key_Num = 1;
							short_single_flag = 0;
						}
						else
						{
							key_Num = 5;
							long_single_flag = 0;
						}
					}
					else if(key4_press_cnt == 2)
					{
						key_Num = 11;
						short_single_flag = 0;
						long_single_flag = 0;
					}
					else
					{
						key_Num = 13;
						short_single_flag = 0;
						long_single_flag = 0;
					}
					key4_press_cnt = 0;
					key4_double_time = 0;
					
				}
				else
				{
								//直接返回
				}
			}
			else
			{
								//直接返回
			}
		}
	}
	return key_Num;
}
