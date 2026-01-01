/* ========================================
 *
 * CASE_STATEMENT:  Auto Street lamp control system  - 1 LDRs
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

uint8_t LED[9][2] = {{2,2},{2,0},{1,6},{1,4},{1,2},{1,0},{5,2},{5,5},{5,7}};

int main()
{  
    for (int i = 0; i < 9; i++) GPIO_Pin_Init(LED[i][0],LED[i][1], &LED_config, HSIOM_SEL_GPIO);

    /* Peripheral clock initialization */
    init_peri_Clock_Config();

    // TCPWM for 100 Hz interrupt
    NVIC_SetPriority(19u, 1u);
    NVIC_ClearPendingIRQ(19u);
    NVIC_EnableIRQ(19u);

	ADC_Init(0x6, 0x1, 0x7, 0x1);
	ADC_ChannelConfig(0x0, 0x0, 0x0, 0x1); // Channels 0 - 15 channels are available 
	ADC_ChannelEnable(0x0);

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

            if (AvgValue < 1000)for (int i = 0; i < 9; i++) GPIO_Set(LED[i][0], LED[i][1]);
            else if (AvgValue > 1000 && AvgValue < 1500){
                for (int i = 0; i < 2; i++) GPIO_Clr(LED[i][0], LED[i][1]);
                for (int i = 2; i < 9; i++) GPIO_Set(LED[i][0], LED[i][1]);
            }
            else if (AvgValue > 1500 && AvgValue < 2000){
                for (int i = 0; i < 4; i++) GPIO_Clr(LED[i][0], LED[i][1]);
                for (int i = 4; i < 9; i++) GPIO_Set(LED[i][0], LED[i][1]);
            }
            else if (AvgValue > 2000 && AvgValue < 3000){
                for (int i = 0; i < 6; i++) GPIO_Clr(LED[i][0], LED[i][1]);
                for (int i = 6; i < 9; i++) GPIO_Set(LED[i][0], LED[i][1]);
            }
            else if (AvgValue > 3000){
                for (int i = 0; i < 9; i++) GPIO_Clr(LED[i][0], LED[i][1]);
            }
        }
    }

    return 0;
}

/*Timer 2 interrupt*/
void tcpwm_interrupts_2_IRQHandler(void)
{
    TCPWM_ClearInterrupt(2, 1);
    chanresult += ADC_Read(0x0)& 0xFFF; 
    count++;
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

    //TCPWM Peri Clock Configuration 100hz
    PeriClock_DisableDivider(1,TCPWM1_DIV_NUM); // Disable Divider type 1 (16-bit) Divider number 1
    PeriClock_Set_16_Divider(TCPWM1_DIV_NUM, 240); // Set the divider value for divider 1
    PeriClock_EnableDivider(1,TCPWM1_DIV_NUM); // Enable Divider type 1 (16-bit) Divider number 1 
    PeriClock_AssignDivider(8,1,TCPWM1_DIV_NUM); // Assign Divider type 1 (16-bit) Divider number 2 to PERIPHERAL 8

}