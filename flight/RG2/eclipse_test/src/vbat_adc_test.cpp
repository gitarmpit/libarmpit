#include "adc_cpp.h"
#include "9163/TFT_ILI9163C.h"
#include "_func.h"
#include "debug.h"

static void vref(TFT_ILI9163C* lcd)
{
    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);
    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->EnableTempVref(true);
    adc1->AddConversion (ADC_VREFINT, ADC_SMP_SLOWEST);
    lcd->setTextSize(2);

    while (1)
    {
       uint32_t vbat = adc1->SingleConversion();
       lcd->printf (0, 0, "%d", vbat);
       lcd->display();
       delay(1000);

    }

    //Vref ADC: 1523-1536  STM32F4 168MHz
    //VCC = 3.3V  Vref = 3.3/4095*1536  = 1.2375

}


//measure voltage of the RTC battery (rechargeable button cell)
void vbat()
{
    TFT_ILI9163C* lcd = get_lcd();

    const static float vref = 1.2375;

    lcd->setBgColor(0);
    lcd->setTextColor(YELLOW);
    lcd->setTextSize(2);

    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);
    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->EnableTempVref(true);
    adc1->EnableVBAT(true);
    adc1->AddJConversion (ADC_VBAT, ADC_SMP_SLOWEST);
    adc1->AddJConversion (ADC_VREFINT, ADC_SMP_SLOWEST);
    adc1->SetAutoInjection(true);
    adc1->SetScanMode();
    lcd->setTextSize(2);

    while (1)
    {
       adc1->SingleJConversion();
       uint32_t adc_vref = adc1->GetJData1();
       uint32_t vbat_adc = adc1->GetJData2();
       float vcc = vref / adc_vref * 4095.;
       float vbat = vcc*vbat_adc/2048; //vbat is divided by half
       lcd->printf (0, 0, "adc_vref=%d", adc_vref);
       lcd->printf (0, 1, "Vcc=%4.1fV", vcc);
       lcd->printf (0, 2, "Vbat=%4.1fV", vbat);
       lcd->display();
       delay(1000);

    }

}

void lipo_v()
{
    TFT_ILI9163C* lcd = get_lcd();
    lcd->setBgColor(0);
    lcd->setTextColor(YELLOW);
    lcd->setTextSize(2);

    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN2);
    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->EnableTempVref(true);
    adc1->AddJConversion (ADC_IN2, ADC_SMP_SLOWEST); //Vr2
    adc1->AddJConversion (ADC_VREFINT, ADC_SMP_SLOWEST);
    adc1->SetAutoInjection(true);
    adc1->SetScanMode();

    char buf[8];
    volatile uint32_t adc_vref, adc_r2;
    volatile float  vcc, v_lipo;
    const static float vref = 1.2375;
    const float r1 = 4.87, r2 = 2.0;

    //Vr2 = V_lipo / (R1 + R2) * R2

    //Vr2 = Vcc / 4096 * adc_R2
    //V_lipo = Vr2 * (R1 + R2 ) / R2

    while (1)
    {
        adc1->SingleJConversion();
        adc_r2 = adc1->GetJData2();
        adc_vref = adc1->GetJData1();
        vcc = vref / adc_vref * 4095.;
        v_lipo = vcc / 4095. * adc_r2 * (r1 + r2) / r2;

        lcd->clearScreen();
        lcd->printf (0, 0, "vcc: %4.1f", vcc);
        lcd->printf (0, 1, "vbat: %4.1f", v_lipo);
        lcd->display();

        //TODO 4095 cancels out:
        //vbat = vref / adc_vref * adc_r2 * (r1 + r2) / r2;
        //also move const out of the loop: k = vref  * (r1 + r2) / r2
        //then vbat = k / adc_vref * adc_r2

        delay(100);
    }
}

