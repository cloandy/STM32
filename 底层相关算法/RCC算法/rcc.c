/*
1. 配置HSE并且等待HSE启动稳定
2. 配置FLASH相关寄存器
3. 设置AHB,APB1,APB2的分频因子
4. 设置PLL倍频因子，官方推荐为72MHz
5. 等待PLL稳定，确保PLL为系统时钟源
*/
#define  RCC_CR_HSEON                        ((uint32_t)0x00010000)//16bit
#define  RCC_CR_HSERDY                       ((uint32_t)0x00020000) //17bit
#define HSE_STARTUP_TIMEOUT   ((uint16_t)0x0500) //out of time
#define  RCC_CFGR_HPRE_DIV1                  ((uint32_t)0x00000000)  //系统时钟不分频
#define  RCC_CFGR_PPRE2_DIV1                 ((uint32_t)0x00000000) //HCLK不分频



static void SetSysClockTo72(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/    
  /* Enable HSE */    
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  /*等待HSE时钟准备好，或者超时退出*/
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;  
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }  
/*HSE启动成功，则继续*/
  if (HSEStatus == (uint32_t)0x01)
  {
  /*-----------可以原样照着写-----------*/
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;    
/*--------------------------------*/
 
    /* HCLK = SYSCLK，设置分频因子，使用宏定义*/
    /*系统时钟不分频*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK APB2不分频 */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK  APB1 2分频*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

    /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz  9倍频*/
      /*先清零在赋值*/
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
                                        RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    
    /* Select PLL as system clock source */
    /*先清零在赋值*/
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* 读取时钟切换状态位，确保PLLCLK被选为系统时钟 */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock 
         configuration. User can add here some code to deal with this error */
         /*HSE启动失败，添加错误代码*/
  }
}