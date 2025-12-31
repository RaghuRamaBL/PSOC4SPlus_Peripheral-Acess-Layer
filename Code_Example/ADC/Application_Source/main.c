/* ========================================
 *
 * ADC Application Example with different resolution
 *
 * ========================================
*/

#include "./../Device_Drivers/CLOCK/peri_clock.h"
#include "./../Device_Drivers/IRQ/irq_mgmt.h"
#include "./../Device_Drivers/GPIO/gpio.h"
#include "./../Device_Drivers/ADC/adc.h"

void init_peri_Clock_Config();
void Delay(int32_t delayNumber);

volatile uint16_t chanresult = 0;
volatile uint8_t chanresult1 = 0;
volatile uint16_t chanresult2 = 0;

#define HSIOM_SEL_GPIO          0u

const gpio_pin_config_t LED_config = 
{
    .outVal = 0,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

int main()
{  
    GPIO_Pin_Init(5, 2u, &LED_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(5, 5u, &LED_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(5, 7u, &LED_config, HSIOM_SEL_GPIO);

    /* Peripheral clock initialization */
    init_peri_Clock_Config();

	ADC_Init(0x6, 0x1, 0x7, 0x0);
	ADC_ChannelConfig(0x0, 0x0, 0x0, 0x00, 0x1); // Channels 0 - 15 channels are available 
    ADC_ChannelConfig(0x1, 0x1, 0x0, 0x00, 0x1);
    ADC_ChannelConfig(0x2, 0x2, 0x0, 0x00, 0x1);

	ADC_ChannelEnable(0x0);
    ADC_ChannelEnable(0x1);
    ADC_ChannelEnable(0x2);

    IRQ_EnableGlobal();

    for(;;)
    {
		ADC_StartConversion();

		chanresult = ADC_Read(0x0)& 0xFFF;
		Delay(500);
        
		chanresult2 = ADC_Read(0x2)& 0x3FF;

		 Delay(500);
		chanresult1 = ADC_Read(0x1)& 0xFF;

		//Turn ON LED if light is low on LDR
		if(chanresult>2000)
		{
            GPIO_Clr(5,2);
		}
		else
		{
            GPIO_Set(5,2);
		}

        //Turn ON LED if light is low on LDR
		if(chanresult1>200)
		{
            GPIO_Clr(5,5);
		}
		else
		{
            GPIO_Set(5,5);
		}

		//Turn ON LED if light is low on LDR
		if(chanresult2>700)
		{
            GPIO_Clr(5,7);
		}
		else
		{
            GPIO_Set(5,7);
		}

    }

    return 0;
}

/*Delay with simple for loops*/
void Delay(int32_t delayNumber)
{
    for(int32_t i=0; i<delayNumber; i++);
    for(int32_t i=0; i<delayNumber; i++);
}

/*Peripheral clock initilizations*/
void init_peri_Clock_Config()
{
    PeriClock_DisableDivider(1,3); // Disable Divider type 1 (16-bit) Divider number 3
    PeriClock_Set_16_Divider(3, 2); // Set the divider value for divider 3
    PeriClock_EnableDivider(1,3); // Enable Divider type 1 (16-bit) Divider number 3 
    PeriClock_AssignDivider(18,1,3); // Assign Divider type 1 (16-bit) Divider number 3 to PERIPHERAL 18 ADC

}