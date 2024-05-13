#ifndef _PINS_H
#define _PINS_H

#define MOTOR_TIMER            TIM1_PA_8_9_10_11

#define PWM_CAPTURE_TIMER  TIMER6

#define PWM_INPUT_ROLL       "D2"
#define PWM_INPUT_PITCH      "D3"
#define PWM_INPUT_THROTTLE   "D4"

//RGB LED
#define LED_R_PIN            "E5"
#define LED_R_TIM            TIMER9
#define LED_R_CH             1
//#define LED_R_AF             AF3

#define LED_G_PIN            "E6"
#define LED_G_TIM            TIM9
#define LED_G_CH             2
//#define LED_G_AF             AF3

#define LED_B_PIN            "B7"
#define LED_B_TIM            TIM4
#define LED_B_CH             2
//#define LED_B_AF             AF2


#define MPU6050_I2C        I2C2_PB_10_11

#define LCD_PROXIMITY_SPI  SPI3_PC_10_11_12
#define RADIO_SPI          SPI3_PB_3_4_5

#define LCD_DC_PIN       (char*)"D1"
#define LCD_RST_PIN      (char*)"D0"
#define LCD_SS_PIN       (char*)"A15"

#define RADIO_SDN_PIN     "a0"  //TODO
#define RADIO_SS_PIN      "a0"

#define BUZZER_PIN       "B15"
#define BUZZER_TIMER     TIMER12
#define BUZZER_TIMER_CHANNEL 2

#define BUZZER_TIMER_AF   AF9

#define BUTTON_TIMER     TIMER7
#define BUTTON_UP        "D6"
#define BUTTON_DOWN		 "D7"




#endif
