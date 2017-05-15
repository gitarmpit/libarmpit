#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "GPIO_Helper.h"
#include "i2c_cpp.h"
#include "SSD1306/Adafruit_SSD1306.h"



static void testLCD() 
{
    GPIOA* portA = GPIOA::GetInstance();
    GPIOB* portB = GPIOB::GetInstance();
    portA->EnablePeripheralClock(true);
    portB->EnablePeripheralClock(true);

    I2C* i2c = GPIO_Helper::SetupI2C1_PB_8_9(100000);
    
   Adafruit_SSD1306 display (i2c);
   display.setTextColor(0, 1);
   display.clearDisplay();
   display.display();
   display.write("Hello, World!");
   display.display();


   while(1)
       ;
}



int main()
{
    RCC_EnableHSI_100Mhz();
    testLCD();
}

