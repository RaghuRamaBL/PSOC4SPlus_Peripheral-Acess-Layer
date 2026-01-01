/* ========================================
 *
 * CASE_STATEMENT:  Alternate the motor rotation direction at every 1sec and 5sec Run motor for 5sec -> Halt for 1Sec -> Run the motor in reverse direction for 5 sec → Halt for 1Sec → Run the motor in reverse direction for 
5 sec → ..
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


// Timer to generate 1 Sec interrupt
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

// Timer to generate 5 Sec interrupt
#define TIMER1_PERIOD_MSEC   5000U

const TCPWM_Config_t tcpwm1_config =
{
    .mode = 0,               // Timer mode
    .period = TIMER1_PERIOD_MSEC,  // Period value
    .compare = 0,            // Not used in Timer mode
    .intrMask = 1,          // Enable Terminal Count interrupt
    .prescaler = 0          // Divide by 1
};


volatile uint8_t flag = 0;
volatile uint8_t flag2 = 0;
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

    // TCPWM for 1 Sec interrupt
    NVIC_SetPriority(19u, 1u);
    NVIC_ClearPendingIRQ(19u);
    NVIC_EnableIRQ(19u);

    // TCPWM for 5 Sec interrupt
    NVIC_SetPriority(18u, 1u);
    NVIC_ClearPendingIRQ(18u);
    NVIC_EnableIRQ(18u);

    /* Peripheral clock initialization */
    init_peri_Clock_Config();

    TCPWM_Init(3, (TCPWM_Config_t *)&tcpwm3_config);
    TCPWM_Start(3);

    //1 Sec Timer Initialization
    TCPWM_Init(2, (TCPWM_Config_t *)&tcpwm2_config);
    // TCPWM_Start(2);

    //5 Sec Timer Initialization
    TCPWM_Init(1, (TCPWM_Config_t *)&tcpwm1_config);
    TCPWM_Start(1);

    IRQ_EnableGlobal();

    for(;;)
    {
        
        if(flag==1u)
        {
            flag = 0u;
            GPIO_Set(5u, 5u);
            GPIO_Set(5u, 7u);
            Delay(150000);
            TCPWM_Start(2); // Start 1 Sec Timer
        }
        if(flag2==1u)
        {
            flag2 = 0u;
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
            TCPWM_Start(1); // Start 5 Sec Timer
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

    //TCPWM Peri Clock Configuration 1 Sec and 5 Sec Timer
    PeriClock_DisableDivider(1,TCPWM1_DIV_NUM); // Disable Divider type 1 (16-bit) Divider number 1
    PeriClock_Set_16_Divider(TCPWM1_DIV_NUM, 24000); // Set the divider value for divider 1
    PeriClock_EnableDivider(1,TCPWM1_DIV_NUM); // Enable Divider type 1 (16-bit) Divider number 1 
    PeriClock_AssignDivider(8,1,TCPWM1_DIV_NUM); // Assign Divider type 1 (16-bit) Divider number 2 to PERIPHERAL 8
    PeriClock_AssignDivider(7,1,TCPWM1_DIV_NUM); // Assign Divider type 1 (16-bit) Divider number 2 to PERIPHERAL 8
}

// 1 Sec Timer Interrupt
void tcpwm_interrupts_2_IRQHandler(void)
{
    TCPWM_ClearInterrupt(2, 1);
    TCPWM_Stop(2); // Stop 1 Sec Timer
    flag2 = 1u;
}

// 5 Sec Timer Interrupt
void tcpwm_interrupts_1_IRQHandler(void)
{
    TCPWM_ClearInterrupt(1, 1);
    TCPWM_Stop(1); // Stop 5 Sec Timer
    flag = 1u;
}