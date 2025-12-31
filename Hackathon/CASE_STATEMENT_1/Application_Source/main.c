/* ========================================
 *
 * CASE_STATEMENT:  Light intensity-based motor speed Control
 *
 * ========================================
*/

#include "./../Device_Drivers/CLOCK/peri_clock.h"
#include "./../Device_Drivers/IRQ/irq_mgmt.h"
#include "./../Device_Drivers/GPIO/gpio.h"
#include "./../Device_Drivers/ADC/adc.h"
#include "./../Device_Drivers/TIMER/tcpwm.h"

void init_peri_Clock_Config();
void Delay(int32_t delayNumber);

volatile uint16_t chanresult = 0;
uint32_t AvgValue = 0;
uint8_t count = 0;

#define HSIOM_SEL_GPIO          0u
#define ADC_DIV_NUM             3u

const gpio_pin_config_t LED_config = 
{
    .outVal = 0,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};



/* Timer period in milliseconds */
//F = 1K Hz
#define TIMER_PERIOD_MSEC   1000U
#define TIMER_COMPARE_MSEC   500U

#define HSIOM_SEL_ACT_0      8u

#define TCPWM_DIV_NUM        2u

const TCPWM_Config_t tcpwm3_config =
{
    .mode = 4,                                  // PWM mode
    .period = TIMER_PERIOD_MSEC,                // Period value
    .compare = TIMER_COMPARE_MSEC,              // Compare value
    .intrMask = 0,                              // Enable Terminal Count interrupt
    .prescaler = 0                              // Divide by 1
};

const gpio_pin_config_t GPIO_config = 
{
    .outVal = 1,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

// Timer to generate 100Hz interrupt
#define TIMER2_PERIOD_MSEC   1000U
#define TCPWM1_DIV_NUM        1u

const TCPWM_Config_t tcpwm2_config =
{
    .mode = 0,               // Timer mode
    .period = TIMER2_PERIOD_MSEC,  // Period value
    .compare = 0,            // Not used in Timer mode
    .intrMask = 1,          // Enable Terminal Count interrupt
    .prescaler = 0          // Divide by 1
};

const gpio_pin_config_t LED1_config = 
{
    .outVal = 1,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

//GPIO for Motor Enable

const gpio_pin_config_t GPIOIN1_config = 
{
    .outVal = 1,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

const gpio_pin_config_t GPIOIN2_config = 
{
    .outVal = 0,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};


int main()
{  
    GPIO_Pin_Init(5, 2u, &LED_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(1, 2u, &GPIO_config, HSIOM_SEL_ACT_0);

    GPIO_Pin_Init(5, 6u, &GPIOIN1_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(5, 7u, &GPIOIN2_config, HSIOM_SEL_GPIO);

    // GPIO_Pin_Init(1, 4u, &LED1_config, HSIOM_SEL_GPIO);

    /* Peripheral clock initialization */
    init_peri_Clock_Config();

    // TCPWM for 100 Hz interrupt
    NVIC_SetPriority(19u, 1u);
    NVIC_ClearPendingIRQ(19u);
    NVIC_EnableIRQ(19u);

	ADC_Init(0x6, 0x1, 0x7, 0x1);
	ADC_ChannelConfig(0x0, 0x0, 0x0, 0x1); // Channels 0 - 15 channels are available 
	ADC_ChannelEnable(0x0);

    TCPWM_Init(3, (TCPWM_Config_t *)&tcpwm3_config);
    TCPWM_Start(3);
    
    //100 Hz Timer Initialization
    TCPWM_Init(2, (TCPWM_Config_t *)&tcpwm2_config);
    TCPWM_Start(2);

    IRQ_EnableGlobal();

    for(;;)
    {
		ADC_StartConversion();
        
        if(count >= 10)
        {   
            AvgValue = chanresult / 10;
            count = 0;
            chanresult = 0;
            if(AvgValue>1500)
                {
                    GPIO_Clr(5,2);
                    TCPWM_SetCompare(3, 750); // Set motor speed to high
                }
            else
                {
                    GPIO_Set(5,2);
                    TCPWM_SetCompare(3, 350); // Set motor speed to low
                }    
        }
    }

    return 0;
}

/*Timer 2 interrupt*/
void tcpwm_interrupts_2_IRQHandler(void)
{
    TCPWM_ClearInterrupt(2, 1);
    // GPIO_Inv(1, 4u);
    chanresult += ADC_Read(0x0)& 0xFFF; 
    count++;

	//Turn ON LED if light is low on LDR
	
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
    //ADC Peri Clock Configuration
    PeriClock_DisableDivider(1,ADC_DIV_NUM); // Disable Divider type 1 (16-bit) Divider number 3
    PeriClock_Set_16_Divider(ADC_DIV_NUM, 2); // Set the divider value for divider 3
    PeriClock_EnableDivider(1,ADC_DIV_NUM); // Enable Divider type 1 (16-bit) Divider number 3 
    PeriClock_AssignDivider(18,1,ADC_DIV_NUM); // Assign Divider type 1 (16-bit) Divider number 3 to PERIPHERAL 18 ADC

    //TCPWM Peri Clock Configuration 1Khz
    PeriClock_DisableDivider(1,TCPWM_DIV_NUM); // Disable Divider type 1 (16-bit) Divider number 2
    PeriClock_Set_16_Divider(TCPWM_DIV_NUM, 24); // Set the divider value for divider 2
    PeriClock_EnableDivider(1,TCPWM_DIV_NUM); // Enable Divider type 1 (16-bit) Divider number 2 
    PeriClock_AssignDivider(9,1,TCPWM_DIV_NUM); // Assign Divider type 1 (16-bit) Divider number 2 to PERIPHERAL 9

    //TCPWM Peri Clock Configuration 100hz
    PeriClock_DisableDivider(1,TCPWM1_DIV_NUM); // Disable Divider type 1 (16-bit) Divider number 1
    PeriClock_Set_16_Divider(TCPWM1_DIV_NUM, 240); // Set the divider value for divider 1
    PeriClock_EnableDivider(1,TCPWM1_DIV_NUM); // Enable Divider type 1 (16-bit) Divider number 1 
    PeriClock_AssignDivider(8,1,TCPWM1_DIV_NUM); // Assign Divider type 1 (16-bit) Divider number 2 to PERIPHERAL 8

}