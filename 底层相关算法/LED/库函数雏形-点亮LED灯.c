/**
 *
 * 使用库函数雏形点亮LED灯PA8,另一端接VCC3.3V
 *
 **/

//片上外设基地址
#define PERIPH_BASE			((unsigned int)0x40000000)  //unsigned int,32位，int也为32位


#define AHBPERIPH_BASE        	(PERIPH_BASE + 0x20000) 
#define RCC_BASE              	(AHBPERIPH_BASE + 0x1000)
#define RCC_APB2ENR             *(unsigned int *)(RCC_BASE + 0x0018)


//GPIOA的相关寄存器配置
#define APB2PERIPH_BASE		(PERIPH_BASE   + 0x10000 )
#define GPIOA_BASE			(APB2PERIPH_BASE     + 0x0800  )

typedef struct
{
  volatile unsigned int CRL;
  volatile unsigned int CRH;
  volatile unsigned int IDR;
  volatile unsigned int ODR;
  volatile unsigned int BSRR;
  volatile unsigned int BRR;
  volatile unsigned int LCKR;
} GPIO_TypeDef;
//这里使用了结构体的字节对齐
//使得结构体地址等于GPIOA外设的基地址
//结构体的成员等于寄存器地址
//使得寄存器开发过程中的基地址+偏移地址可以使用结构体基地址+结构体成员地址来访问
//我们只需要知道外设的基地址，就可以操作所有的相关的外设寄存器
#define GPIOA 		((GPIO_TypeDef *)GPIOA_BASE)


int main(void)
{
	
	//GPIOA的开启时钟
	RCC_APB2ENR &= ~(1 << 2); 
	RCC_APB2ENR |= (1 << 2);  
	
	//GPIOA配置为输出模式，最大速度为50Mhz
	//清空
	GPIOA->CRH   &= ~(3 << (2 * 0));   
	//赋值
	GPIOA->CRH   |= (3 << (2 * 0));   
	
	//GPIOA的模式选择：0x00 为通用推挽输出
	//清空
	GPIOA->CRH   &= ~(3 << (2 * 1)); 
	//赋值
	GPIOA->CRH   |= (00 << (2 * 1));  
	
	//设置PGIOA的输出电平
	
	
	//清空
	GPIOA->ODR &= ~(0 << 8);
	while(1)
	{
		
		GPIOA->ODR |= (0 << 8);
	}
	return 0;
}
