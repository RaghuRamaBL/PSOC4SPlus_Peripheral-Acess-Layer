/* ========================================
 *
 * TCPWM3_1 - PWM Application Example
 *
 * ========================================
*/

#include "./../Device_Drivers/TIMER/tcpwm.h"
#include "./../Device_Drivers/CLOCK/peri_clock.h"
#include "./../Device_Drivers/IRQ/irq_mgmt.h"
#include "./../Device_Drivers/GPIO/gpio.h"

/* Timer period in milliseconds */
#define TIMER_PERIOD_MSEC   10000U
#define TIMER_COMPARE_MSEC   5000U

#define HSIOM_SEL_ACT_0          8u

void init_peri_Clock_Config();

const TCPWM_Config_t tcpwm3_config =
{
    .mode = 4,                                  // PWM mode
    .period = TIMER_PERIOD_MSEC,                // Period value
    .compare = TIMER_COMPARE_MSEC,              // Compare value
    .intrMask = 0,                              // Enable Terminal Count interrupt
    .prescaler = 0                              // Divide by 1
};

const gpio_pin_config_t LED_config = 
{
    .outVal = 1,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

int main()
{  
    GPIO_Pin_Init(1, 2u, &LED_config, HSIOM_SEL_ACT_0);

    /* Peripheral clock initializatio*/
    init_peri_Clock_Config();

    NVIC_SetPriority(19u, 1u);
    NVIC_ClearPendingIRQ(19u);
    NVIC_EnableIRQ(19u);

	TCPWM_Init(3, (TCPWM_Config_t *)&tcpwm3_config);
    TCPWM_Start(3);

    /* Enable Interrupts at CPU level */
    IRQ_EnableGlobal();

     for(;;)
    {
        
    }

    return 0;
}

void init_peri_Clock_Config()
{
    PeriClock_DisableDivider(1,3); // Disable Divider type 1 (16-bit) Divider number 3
    PeriClock_Set_16_Divider(3, 240); // Set the divider value for divider 3
    PeriClock_EnableDivider(1,3); // Enable Divider type 1 (16-bit) Divider number 3 
    PeriClock_AssignDivider(9,1,3); // Assign Divider type 1 (16-bit) Divider number 3 to PERIPHERAL 9

}