#include "gpio_helper.h"
#include <stdlib.h>
#include <string.h>

static SPI* SetupSPIx (BOOL isMaster, uint8_t presc, SPI_BRDIV div, BOOL isPhaseStart, BOOL isPolarityLow, BOOL isLSB,
        GPIO_PORT* port,  GPIO_PIN_N clkPin, GPIO_PIN_N mosiPin, GPIO_PIN_N misoPin, SPI_N spiN, int af)
{
    GPIO_EnableClock(port, TRUE);
    GPIO_PIN CLK_pin = GPIO_GetPin (port, clkPin);
    GPIO_PIN MOSI_pin = GPIO_GetPin (port, mosiPin);
    GPIO_PIN MISO_pin = GPIO_GetPin (port, misoPin);

    GPIO_SetAF(&CLK_pin, af);
    GPIO_SetAF(&MISO_pin, af);
    GPIO_SetAF(&MOSI_pin, af);

    SPI* spi = SPI_GetInstance(spiN);
    SPI_Initialize(spi, isMaster, presc, div, isPhaseStart, isPolarityLow, isLSB);

    return spi;
}

SPI* GPIO_Helper_SetupSPI(SPI_Pins spi_no, BOOL isMaster, uint8_t presc, SPI_BRDIV div, BOOL isPhaseStart, BOOL isPolarityLow, BOOL isLSB)
{
    switch (spi_no)
    {
    case SPI0_PD_1_2_3:
        return SetupSPIx(isMaster, presc, div, isPhaseStart, isPolarityLow, isLSB, GPIO_GetInstance(PORTD),
                GPIO_PIN1, GPIO_PIN2, GPIO_PIN3, SPI0, 2);

    case SPI0_PC_5_6_7:
        return SetupSPIx(isMaster, presc, div, isPhaseStart, isPolarityLow, isLSB, GPIO_GetInstance(PORTC),
                GPIO_PIN5, GPIO_PIN6, GPIO_PIN7, SPI0, 2);

    case SPI1_PD_5_6_7:
        return SetupSPIx(isMaster, presc, div, isPhaseStart, isPolarityLow, isLSB, GPIO_GetInstance(PORTD),
                GPIO_PIN5, GPIO_PIN6, GPIO_PIN7, SPI1, 2);

    case SPI0_PE_17_18_19:
        return SetupSPIx(isMaster, presc, div, isPhaseStart, isPolarityLow, isLSB, GPIO_GetInstance(PORTE),
                GPIO_PIN17, GPIO_PIN18, GPIO_PIN19, SPI0, 2);
    };

    return NULL;

}

GPIO_PIN GPIO_Helper_GetPin (const char* pin) //A1,B1,C15 etc
{
    if (!pin || strlen(pin) < 2)
    {
        while(1)
            ;
    }

    GPIO_PORT* port;
    switch (pin[0])
    {
    case 'a':
    case 'A':
        port = GPIO_GetInstance(PORTA);
        break;
    case 'b':
    case 'B':
        port = GPIO_GetInstance(PORTB);
        break;
    case 'c':
    case 'C':
        port = GPIO_GetInstance(PORTC);
        break;
    case 'd':
    case 'D':
        port = GPIO_GetInstance(PORTD);
        break;
    case 'e':
    case 'E':
        port = GPIO_GetInstance(PORTE);
        break;
    default:
        while(1)
            ;

    };

    GPIO_EnableClock(port, TRUE);

    int pin_no = atoi(&pin[1]);
    return GPIO_GetPin(port, (GPIO_PIN_N)pin_no);
}

static TPM_Channel_t* _GPIO_Helper_SetupTPM(TPM_N tpmN, uint8_t chN, const char* cpin, uint8_t af)
{
    GPIO_PIN pin = GPIO_Helper_GetPin(cpin);
    GPIO_SetAF(&pin, af);
    GPIO_PullDown(&pin);

	TPM* tpm = TPM_GetInstance(tpmN);
	TPM_EnableClock(tpm, TRUE);

	return TPM_GetChannel(tpm, chN);
}

