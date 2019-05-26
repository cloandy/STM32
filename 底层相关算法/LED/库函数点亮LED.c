#include "led.h"

//宏定义实现LED灯的亮灭

#define DS0_ON			GPIOA->BRR = GPIO_Pin_8
#define DS0_OFF			GPIOA->BSRR = GPIO_Pin_8
#define DS0_REVERSE		GPIOA->ODR = ^(GPIOA->ODR)


//位带操作实现LED的亮灭
#define DS0ON			PAout(8) = 0
#define DS0OFF			PAout(8) = 1

int main(void)
{
	LED_Init();
	while(1)
	{
		DS0ON;
	}
	return 0;
	
}

