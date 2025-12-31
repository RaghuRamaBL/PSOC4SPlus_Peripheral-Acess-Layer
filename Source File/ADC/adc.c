#include "adc.h"
#include <stdint.h>


void ADC_Init(uint8_t VrefSelect, uint8_t PositiveInput, uint8_t NegativeInput, uint8_t ContinuousMode)
{
    uint32_t tempreg;
    tempreg = ADC->CTRL;
    tempreg &= ~ADC_CTRL_MASK;
    tempreg |= (((VrefSelect & 0x07) << 4) | 
                 (1u << 7) | 
                 ((NegativeInput & 0x07) << 9) | 
                 (1u << 30) | 
                 (1u << 31));
    ADC->CTRL = tempreg;
    ADC->MUX_SWITCH0 = ( 1u << PositiveInput);
    ADC->SAMPLE_CTRL |= (ContinuousMode << 16);
    ADC->SAMPLE_TIME01 |= (0xA);
}


uint16_t ADC_Read(uint8_t channel)
{
    uint16_t result;
    // Start conversion on the specified channel
    ADC->START_CTRL = (1 << channel);
    
    // Wait for conversion to complete
    while ((ADC->INTR & 0x1) != 0x1){}
    
    result = (uint16_t)(ADC->CHAN_RESULT[channel] & 0xFFFF);
    // Return the result
    return result;
}

void ADC_StartConversion(void)
{
    // Start ADC conversion
    ADC->START_CTRL = 0x01; // Start conversion on all enabled channels
}

//0  subresolution- 12 bit (0), 8 bit(1), 10 bit(2)
void ADC_ChannelConfig(uint8_t channel, uint8_t subresolution, uint8_t sampleTime, uint8_t PortSelect, uint8_t PinNumber)
{
    uint32_t tempreg;

    if(subresolution == 2){
        ADC->SAMPLE_CTRL |= (0x1 << 0);
        subresolution = 1;
    } 
    tempreg = ADC->CHAN_CONFIG[channel];
    tempreg &= ~ADC_CHANAL_CONFIG_MASK;
    tempreg |= (PinNumber & 0x0F) | 
               ((PortSelect & 0x0F) << 4) | 
               ((subresolution & 0x01) << 9) | 
               ((sampleTime & 0x03) << 12);
    // Configure the specified channel
    ADC->CHAN_CONFIG[channel] = tempreg;

}

void ADC_ChannelEnable(uint8_t channel)
{
    // Enable the specified ADC channel
    ADC->CHAN_EN |= (1 << channel);
}

void ADC_ChannelDisable(uint8_t channel)
{
    // Disable the specified ADC channel
    ADC->CHAN_EN &= ~(1 << channel);
}