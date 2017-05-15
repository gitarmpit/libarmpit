#include "_pins.h"
#include "GPIO_Helper.h"
#include "ESC_PWM_Control.h"

void pwm_out_test()
{
	TIMER_With_Channels* pwmTimer = GPIO_Helper::SetupTimer(MOTOR_TIMER);
	ESC_PWM_Control esc (pwmTimer);
    esc.SetDuty(700, 1000, 1500, 2000);
    while(7)
        ;


}
