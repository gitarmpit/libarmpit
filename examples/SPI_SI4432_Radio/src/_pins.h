#ifndef _PINS_H
#define _PINS_H

//#define MOTOR_TIMER            GPIO_Helper::SetupTIM3_PC_6_7_8_9    //TIM3
//#define CONTROLLER_TIMER       TIM2::GetInstance()                      //TIM2
//#define PITCH_ROLL_LED_TIMER   GPIO_Helper::SetupTIM1_PA_8_9_10_11        //TIM1
//#define THROTTLE_LED_TIMER     GPIO_Helper::SetupTIM4_PD_12_13_14_15      //TIM4

//RGB LED
//#define RGB_LED_RED            GPIOE::GetInstance()->GetPin(GPIO_PIN2)  //E2
//#define RGB_LED_GREEN          GPIOE::GetInstance()->GetPin(GPIO_PIN3)  //E3
//#define RGB_LED_BLUE           GPIOE::GetInstance()->GetPin(GPIO_PIN4)  //E4
#define LED_R_PIN            "E2"  //E5
#define LED_G_PIN            "E3"  //E6
#define LED_B_PIN            "E4"  //B7

//single LED
#define LED_GREEN         GPIOC::GetInstance()->GetPin(GPIO_PIN11)    //C11

#define MPU6050_I2C       GPIO_Helper::SetupI2C2_PB10_11

#define LCD_RADIO_SPI     GPIO_Helper::SetupSPI1_MasterMode_PA_5_6_7

#define LCD_DC_PIN        GPIOB::GetInstance()->GetPin(GPIO_PIN0)  //B0
#define LCD_RST_PIN       GPIOB::GetInstance()->GetPin(GPIO_PIN1)  //B1
#define LCD_SS_PIN        GPIOB::GetInstance()->GetPin(GPIO_PIN2)  //B2

#define RADIO_SDN_PIN     GPIOC::GetInstance()->GetPin(GPIO_PIN4)  //C4
#define RADIO_SS_PIN      GPIOA::GetInstance()->GetPin(GPIO_PIN4)  //A4

#define PROXIMITY_SPI     GPIO_Helper::SetupSPI2_MasterMode_PB_13_14_15


#define CONTROLLER_TIMER_AF            AF1
#define CONTROLLER_TIMER_PORT          GPIOA::GetInstance() //TIM2

#define CONTROLLER_ROLL_PIN            GPIO_PIN0
#define CONTROLLER_PITCH_PIN           GPIO_PIN1
#define CONTROLLER_THROTTLE_PIN        GPIO_PIN2

#define CONTROLLER_ROLL_CHANNEL        1
#define CONTROLLER_PITCH_CHANNEL       2
#define CONTROLLER_THROTTLE_CHANNEL    3





#endif
