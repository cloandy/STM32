```C
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


```

# 位带操作

- 类似于51的位操作`sbit key = P1^1;`可以直接使用`key`进行一个比特位的读写

- 在STM32中有两个地方实现了位带操作：

  - `SRAM`空间的最低1MB的地方
  - 外设区的最低1MB的地方
  - 位带区可膨胀，其膨胀原理可能和`GPIO_Init`函数的一个部分相似，将其1MB的空间的每一个为膨胀为一个32位的字

- 外设位带区地址：`0x4000 0000 ~ 0x4010 0000` 大小为1MB，包含全部外设

  - 指向的未带别名区地址为`0x4200 0000~ 0x43ff ffff`

- SRAM 的位带区的地址为：` 0X2000 0000~X2010 0000`大小为 1MB

  - 位 带别名区地址为： 0X2200 0000~0X23FF FFFF，大小为 32MB

- 位带区和位带别名区的地址转换

  ```C
  AliasAddr = 0x42000000 +(A - 0x40000000)*8*4 + n*4
           /*位带别名区地址   该比特前面有多少个位，一个字节8位，一个位膨胀为4个字节 */  
  ```

  

