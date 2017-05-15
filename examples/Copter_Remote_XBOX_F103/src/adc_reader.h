#ifndef _ADC_READER_H
#define _ADC_READER_H

#include "adc_cpp.h"
#include "gpio_cpp.h"
#include "RCC_Helper.h"
#include "dma_cpp.h"
#include <stdio.h>

enum  ANALOG_BUTTON
{
    A, B, X, Y
};

class ADC_Button_Handler
{
public:
    virtual void Analog_Clicked(ANALOG_BUTTON button) = 0;
    virtual ~ADC_Button_Handler() {}
};


class ADC_Reader : public DMA_IRQ_Handler
{
private:
	uint16_t data[16] = {0};

	bool is_Y;
	bool is_B;
	bool is_X;
	bool is_A;

	ADC_Button_Handler* _buttonHandler;

public:
	ADC_Reader();
	void Start();
    void SetHandler(ADC_Button_Handler* buttonHandler);

    virtual void HandleInterrupt(DMA_Channel* channel, bool isTransferComplete,
            bool isHalfTransfer, bool isTransferError);

    void* getDataPtr() { return data; }

    uint32_t getThrottle() { return data[0]; }

    virtual ~ADC_Reader() {}
};



#endif
