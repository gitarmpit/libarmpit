#include "adc_reader.h" 


ADC_Reader::ADC_Reader()
{
	is_A = false;
	is_B = false;
	is_X = false;
	is_Y = false;
	_buttonHandler = 0;
}

void ADC_Reader::SetHandler(ADC_Button_Handler* buttonHandler)
{
    _buttonHandler = buttonHandler;
}

void ADC_Reader::HandleInterrupt(DMA_Channel* channel, bool isTransferComplete,
		bool isHalfTransfer, bool isTransferError)
{
    UNUSED(channel);
    UNUSED(isHalfTransfer);
    UNUSED(isTransferError);

	if (isTransferComplete && _buttonHandler)
	{
//#ifdef SEMIHOSTING
//		for (uint8_t i = 0 ; i < 10; ++i)
//		{
//			//printf ("%d %d %d %d %d %d\n", data[0], data[1], data[2], data[3], data[8], data[9]);
//			printf ("%d %d %d %d\n", data[4], data[5], data[6], data[7]);
//		}
//#endif

		//hysteresis
		static uint32_t high_threshold = 4040;
		static uint32_t low_threshold = 3900;

		if (!is_Y && data[4] < low_threshold)
		{
			is_Y = true;
		}

		if (is_Y && data[4] > high_threshold)
		{
			is_Y = false;
			_buttonHandler->Analog_Clicked(ANALOG_BUTTON::Y);
		}

		if (!is_B && data[5] < low_threshold)
		{
			is_B = true;
		}

		if (is_B && data[5] > high_threshold)
		{
			is_B = false;
            _buttonHandler->Analog_Clicked(ANALOG_BUTTON::B);
		}

		if (!is_X && data[6] < low_threshold)
		{
			is_X = true;
		}

		if (is_X && data[6] > high_threshold)
		{
			is_X = false;
            _buttonHandler->Analog_Clicked(ANALOG_BUTTON::X);
		}

		if (!is_A && data[7] < low_threshold)
		{
			is_A = true;
		}

		if (is_A && data[7] > high_threshold)
		{
			is_A = false;
            _buttonHandler->Analog_Clicked(ANALOG_BUTTON::A);
		}
	}
}






void ADC_Reader::Start()
{

    GPIOA* gpioA = GPIOA::GetInstance();
    gpioA->EnablePeripheralClock(true);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN0);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN1);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN2);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN3);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN4);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN5);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN6);
    gpioA->SetupGPIO_InAnalog(GPIO_PIN7);
    //gpioA->SetupGPIO_InAnalog(GPIO_PIN8);
    //gpioA->SetupGPIO_InAnalog(GPIO_PIN9);

    ADC1* adc1 = ADC1::GetInstance();
    adc1->Enable(true);
    adc1->SetScanMode();
    adc1->AddConversion(ADC_IN0, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN1, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN2, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN3, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN4, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN5, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN6, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN7, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN8, ADC_SMP_SLOWEST);
    adc1->AddConversion(ADC_IN9, ADC_SMP_SLOWEST);

    //adc1->EnableInterrupt(true);
    //ADC1_IRQ_Handler adcHandler;
    //adc1->SetInterruptHandler(&adcHandler);

    DMA1* dma1 = DMA1::GetInstance();
    dma1->EnablePeripheralClock(true);
    DMA_Channel* ch = dma1->GetChannel(DMA_CHANNEL_1);
    ch->EnableTransferCompleteInterrupt();

    adc1->EnableDMA(ch, 10, getDataPtr());
    adc1->StartContinuousConversion();

}

