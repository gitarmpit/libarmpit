#ifndef _FUNC_H
#define _FUNC_H

#include "9163/TFT_ILI9163C.h"

TFT_ILI9163C* get_lcd();
void lcd_hue();
void lcd_test();
void lcd_speed_test();


void rgb_test();
void rgb_pwm_test();


void boot();
void buzzer();
void bmp_test();

void flash_cfg();
void flash_test();

//buttons
void clicks_test();
void button_test();
void button_test2();

void desktop_test();


void lipo_v();
void vbat();


void systick_test();


void pwm_out_test();

void pwm_capture_test();
void pwm_capture_test2();

void rtc_test();


void motionapp_test();
void motionapp_test2();


void kwiner_raw();
void kwiner_ahrs_loop();

#endif

