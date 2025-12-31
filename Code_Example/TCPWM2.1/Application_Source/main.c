/* ========================================
 *
 * TCPWM2_1 - Timer Application Percise Delay Example
 *
 * ========================================
*/

#include "./../Device_Drivers/TIMER/tcpwm.h"
#include "./../Device_Drivers/CLOCK/peri_clock.h"
#include "./../Device_Drivers/IRQ/irq_mgmt.h"
#include "./../Device_Drivers/GPIO/gpio.h"

void timer_Delay_uS(uint32_t delay_uS);

/* Timer period in milliseconds */
#define TIMER_PERIOD_MSEC   32768U

#define HSIOM_SEL_GPIO          0u

void init_peri_Clock_Config();

const TCPWM_Config_t tcpwm_config =
{
    .mode = 0,               // Timer mode
    .period = TIMER_PERIOD_MSEC,  // Period value
    .compare = 0,            // Not used in Timer mode
    .intrMask = 0,          // Enable Terminal Count interrupt
    .prescaler = 0          // Divide by 1
};

const gpio_pin_config_t LED_config = 
{
    .outVal = 1,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

int main()
{  

    GPIO_Pin_Init(1, 6u, &LED_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(1, 4u, &LED_config, HSIOM_SEL_GPIO);

    /* Peripheral clock initializatio*/
    init_peri_Clock_Config();

    NVIC_SetPriority(19u, 1u);
    NVIC_ClearPendingIRQ(19u);
    NVIC_EnableIRQ(19u);

	TCPWM_Init(0, (TCPWM_Config_t *)&tcpwm_config); //Initialize TCPWM block 0 in Timer mode with the configuration structure
    
    IRQ_EnableGlobal();

    GPIO_Clr(1, 4u);

     for(;;)
    {
        GPIO_Inv(1, 4u);
        GPIO_Inv(1, 6u);
        timer_Delay_uS(20000u);

        GPIO_Inv(1, 4u);
        GPIO_Inv(1, 6u);
        timer_Delay_uS(20000u);


    }

    return 0;
}


/*Delay with simple for loops*/
void Delay(int32_t delayNumber)
{
    for(int32_t i=0; i<delayNumber; i++);
    for(int32_t i=0; i<delayNumber; i++);
}

/*Delay with timer block*/
void timer_Delay_uS(uint32_t delay_uS)
{
    TCPWM_SetCounter(0, 0); //Reset the counter value to 0
    TCPWM_Start(0); //Start the Timer
    while (delay_uS > TCPWM_GetCounter(0)); // Keep Checking the TCPWM_CNT2_COUNTER Register
    TCPWM_Stop(0); //Stop the Timer
}

void init_peri_Clock_Config()
{
    PeriClock_DisableDivider(1,3); // Disable Divider type 1 (16-bit) Divider number 3
    PeriClock_Set_16_Divider(3, 24); // Set the divider value for divider 3
    PeriClock_EnableDivider(1,3); // Enable Divider type 1 (16-bit) Divider number 3 
    PeriClock_AssignDivider(6,1,3); // Assign Divider type 1 (16-bit) Divider number 3 to PERIPHERAL 6
}