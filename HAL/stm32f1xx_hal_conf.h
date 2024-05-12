Template:

C:\git\STM32CubeF1\Drivers\STM32F1xx_HAL_Driver\Inc\stm32f1xx_hal_conf_template.h

called from 
C:\git\STM32CubeF1\Drivers\STM32F1xx_HAL_Driver\Inc\stm32f1xx_hal.h


copy to project and rename to stm32f1xx_hal_conf.h


Modify to include specific peripherals 

#define HAL_PPP_MODULE_ENABLED

#ifdef HAL_PPP_MODULE_ENABLED
 #include "stm32f1xx_hal_ppp.h"
#endif 

