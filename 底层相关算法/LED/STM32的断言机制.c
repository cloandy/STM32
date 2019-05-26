//头文件stm32f10x_conf.h
//未用
/* #define USE_FULL_ASSERT    1 */

#if USE_FULL_ASSERT

#defne assert_param(expr)  ((expr)?(void)0:assert_failed((uint8_t *)__FILE__,__LINE__))
void assert_failed((uint8_t *)__FILE__,__LINE__); //头文件声明,函数主体可以在main.c中定义，使用while循环
#else

#define assert_param(expr)  (void)0 //空语句

#endif

//头文件stm32f10x.h
#if !defined  USE_STDPERIPH_DRIVER
 /*#define USE_STDPERIPH_DRIVER*/
#endif

#ifdef USE_STDPERIPH_DRIVER
  #include "stm32f10x_conf.h"
#endif

//这个宏的定义可以在两个地方，一个是将注释去掉，一个是在魔法棒选项卡的C/C++的defination框内
//只有定义了USE_STDPERIPH_DRIVER才能包含调试配置头文件，只有定义USE_FULL_ASSERT = 1 才能正式启动调试参数