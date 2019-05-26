# STM32系列单片机的assert_param()机制

- 在每一个库函数的源码实现中，都会有函数参数的断言机制

- 断言函数的宏定义原型：

  ```c
  //函数宏定义头文件stm32f10x_conf.h
  #ifdef  USE_FULL_ASSERT
    #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
  
    void assert_failed(uint8_t* file, uint32_t line);
  #else
    #define assert_param(expr) ((void)0)
  #endif /* USE_FULL_ASSERT */
  
  #endif /* __STM32F10x_CONF_H */
  ```

  - 若是没有定义`USE_FULL_ASSERT`，我们调用`assert_param`时，不对参数的正确性做任何检查，进而执行语句`((void)0)`,这条语句相当于一条空语句的表达式，不对程序产生任何影响

  - 若是定义`USE_FULL_ASSERT`，我们调用`assert_param`时，对参数的正确性做检查，进而执行语句`assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))`,正确性检查正确，则执行空语句表达式`((void)0);`;正确性检查错误，则执行语句`assert_failed((uint8_t *)__FILE__, __LINE__))`

  - `void assert_failed(uint8_t* file, uint32_t line)`在库函数中有解释，在这里的参数`__FILE__`和`__LINE__`是标准库函数中的宏定义

  - `void assert_failed(uint8_t* file, uint32_t line); `在这里属于函数在头文件中的声明

  - 函数实体在官方模板中`main.c`中找见：

    ```c
    void assert_failed(u8* file, u32 line) 
    {   
    	while (1)
        { 
        }
    }
    ```

    - 通过输入参数来确定位置，最简单的方法就是串口打印了，这个函数的主要思想是在输入参数有问题的时候
    - 在编译时可以帮你检查参数的有效性

- `assert_param()` 如何进行工程的包含

  - 在魔术棒选项的C/C++选项卡的`Define`处有这样的宏定义`USE_STDPERIPH_DRIVER(使用标准外设库)`

  - 在`stm32f10x.h`的文件中有这样的定义：

    ```c
    #if !defined  USE_STDPERIPH_DRIVER
    /**
     * @brief Comment the line below if you will not use the peripherals drivers.
       In this case, these drivers will not be included and the application code will 
       be based on direct access to peripherals registers 
       */
      /*#define USE_STDPERIPH_DRIVER*/
    #endif
    
    //如果没有定义 USE_STDPERIPH_DRIVER 这个宏
    //定义  USE_STDPERIPH_DRIVER 这个宏
    //这里加注释的原因是在魔术棒选项卡中已经有了这个宏定义
    //若没有在魔术棒选项中添加宏，也可以直接把这个注释去掉
    ```

  - 还有这样一个部分：

    ```c
    #ifdef USE_STDPERIPH_DRIVER
      #include "stm32f10x_conf.h"
    #endif
    //如果定义了USE_STDPERIPH_DRIVER 宏，则包含stm32f10x_conf.h这个文件
    //明白在编写库程序时，添加stm32f10x.h 就可以很容易的包含所有的文件夹
    ```

  - 这项软件技术可以在调试阶段帮助程序员快速地排除那些明显的错误。 

  - 它确实在程序的运行上牺牲了效率(但只是在调试阶段)，但在项目的开发上却帮助你提高了效率。  

  - 当你的项目开发成功，使用release模式编译之后，或在stm32f10x_conf.h文件中注释掉对USE_FULL_ASSERT的宏定义， 所有的assert_param()检验都消失了，不会影响最终程序的运行效率 

  - 函数使用模板：

    ```c
    void assert_failed(uint8_t* file, uint32_t line){
    
    while (1){}}
    //你可以按照自己使用的环境需求，添加适当的语句输出错误的信息提示，或修改这个函数做出适当的错误处理。
    //比如说，添加打印信息printf("File Error :%s and Line Error: %d\n",*file,line,)
    ```

  -  如果不使用调试模块，可以取消其宏定义