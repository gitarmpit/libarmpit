       IMPORT  TIM_Init
    IMPORT TIM_EnableCounter
    IMPORT TIM_EnableChannel
    IMPORT TIM_EnableUpdateEvent
    IMPORT TIM_EnableAutoPreload    
    IMPORT TIM_EnableUpdateInterrupt
    IMPORT TIM_EnableNVICInterrupt
    IMPORT TIM_EnableTriggerInterrupt
    IMPORT TIM_SetCounterValue
    IMPORT TIM_SetPrescalerValue
    IMPORT TIM_SetAutoReloadValue
    IMPORT TIM_GetCounterValue
    IMPORT TIM_GetPrescalerValue
    IMPORT TIM_GetAutoReloadValue
    IMPORT TIM_CalculateTimerValues
    IMPORT TIM_SetUpdatePeriod_us
    IMPORT TIM_IsUIF
    IMPORT TIM_ClearUIF
    IMPORT TIM_SetICCFallingEdge
    IMPORT TIM_SetICCRisingEdge
    IMPORT TIM_SetupPWM1

    IMPORT GPIO_init              ;GPIO_cfg: crl crh idr odr bsrr brr
    IMPORT GPIO_cfg_out_alt_pp        ;GPIO_cfg  pin 
    IMPORT GPIO_set_pin           ;GPIO_cfg  pin
    IMPORT GPIO_reset_pin         ;GPIO_cfg  pin
    IMPORT GPIO_toggle_pin           ;GPIO_cfg  pin
    IMPORT GPIO_cfg_in_float
    IMPORT GPIO_is_set

    IMPORT RCC_SetAPB1Prescaler

    IMPORT FLASH_SetWaitState
    IMPORT RCC_EnablePLL
    IMPORT TIM_EnableCCInterrupt
    IMPORT TIM_SetModeInputDirect
    IMPORT TIM_GetCCR
    IMPORT TIM_IsCCFlag
    IMPORT TIM_GetTimerClockFreq

    IMPORT _udiv64 
    IMPORT Debug_EnableCYCCNT
    IMPORT Debug_Delay_us

 END