/* ========================================
 *
 * TCPWM1_1 - Dual Timer Application Example
 *
 * ========================================
*/

#include "./../Device_Drivers/TIMER/tcpwm.h"
#include "./../Device_Drivers/CLOCK/peri_clock.h"
#include "./../Device_Drivers/IRQ/irq_mgmt.h"
#include "./../Device_Drivers/GPIO/gpio.h"

/* Timer period in milliseconds */
#define TIMER2_PERIOD_MSEC   1000U
#define TIMER1_PERIOD_MSEC   2000U

#define HSIOM_SEL_GPIO          0u

void init_peri_Clock_Config();

const TCPWM_Config_t tcpwm2_config =
{
    .mode = 0,               // Timer mode
    .period = TIMER2_PERIOD_MSEC,  // Period value
    .compare = 0,            // Not used in Timer mode
    .intrMask = 1,          // Enable Terminal Count interrupt
    .prescaler = 0          // Divide by 1
};

const TCPWM_Config_t tcpwm1_config =
{
    .mode = 0,               // Timer mode
    .period = TIMER1_PERIOD_MSEC,  // Period value
    .compare = 0,            // Not used in Timer mode
    .intrMask = 1,          // Enable Terminal Count interrupt
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

    /* Peripheral clock initialization */
    init_peri_Clock_Config();

    NVIC_SetPriority(19u, 1u);
    NVIC_ClearPendingIRQ(19u);
    NVIC_EnableIRQ(19u);
    NVIC_SetPriority(18u, 1u);
    NVIC_ClearPendingIRQ(18u);
    NVIC_EnableIRQ(18u);

	TCPWM_Init(2, (TCPWM_Config_t *)&tcpwm2_config);
    TCPWM_Init(1, (TCPWM_Config_t *)&tcpwm1_config);
    TCPWM_Start(2);
    TCPWM_Start(1);

    IRQ_EnableGlobal();

     for(;;)
    {

    }

    return 0;
}

/*Timer 1 interrupt*/
void tcpwm_interrupts_1_IRQHandler(void)
{
    TCPWM_ClearInterrupt(1, 1);
    GPIO_Inv(1, 6u);
}

/*Timer 2 interrupt*/
void tcpwm_interrupts_2_IRQHandler(void)
{
    TCPWM_ClearInterrupt(2, 1);
    GPIO_Inv(1, 4u);
}

/*Peripheral clock initilizations*/
void init_peri_Clock_Config()
{
    PeriClock_DisableDivider(1,3); // Disable Divider type 1 (16-bit) Divider number 3
    PeriClock_Set_16_Divider(3, 24000); // Set the divider value for divider 3
    PeriClock_EnableDivider(1,3); // Enable Divider type 1 (16-bit) Divider number 3 
    PeriClock_AssignDivider(8,1,3); // Assign Divider type 1 (16-bit) Divider number 3 to PERIPHERAL 8
    PeriClock_AssignDivider(7,1,3); // Assign Divider type 1 (16-bit) Divider number 3 to PERIPHERAL 7

}