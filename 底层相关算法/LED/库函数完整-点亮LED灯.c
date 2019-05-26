/**
 *
 * 使用寄存器点亮LED灯PA8,另一端接VCC3.3V
 *
 **/

//片上外设基地址
#define PERIPH_BASE				((unsigned int)0x40000000)  //unsigned int,32位，int也为32位

//时钟基地址
#define AHBPERIPH_BASE        	(PERIPH_BASE + 0x20000) 
#define RCC_BASE              	(AHBPERIPH_BASE + 0x1000)
#define RCC_APB2ENR             *(unsigned int *)(RCC_BASE + 0x0018)


//GPIOA的相关寄存器配置
#define APB2PERIPH_BASE			(PERIPH_BASE + 0x10000 )
#define GPIOA_BASE				(APB2PERIPH_BASE + 0x0800  )

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

#define assert_param(expr) ((expr) ? (void)0 : assert_failed((unsigned char  *)__FILE__, __LINE__))

#define _IS_IS_GPIOx(GPIOx)  ((GPIOx == GPIOA) || \
							  (GPIOx == GPIOB) || \
							  (GPIOx == GPIOC))

//GPIO引脚配置
#define GPIO_Pin_0                 ((unsigned short int)0x0001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((unsigned short int)0x0002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((unsigned short int)0x0004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((unsigned short int)0x0008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((unsigned short int)0x0010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((unsigned short int)0x0020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((unsigned short int)0x0040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((unsigned short int)0x0080)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((unsigned short int)0x0100)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((unsigned short int)0x0200)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((unsigned short int)0x0400)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((unsigned short int)0x0800)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((unsigned short int)0x1000)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((unsigned short int)0x2000)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((unsigned short int)0x4000)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((unsigned short int)0x8000)  /*!< Pin 15 selected */
#define GPIO_Pin_All               ((unsigned short int)0xFFFF)  /*!< All pins selected */

#define _IS_GPIO_PIN(pin)			((((unsigned int)pin &0x0000) == 0) && (unsigned int)pin != 0x0000)

//限制输入模式
typedef enum
{
	GPIO_Mode_AIN = 0x0,
	GPIO_Mode_IN_FLOATING = 0x04, 
	GPIO_Mode_IPD = 0x28,         
	GPIO_Mode_IPU = 0x48,        
	GPIO_Mode_Out_OD = 0x14,      
	GPIO_Mode_Out_PP = 0x10,
	GPIO_Mode_AF_OD = 0x1C,
	GPIO_Mode_AF_PP = 0x18
	
}GPIOMode_TypeDef;

//限制输出速度
typedef enum
{
	GPIO_Speed_10MHz = 1,
	GPIO_Speed_2MHz, 
	GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;

//GPIO初始化结构体
typedef struct
{
	unsigned short GPIO_Pin;
	GPIOSpeed_TypeDef GPIO_Speed;
	 GPIOMode_TypeDef GPIO_Mode; 
}GPIO_InitTypeDef;

//unsigned short 
/*
 0000 0000
*/


void GPIO_ResetBit(GPIO_TypeDef* GPIOx, unsigned short GPIO_Pin);
void GPIO_SetBit(GPIO_TypeDef* GPIOx, unsigned short GPIO_Pin);
void GPIOA_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);


int main(void)
{
	GPIO_InitTypeDef GPIO_InitTypeStructure;
	
	//GPIOA的开启时钟
	RCC_APB2ENR &= ~(1 << 2); 
	RCC_APB2ENR |= (1 << 2);  
	
	GPIO_InitTypeStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitTypeStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitTypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOA_Init(GPIOA,&GPIO_InitTypeStructure);
	
	while(1)
	{
		
		GPIO_ResetBit(GPIOA,GPIO_Pin_8);
	}
	return 0;
}

void GPIO_ResetBit(GPIO_TypeDef* GPIOx, unsigned short GPIO_Pin)
{
	//assert_param(_IS_GPIO_PIN(GPIO_Pin));
	GPIOx->BRR = GPIO_Pin;
}	

