


#include "hd44780.h"
#include <stdio.h>
#include "TempLM74.h"
#include "gpio_cpp.h"
#include "spi_cpp.h"

static void lcd_test()
{
    /*    GPIOB::GetInstance()->Enable();
     GPIOB::GetInstance()->SetupGPIO_OutPP (GPIO_PIN5);
     GPIOB::GetInstance()->SetupGPIO_OutPP (GPIO_PIN6);
     GPIOB::GetInstance()->SetupGPIO_OutPP (GPIO_PIN7);
     GPIOB::GetInstance()->SetupGPIO_OutPP (GPIO_PIN8);
     GPIOB::GetInstance()->SetupGPIO_OutPP (GPIO_PIN12);
     GPIOB::GetInstance()->SetupGPIO_OutPP (GPIO_PIN14);
     GPIOB::GetInstance()->SetPin (GPIO_PIN4);
     */

    LCD lcd(GPIOB::GetInstance(), GPIOB::GetInstance(), GPIO_PIN5, GPIO_PIN6,
            GPIO_PIN7, GPIO_PIN8, GPIO_PIN14,  //EN
            GPIO_PIN12,  //RS
            GPIO_PIN4);  //RW

    lcd.LCD_INIT();
    //lcd.LCD_CursorOn();
    lcd.LCD_PrintChar('*');
    lcd.LCD_PrintString((uint8_t *) "rogI2\n");
    lcd.LCD_LShift();
    lcd.LCD_PrintChar('R');
    char buf[64];
    lcd.LCD_Clear();

    GPIOA::GetInstance()->EnablePeripheralClock(true);
    GPIOA::GetInstance()->SetupGPIO_OutPP(GPIO_PIN4); //NSS  slave select

    SPI1* spi1 = SPI1::GetInstance();
    spi1->Enable(true);
    spi1->SetSlaveSelectHigh();

    bool isMaster = true;
    bool isFullDuplex = true;
    bool isClockPhase2 = true;
    bool isClockPolarity1 = true;
    bool isLSBFirst = false;
    bool isSetSSM = true;
    bool initializeGPIO = true;

    spi1->Initialize(isMaster, isFullDuplex, SPI::SPI_BAUD_RATE_256,
            isClockPhase2, isClockPolarity1, isLSBFirst, isSetSSM,
            initializeGPIO);

    TempLM74 lm74(spi1);
    lm74.Init();

    volatile uint16_t code = lm74.GetProductCode();

    lm74.Shutdown();

    volatile float t;

    bool good = false;
    while (1)
    {
        lcd.LCD_Home();
        t = lm74.GetTemp();
        if (t > -60.)
        {
            sprintf(buf, "Temp: %8.3f C\n", t);
            lcd.LCD_PrintString((uint8_t *) buf);
            good = true;
        }
        else
        {
            if (good)
            {
                good = false;
                lcd.LCD_Clear();
            }
            lcd.LCD_PrintString((uint8_t *) "Temp: ???\n");
        }
        RCC::GetInstance()->Delay_ms(500);
    }
    //https://learningmsp430.wordpress.com/2013/11/16/16x2-lcd-interfacing-in-4-bit-mode/  
    //https://learningmsp430.wordpress.com/2013/11/13/16x2-lcd-interfacing-in-8bit-mode/
}


int main()
{
    RCC* rcc = RCC::GetInstance();
    rcc->SetAHBPrescalerDiv4();
    rcc->SetAPB1PrescalerDiv1();
    rcc->SetAPB2PrescalerDiv1();
    rcc->SetADCPrescalerDiv2();
    rcc->EnableHSI(3);

    lcd_test();
}