// TPM0: 0-5, TP1/TPM2: 0,1
TPM_Channel_t* GPIO_Helper_SetupTPM_Channel_t(TPM_Pins pin)
{
    switch (pin)
    {
    case TPM1_CH0_E20:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 0, "E20", 3);
    case TPM1_CH1_E21:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 1, "E21", 3);

    case TPM2_CH0_E22:
    	return _GPIO_Helper_SetupTPM(TPM2_INSTANCE, 0, "E22", 3);
    case TPM2_CH1_E23:
    	return _GPIO_Helper_SetupTPM(TPM2_INSTANCE, 1, "E23", 3);

    case TPM0_CH2_E29:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 2, "E29", 3);
    case TPM0_CH3_E30:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 3, "E30", 3);
    case TPM0_CH4_E31:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 4, "E31", 3);

    case TPM0_CH0_E24:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 0, "E24", 3);
    case TPM0_CH1_E25:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 1, "E25", 3);

    case TPM0_CH5_A0:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 5, "A0", 3);
    case TPM2_CH0_A1:
    	return _GPIO_Helper_SetupTPM(TPM2_INSTANCE, 0, "A1", 3);
    case TPM2_CH1_A2:
    	return _GPIO_Helper_SetupTPM(TPM2_INSTANCE, 1, "A2", 3);

    case TPM0_CH0_A3:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 0, "A3", 3);
    case TPM0_CH1_A4:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 1, "A4", 3);
    case TPM0_CH2_A5:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 2, "A5", 3);

    case TPM1_CH0_A12:
    	return _GPIO_Helper_SetupTPM(TPM1_INSTANCE, 0, "A12", 3);
    case TPM1_CH1_A13:
    	return _GPIO_Helper_SetupTPM(TPM1_INSTANCE, 1, "A13", 3);

    case TPM1_CH0_B0:
    	return _GPIO_Helper_SetupTPM(TPM1_INSTANCE, 0, "B0", 3);
    case TPM1_CH1_B1:
    	return _GPIO_Helper_SetupTPM(TPM1_INSTANCE, 1, "B1", 3);

    case TPM2_CH0_B2:
    	return _GPIO_Helper_SetupTPM(TPM2_INSTANCE, 0, "B2", 3);
    case TPM2_CH1_B3:
    	return _GPIO_Helper_SetupTPM(TPM2_INSTANCE, 1, "B3", 3);

    case TPM2_CH0_B18:
    	return _GPIO_Helper_SetupTPM(TPM2_INSTANCE, 0, "B18", 3);
    case TPM2_CH1_B19:
    	return _GPIO_Helper_SetupTPM(TPM2_INSTANCE, 1, "B19", 3);

    case TPM0_CH0_C1:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 0, "C1", 4);
    case TPM0_CH1_C2:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 1, "C2", 4);
    case TPM0_CH2_C3:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 2, "C3", 4);
    case TPM0_CH3_C4:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 3, "C4", 4);

    case TPM0_CH0_D0:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 0, "D0", 4);
    case TPM0_CH1_D1:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 1, "D1", 4);
    case TPM0_CH2_D2:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 2, "D2", 4);
    case TPM0_CH3_D3:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 3, "D3", 4);
    case TPM0_CH4_D4:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 4, "D4", 4);
    case TPM0_CH5_D5:
    	return _GPIO_Helper_SetupTPM(TPM0_INSTANCE, 5, "D5", 4);
    default:
          while(1)
              ;

    };
}


ADC_Channel GPIO_Helper_SetupADC(ADC_N nADC, ADC_CH_N nCh)
{
	ADC* adc = ADC_GetInstance(nADC);
	ADC_EnableClock(adc, TRUE);

	GPIO_PIN pin;
	BOOL setPin = TRUE;

    switch (nCh)
    {
    	case ADC_CH_SE0:
    	{
    		pin = GPIO_Helper_GetPin("E20");
    		break;
    	}

    	case ADC_CH_SE1:
    	{
    		pin = GPIO_Helper_GetPin("E16");
    		break;
    	}
    	case ADC_CH_SE2:
    	{
    		pin = GPIO_Helper_GetPin("E18");
    		break;
    	}
    	case ADC_CH_SE3:
    	{
    		pin = GPIO_Helper_GetPin("E22");
    		break;
    	}
    	case ADC_CH_SE4A:
    	{
    		pin = GPIO_Helper_GetPin("E21");
    		break;
    	}
    	case ADC_CH_SE5A:
    	{
    		pin = GPIO_Helper_GetPin("E17");
    		break;
    	}
    	case ADC_CH_SE6A:
    	{
    		pin = GPIO_Helper_GetPin("E19");
    		break;
    	}
    	case ADC_CH_SE7A:
    	{
    		pin = GPIO_Helper_GetPin("E23");
    		break;
    	}
    	case ADC_CH_SE8:
    	{
    		pin = GPIO_Helper_GetPin("B0");
    		break;
    	}
    	case ADC_CH_SE9:
    	{
    		pin = GPIO_Helper_GetPin("B1");
    		break;
    	}
    	case ADC_CH_SE11:
    	{
    		pin = GPIO_Helper_GetPin("C2");
    		break;
    	}
    	case ADC_CH_SE12:
    	{
    		pin = GPIO_Helper_GetPin("B2");
    		break;
    	}
    	case ADC_CH_SE13:
    	{
    		pin = GPIO_Helper_GetPin("B3");
    		break;
    	}
    	case ADC_CH_SE14:
    	{
    		pin = GPIO_Helper_GetPin("C0");
    		break;
    	}
    	case ADC_CH_SE15:
    	{
    		pin = GPIO_Helper_GetPin("C1");
    		break;
    	}
    	case ADC_CH_SE23:
    	{
    		pin = GPIO_Helper_GetPin("E30");
    		break;
    	}
    	case ADC_CH_SE4B:
    	{
    		pin = GPIO_Helper_GetPin("E29");
    		break;
    	}
    	case ADC_CH_SE5B:
    	{
    		pin = GPIO_Helper_GetPin("D1");
    		break;
    	}
    	case ADC_CH_SE6B:
    	{
    		pin = GPIO_Helper_GetPin("D5");
    		break;
    	}
    	case ADC_CH_SE7B:
    	{
    		pin = GPIO_Helper_GetPin("D6");
    		break;
    	}
    	case ADC_CH_TEMP:
    	case ADC_CH_VREFSH:
    	case ADC_CH_VREFSL:
    	case ADC_CH_BANDGAP:
    	{
    		setPin = FALSE;
    		break;
    	}

    	default:
    		while(1);
    };

    if (setPin)
    {
    	GPIO_SetupAnalog(&pin);
    }

    return ADC_GetChannel(adc, nCh);
}

