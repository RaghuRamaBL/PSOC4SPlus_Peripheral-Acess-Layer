/* ========================================
 *
 * CASE_STATEMENT:  Motor Direction & Speed control with switch
 *
 * ========================================
*/

#include "./../Device_Drivers/CLOCK/peri_clock.h"
#include "./../Device_Drivers/IRQ/irq_mgmt.h"
#include "./../Device_Drivers/GPIO/gpio.h"
#include "./../Device_Drivers/TIMER/tcpwm.h"
#include <stdint.h>

void init_peri_Clock_Config();
void Delay(int32_t delayNumber);

#define HSIOM_SEL_GPIO          0u
#define ADC_DIV_NUM             3u

const gpio_pin_config_t SW2_P3_7_config = 
{
    .outVal = 1,
    .driveMode = 0x02UL, // 1 - HZ, 2 - PU, 3 - PD, 6 - SD
    .intEdge = 0x02UL, // 0 - disabled, 1 - Rising, 2 - Falling, 3 - Both
};



/* Timer period in milliseconds */
//F = 1K Hz
#define TIMER_PERIOD_MSEC   1000U
#define TIMER_COMPARE_MSEC   250U

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

//GPIO for Motor Enable

const gpio_pin_config_t GPIOIN1_config = 
{
    .outVal = 1,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

const gpio_pin_config_t GPIOIN2_config = 
{
    .outVal = 1,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

volatile uint8_t flag = 0;
uint8_t dir = 0;

int main()
{  
    GPIO_Pin_Init(1, 2u, &GPIO_config, HSIOM_SEL_ACT_0); // Motor PWM OUT
    GPIO_Pin_Init(5, 5u, &GPIOIN1_config, HSIOM_SEL_GPIO); // Motor IN1
    GPIO_Pin_Init(5, 7u, &GPIOIN2_config, HSIOM_SEL_GPIO); // Motor IN2

    GPIO_Pin_Init(3, 7u, &SW2_P3_7_config, HSIOM_SEL_GPIO); // SW2 Input

    NVIC_SetPriority(3u, 1u);
    NVIC_ClearPendingIRQ(3u);
    NVIC_EnableIRQ(3u);

    /* Peripheral clock initialization */
    init_peri_Clock_Config();

    TCPWM_Init(3, (TCPWM_Config_t *)&tcpwm3_config);
    TCPWM_Start(3);

    IRQ_EnableGlobal();

    for(;;)
    {
        
        if(flag==1u)
        {
            flag = 0u;
            GPIO_Set(5u, 5u);
            GPIO_Set(5u, 7u);
            Delay(150000);
            if(dir)
            {
                GPIO_Clr(5u, 5u);
                GPIO_Set(5u, 7u);
            }
            else
            {
                GPIO_Clr(5u, 7u);
                GPIO_Set(5u, 5u);
            }
            dir = !dir;
            TCPWM_SetCompare(3, 250);
            Delay(150000);
            TCPWM_SetCompare(3, 500);
            Delay(150000);
            TCPWM_SetCompare(3, 750);
            Delay(150000);
            TCPWM_SetCompare(3, 1000);
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
    //TCPWM Peri Clock Configuration 1Khz
    PeriClock_DisableDivider(1,TCPWM_DIV_NUM); // Disable Divider type 1 (16-bit) Divider number 2
    PeriClock_Set_16_Divider(TCPWM_DIV_NUM, 24); // Set the divider value for divider 2
    PeriClock_EnableDivider(1,TCPWM_DIV_NUM); // Enable Divider type 1 (16-bit) Divider number 2 
    PeriClock_AssignDivider(9,1,TCPWM_DIV_NUM); // Assign Divider type 1 (16-bit) Divider number 2 to PERIPHERAL 9

}

void ioss_interrupts_gpio_3_IRQHandler(void)
{

    GPIO_ClearInterrupt(3, 7u);
    Delay(20000);

    if((GPIO_Read(3, 7u) == 0u))
    {
        flag = 1u;
    }  

}