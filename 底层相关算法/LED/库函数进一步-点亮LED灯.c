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

#define GPIOA 		((GPIO_TypeDef *)GPIOA_BASE)

#define GPIOA_Pin_8	(unsigned short)0x0100
	
typedef struct {
	unsigned int GPIO_Mode;
	unsigned int GPIO_Speed;
	
}GPIO_InitTypeDef;


void GPIO_ResetBit(GPIO_TypeDef* GPIOx, unsigned short GPIO_Pin);
void GPIO_SetBit(GPIO_TypeDef* GPIOx, unsigned short GPIO_Pin);


int main(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStructure;
	
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
	
	while(1)
	{
		
		GPIO_ResetBit(GPIOA,GPIOA_Pin_8);
	}
	return 0;
}

void GPIO_ResetBit(GPIO_TypeDef* GPIOx, unsigned short GPIO_Pin)
{
    //对参数进行assert
	GPIOx->BRR = GPIO_Pin;
}	

void GPIO_SetBit(GPIO_TypeDef* GPIOx, unsigned short GPIO_Pin)
{
    //对参数进行assert
	GPIOx->BSRR = GPIO_Pin;
}	
