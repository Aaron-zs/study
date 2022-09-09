#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
//#include "Key.h"
#include "myKey.h"
#include "Serial.h"
uint16_t key;
uint16_t show_Num;
uint8_t cnt0;
uint8_t flag;
int main(void)
{
	OLED_Init();
	Timer_Init();
	myKey_Init();
	Serial_Init();
	while (1)
	{
		if(!flag)
		{
			cnt0++;
		}
		else
		{
			cnt0--;
		}
		
		if(cnt0 == 100)
		{
			flag = 1;
			GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
		}
		if(0 == cnt0)
		{
			flag = 0;
			GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
		}
		switch (key)
		{
			case 1:
				key = 0;
				show_Num ++;
				break;
			
			case 2:
				key = 0;
				show_Num --;
				break;
			
			case 3:
				key = 0;
				show_Num = 88;
				break;
			
			case 5:
				key = 0;
				show_Num += 20;
				break;
			
			case 11:
				key = 0;
				show_Num = 11;
				break;
			
			case 13:
				key = 0;
				show_Num = 333;
				break;
			case 100:
				key = 0;
				show_Num = 666;
				break;
			case 106:
				key = 0;
				show_Num = 999;
				break;
			default:
				
				break;
		}
		OLED_ShowNum(1, 1, show_Num, 4);

	}
}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		key = myKey_GetNum2();
//		Serial_SendNumber(key, 2);
//		Serial_SendString("\r\n");
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
