

HAL_InitTick(TICK_INT_PRIORITY);  // activate 1ms tick 

HAL_MspInit();  // callback:  can be implemented in user code
                              stm32f1xx_hal_msp.c : example of implementation in some examples, it is empty, from an empty template
 