void GPIO_SetBit(GPIO_TypeDef* GPIOx, unsigned short GPIO_Pin)
{
	//assert_param(_IS_GPIO_PIN(GPIO_Pin));
	GPIOx->BSRR = GPIO_Pin;
}	

void GPIOA_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  unsigned int currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  unsigned int tmpreg = 0x00, pinmask = 0x00;
  /* 函数的形参检测，assert_param()只是提供了出错时的函数，但没有实现各个部分参数判断，最重要的是将函数形参的正误用1或者0来表示 */
  /*使用宏定义和逻辑或操作实现参数的正误判断*/
  //assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  /*对于配置过程中有限参数可以使用枚举作一下限制*/
  //assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  /*这个判断思路值的借鉴，使用短路与操作*/
 // assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));  
  
/*---------------------------- GPIO Mode Configuration -----------------------*/
	//得到0~3位，配置CRL/CRH的CNF 和NODE 这4位，主要配置输入/出模式
  currentmode = ((unsigned int)GPIO_InitStruct->GPIO_Mode) & ((unsigned int)0x0F);
	//得到第4位，0表示输入，1表示输出，如果是输出，则需要加上输出速度设置
  if ((((unsigned int)GPIO_InitStruct->GPIO_Mode) & ((unsigned int)0x10)) != 0x00)
  { 
    /* Check the parameters */
   // assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
    /* Output mode */
    currentmode |= (unsigned int)GPIO_InitStruct->GPIO_Speed;
  }
  
  
/*---------------------------- GPIO CRL Configuration ------------------------*/
  /* 配置低8脚，使用mask得到低8位 */
  if (((unsigned int)GPIO_InitStruct->GPIO_Pin & ((unsigned int)0x00FF)) != 0x00)
  {
	 //tmpreg 记录当前的CRL的状态
    tmpreg = GPIOx->CRL;
	
	//循环从pin0开始，找出对应的pin
    for (pinpos = 0x00; pinpos < 0x08; pinpos++) //运行8次，CRL只管低8位
    {
      pos = ((unsigned int)0x01) << pinpos;       // 1000 000
      /* Get the port pins position */
      currentpin = (GPIO_InitStruct->GPIO_Pin) & pos; //得到当前的pin的16进制值                                                                                                                                                                                                                                                                                                                                                                                              
		
	  //表示找到当前引脚
      if (currentpin == pos)
      {
        pos = pinpos << 2;  //7*4 = 28 ,pinpos才是真正的引脚，左移2位为乘4操作，因为寄存器属于4位控制
        /* 清空相应的位*/
        pinmask = ((unsigned int)0x0F) << pos; 
        tmpreg &= ~pinmask;
		  
        /* 将tepreg赋值 */
        tmpreg |= (currentmode << pos);
        /* Reset the corresponding ODR bit */
		  
		
		//值的借鉴，使用if...else结构，实现两个精确的判断条件
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((unsigned int)0x01) << pinpos);
        }
        else
        {
          /* Set the corresponding ODR bit */
          if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
          {
            GPIOx->BSRR = (((unsigned int)0x01) << pinpos);
          }
        }
		
		
      }
    }
	//写入
    GPIOx->CRL = tmpreg;
  }
  

/*---------------------------- GPIO CRH Configuration ------------------------*/
  /* 配置高8位 */
  if (GPIO_InitStruct->GPIO_Pin > 0x00FF)
  {
    tmpreg = GPIOx->CRH;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = (((unsigned int)0x01) << (pinpos + 0x08));
      /* Get the port pins position */
      currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        pinmask = ((unsigned int)0x0F) << pos;
        tmpreg &= ~pinmask;
        tmpreg |= (currentmode << pos);
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((unsigned int)0x01) << (pinpos + 0x08));
        }
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
        {
          GPIOx->BSRR = (((unsigned int)0x01) << (pinpos + 0x08));
        }
      }
    }
    GPIOx->CRH = tmpreg;
  }
}
