#ifndef _PINS_H
#define _PINS_H

#define MOTOR_TIMER          TIM3_PC_6_7_8_9
#define PITCH_ROLL_LED_TIMER TIM1_PA_8_9_10_11
#define THROTTLE_LED_TIMER   TIM4_PD_12_13_14_15


#define PWM_CAPTURE_TIMER  TIM2
#define PWM_INPUT_ROLL       "A0"
#define PWM_INPUT_PITCH      "A1"
#define PWM_INPUT_THROTTLE   "A2"


//RGB LED
#define RGB_LED_RED            "E2"
#define RGB_LED_GREEN          "E3"
#define RGB_LED_BLUE           "E4"

//single LED
#define LED_GREEN         "C11"

#define MPU6050_I2C       I2C2_PB_10_11

#define LCD_RADIO_SPI     SPI1_PA_5_6_7

#define LCD_DC_PIN        "B0"
#define LCD_RST_PIN       "B1"
#define LCD_SS_PIN        "B2"

#define RADIO_SDN_PIN     "C4"
#define RADIO_SS_PIN      "A4"

#define PROXIMITY_SPI     SPI2_PB_13_14_15


#endif
