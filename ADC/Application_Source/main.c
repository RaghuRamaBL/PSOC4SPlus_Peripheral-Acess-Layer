#include "./../Device_Drivers/TIMER/tcpwm.h"
#include "./../Device_Drivers/CLOCK/peri_clock.h"
#include "./../Device_Drivers/IRQ/irq_mgmt.h"
#include "./../Device_Drivers/GPIO/gpio.h"
#include "./../Device_Drivers/ADC/adc.h"

void init_peri_Clock_Config();
void clock_config(void);
void Delay(int32_t delayNumber);

volatile uint16_t chanresult = 0;
volatile uint8_t chanresult1 = 0;
volatile uint16_t chanresult2 = 0;

#define HSIOM_SEL_GPIO          0u

const gpio_pin_config_t LED8_P2_1_config = 
{
    .outVal = 0,
    .driveMode = 0x00UL,
    .intEdge = 0x00UL,
};

const gpio_pin_config_t LED8_P1_6_config = 
{
    .outVal = 0,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

int main()
{  
    /* HF CLOCK divider init*/
   // Cy_SysClk_ClkHfSetDivider(0u); //0 - No Divider, 1 - DIV by 2, 2 = DIV by 4, 3 = DIV by 8
    // clock_config();

    GPIO_Pin_Init(2, 1u, &LED8_P2_1_config, HSIOM_SEL_GPIO);
	/* Configure P2.1 for ADC*/
	// *((uint32_t *)0x40040208) &= ~(0x3 << 3u); // clear drive mode of P2.1 to analog mode -> GPIO2.1.PC

	/* Configure P2.2 for LED*/
	// *((uint32_t *)0x40040200) = (1 << 2); // Set default output value of P2.2 to 1 GPIO_PRT2_DR
	// *((uint32_t *)0x40040208) |= (0x6 << 6u); // set drive mode of P2.2 to strong drive mode -> GPIO2.2.PC
	// *((uint32_t *)0x40020200) &= ~(0xF << 8u); // Clear HSIOM_PORT_SEL2 pin 2 -> SW GPIO
    GPIO_Pin_Init(5, 2u, &LED8_P1_6_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(5, 5u, &LED8_P1_6_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(5, 7u, &LED8_P1_6_config, HSIOM_SEL_GPIO);

    /* Peripheral clock initialization */
    init_peri_Clock_Config();

	ADC_Init(0x6, 0x1, 0x7, 0x0);
	ADC_ChannelConfig(0x0, 0x0, 0x0, 0x00, 0x1);
    // ADC_SetSubResolution(0x0);
    ADC_ChannelConfig(0x1, 0x1, 0x0, 0x00, 0x1);
    // ADC_SetSubResolution(0x1);
    ADC_ChannelConfig(0x2, 0x2, 0x0, 0x00, 0x1);

	ADC_ChannelEnable(0x0);
    ADC_ChannelEnable(0x1);
    ADC_ChannelEnable(0x2);

	/*ADC INIT*/
	// SAR_CTRL  4-> Vref=VDDA/2V, 7->BypassEn, 9->NEG_SEL=Vref, 30->SAR_SEQ=Disable, 31->SAR_En=1
	// *((uint32_t*)0x403A0000) |= ((0x6 << 4) | 
	// 							 (0x1 << 7) | 
	// 							 (0x7 << 9) | 
	// 							 (0x1 << 30)| 
	// 							 (0x1 << 31));

	// // SAR_MUX_SWITCH0 -> select Vplus to P2.1
	// *((uint32_t*)0x403A0300) = (0x1<<1u); 

	// //SAR_SAMPLE_CTRL 1->Left/Right Align, 2->SingleEnded=Unsigned, 16->Continuous=False
	// *((uint32_t*)0x403A0004) |= (0x0 << 1) | 
	// 							(0x0 << 2) |
	// 							(0x0 << 16);
	
	// //SAMPLE_TIME01 -> set Time0 to 10 sampling adc clock cycles
	// *((uint32_t*)0x403A0010) |= (0xA);
	
	// SAR_CHAN0_CONFIG -> 0-6 ->pin and port address to P2.1, 12&13->SAMPLE_TIME=0, 9->RESOLUTION=MAX(12)
	// *((uint32_t*)0x403A0080) =  (0x1 << 0)  |
	// 							(0x0 << 4) |
	// 							(0x0 << 9) |
	// 							(0x0 << 12);

								
	//SAR_CHAN_EN -> Channel 0 enable
	// *((uint32_t*)0x403A0020) = (0x1<<0);

    IRQ_EnableGlobal();

    for(;;)
    {
		ADC_StartConversion();
		//SAR_START_CTRL -> Start ADC conversion
        // *((uint32_t*)0x403A0024) = (0x1<<0); 

		// SAR_INTR-> check for EOS -Wail till ADC completes conversion
		// while( (*((uint32_t*)0x403A0210) & 0x1) != 0x1) 
        // { }

		// // SAR_CHAN_RESULT0 -> Read result
		// chanresult = (uint16_t)((*(uint32_t*)0x403A0180) & 0xFFFF); 

		chanresult = ADC_Read(0x0)& 0xFFF;
		Delay(500);
        
		chanresult2 = ADC_Read(0x2)& 0x3FF;

		 Delay(500);
		chanresult1 = ADC_Read(0x1)& 0xFF;

		//Turn ON LED if light is low on LDR
		if(chanresult>2000)
		{
			// clear P2.2 -> TURNS ON LED10
            GPIO_Clr(5,2);
			// *((uint32_t *)0x40040200) &= ~(1 << 2); 
		}
		else
		{
            GPIO_Set(5,2);
			// Set P2.2 -> TURNS OFF LED10
			// *((uint32_t *)0x40040200) |= (1 << 2); 
		}
        //Turn ON LED if light is low on LDR
		if(chanresult1>200)
		{
			// clear P2.2 -> TURNS ON LED10
            GPIO_Clr(5,5);
			// *((uint32_t *)0x40040200) &= ~(1 << 2); 
		}
		else
		{
            GPIO_Set(5,5);
			// Set P2.2 -> TURNS OFF LED10
			// *((uint32_t *)0x40040200) |= (1 << 2); 
		}
        //Turn ON LED if light is low on LDR
		if(chanresult2>700)
		{
			// clear P2.2 -> TURNS ON LED10
            GPIO_Clr(5,7);
			// *((uint32_t *)0x40040200) &= ~(1 << 2); 
		}
		else
		{
            GPIO_Set(5,7);
			// Set P2.2 -> TURNS OFF LED10
			// *((uint32_t *)0x40040200) |= (1 << 2); 
		}

		// Delay(5000);
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
    PeriClock_AssignDivider(18,1,3); // Assign Divider type 1 (16-bit) Divider number 3 to PERIPHERAL 8
	/* ADC CLOCK CONFIGURATION*/
	// *((uint32_t *)0x40010000) = (1<<30)|(1<<6)|(1<<0); // Disable the Divider 1 using PERI_DIV_CMD 
	// *((uint32_t *)0x40010304) = (2 - 1) << 8 ; //Set the divider value in PERI_DIV_16_CTL1, We are configuring Divider 1 to 12MHz
	// *((uint32_t *)0x40010000) |= (1<<31) |(3<<14) |(63<<8) |(1<<6)|(1<<0); //PERI_DIV_CMD 
	// *((uint32_t *)0x40010148) = (1<<6)|(1<<0); // Specify Divider type 7:6 and Selected Divider 3:0 (Divider 1) in register PERI_PCLK_CTL18 ADC is PERIPHERAL 18
}