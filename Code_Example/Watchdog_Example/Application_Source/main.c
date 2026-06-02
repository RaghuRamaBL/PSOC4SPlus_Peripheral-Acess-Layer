#include <stdint.h>
#include "./../Device_Drivers/GPIO/gpio.h"
#include "./../Special_Libraries/cmsis_gcc.h"

#define CYREG_GPIO_PRT2_DR      0x40040200u
#define CYREG_GPIO_PRT2_PC      0x40040208u

#define CYREG_GPIO_PRT1_DR      0x40040100u
#define CYREG_GPIO_PRT1_PC      0x40040108u

#define CYREG_GPIO_PRT3_DR      0x40040300u
#define CYREG_GPIO_PRT3_PC      0x40040308u

#define SRSS_INTR_REG (*(volatile uint32_t *)0x40030044u)
#define WDT_MATCH_REG (*(volatile uint32_t *)0x40030040u)
#define SRSS_INTR_MASK_REG (*(volatile uint32_t *)0x4003004Cu)

#define WDT_TIMEOUT_COUNT 40000
volatile uint32_t wdt_count = 0;

volatile uint32_t dbg_match;

#define HSIOM_SEL_GPIO          0u

const gpio_pin_config_t LED10_P2_2_config = 
{
    .outVal = 1,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

const gpio_pin_config_t LED8_P1_6_config = 
{
    .outVal = 1,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

const gpio_pin_config_t SW2_P3_7_config = 
{
    .outVal = 1,
    .driveMode = 0x02UL, // 1 - HZ, 2 - PU, 3 - PD, 6 - SD
    .intEdge = 0x02UL, // 0 - disabled, 1 - Rising, 2 - Falling, 3 - Both
};

const gpio_pin_config_t SW_LED9_P2_0_config = 
{
    .outVal = 1,
    .driveMode = 0x02UL, // 1 - HZ, 2 - PU, 3 - PD, 6 - SD
    .intEdge = 0x02UL, // 0 - disabled, 1 - Rising, 2 - Falling, 3 - Both
};


void Delay(int32_t);

int main()
{
   // register int Var_1, Var_2, Var_3, Var_4, Var_5; 
   
    WDT_MATCH_REG = WDT_TIMEOUT_COUNT;   // Set WDT timeout to 1 second (assuming 40kHz WCO)
    SRSS_INTR_REG = 1u;
    SRSS_INTR_MASK_REG |= 1u;   // Enable WDT_MATCH interrupt forwarding

    GPIO_Pin_Init((GPIO_PRT_Type *)CYREG_GPIO_PRT2_DR, 2u, &LED10_P2_2_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init((GPIO_PRT_Type *)CYREG_GPIO_PRT1_DR, 6u, &LED8_P1_6_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init((GPIO_PRT_Type *)CYREG_GPIO_PRT3_DR, 7u, &SW2_P3_7_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init((GPIO_PRT_Type *)CYREG_GPIO_PRT2_DR, 0u, &SW_LED9_P2_0_config, HSIOM_SEL_GPIO);


    NVIC_SetPriority(6u, 1u);
    NVIC_SetPriority(3u, 1u);
    NVIC_SetPriority(2u, 1u);
    /* Clearing and enabling the GPIO interrupt in NVIC */
    NVIC_ClearPendingIRQ(3u);
    NVIC_ClearPendingIRQ(6u);
    NVIC_ClearPendingIRQ(2u);
    NVIC_EnableIRQ(3u);
    NVIC_EnableIRQ(6u);
    NVIC_EnableIRQ(2u);
   
    enable_irq();

    // GPIO_Clr((GPIO_PRT_Type *)CYREG_GPIO_PRT1_DR, 6u);
    for (int32_t i = 0; i < 6; i++)
    {
        GPIO_Inv((GPIO_PRT_Type *)CYREG_GPIO_PRT1_DR, 6u);
        Delay(100000);
    }

    for(;;)
    {

    }
    return 0;
}

void Delay(int32_t delayNumber)
{
    for(int32_t i=0; i<delayNumber; i++);
    for(int32_t i=0; i<delayNumber; i++);
}

void srss_interrupt_wdt_IRQHandler(void)
{
    dbg_match  = WDT_MATCH_REG;
    SRSS_INTR_REG = 1u;

    wdt_count++;

    uint32_t match = WDT_MATCH_REG & 0xFFFF;
    match = (match + WDT_TIMEOUT_COUNT) & 0xFFFF;

    WDT_MATCH_REG = match;  
}

void ioss_interrupts_gpio_3_IRQHandler(void)
{

    GPIO_ClearInterrupt((GPIO_PRT_Type *)CYREG_GPIO_PRT3_DR, 7u);
    Delay(20000);

    if((GPIO_Read((GPIO_PRT_Type *)CYREG_GPIO_PRT3_DR, 7u) == 0u))
    {
        GPIO_Inv((GPIO_PRT_Type *)CYREG_GPIO_PRT2_DR, 2u);
    }  

    while(1);

}

void ioss_interrupts_gpio_2_IRQHandler(void)
{

    GPIO_ClearInterrupt((GPIO_PRT_Type *)CYREG_GPIO_PRT2_DR, 0u);
    Delay(20000);

    if((GPIO_Read((GPIO_PRT_Type *)CYREG_GPIO_PRT2_DR, 0u) == 0u))
    {
        GPIO_Inv((GPIO_PRT_Type *)CYREG_GPIO_PRT2_DR, 2u);
    }  

}