/**
 *
 * 使用寄存器点亮LED灯PA8,另一端接VCC3.3V
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
#define GPIOA_CRL			*(unsigned int *)(GPIOA_BASE  + 0x0000  )
#define GPIOA_CRH			*(unsigned int *)(GPIOA_BASE  + 0x0004  )
#define GPIOA_IDR			*(unsigned int *)(GPIOA_BASE  + 0x0008  )
#define GPIOA_ODR			*(unsigned int *)(GPIOA_BASE  + 0x000C  )
#define GPIOA_BSRR			*(unsigned int *)(GPIOA_BASE  + 0x0010  )
#define GPIOA_BRR			*(unsigned int *)(GPIOA_BASE  + 0x0014  )
#define GPIOA_LCKR			*(unsigned int *)(GPIOA_BASE  + 0x0018  )

int main(void)
{
	//GPIOA的开启时钟
	RCC_APB2ENR &= ~(1 << 2); 
	RCC_APB2ENR |= (1 << 2);  
	
	//GPIOA配置为输出模式，最大速度为50Mhz
	//清空
	GPIOA_CRH   &= ~(3 << (2 * 0));   
	//赋值
	GPIOA_CRH   |= (3 << (2 * 0));   
	
	//GPIOA的模式选择：0x00 为通用推挽输出
	//清空
	GPIOA_CRH   &= ~(3 << (2 * 1)); 
	//赋值
	GPIOA_CRH   |= (00 << (2 * 1));  
	
	//设置PGIOA的输出电平
	
	
	//清空
	GPIOA_ODR &= ~(0 << 8);
	while(1)
	{
		
		GPIOA_ODR |= (1 << 8);
	}
	return 0;
}