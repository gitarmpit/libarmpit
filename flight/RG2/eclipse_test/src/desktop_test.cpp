#include "_pins.h"
#include "buzzer.h"
#include "GPIO_Helper.h"
#include "main_button_handler.h"
#include "_func.h"


void desktop_test()
{
    boot();
    uint32_t i2cClockSpeed = 100000;
    I2C* i2c = GPIO_Helper::SetupI2C(MPU6050_I2C, i2cClockSpeed);

    Buzzer*  buzzer = Buzzer::GetInstance();
    buzzer->Init(BUZZER_TIMER, BUZZER_PIN, BUZZER_TIMER_CHANNEL);
    buzzer->SetVolume(100);
    MainButtonHandler appl (TIMER::Get_Instance(BUTTON_TIMER), get_lcd(), i2c, buzzer);
    appl.Run();

    while(1)
        ;

}
