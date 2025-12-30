#include <stdint.h>
#include "./../Device_Drivers/gpio.h"
#include "./../Device_Drivers/irq_mgmt.h"


#define CYREG_GPIO_PRT2_DR      0x40040200u
#define CYREG_GPIO_PRT2_PC      0x40040208u

#define CYREG_GPIO_PRT3_DR      0x40040300u
#define CYREG_GPIO_PRT3_PC      0x40040308u

#define CYREG_GPIO_PRT1_DR      0x40040100u
#define CYREG_GPIO_PRT1_PC      0x40040108u

#define HSIOM_SEL_GPIO          0u

const gpio_pin_config_t LED10_P2_2_config = 
{
    .outVal = 1,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

const gpio_pin_config_t SW_LED9_P2_0_config = 
{
    .outVal = 1,
    .driveMode = 0x02UL, // 1 - HZ, 2 - PU, 3 - PD, 6 - SD
    .intEdge = 0x02UL, // 0 - disabled, 1 - Rising, 2 - Falling, 3 - Both
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

void Delay(int32_t);

int main()
{
   // register int Var_1, Var_2, Var_3, Var_4, Var_5; 
    IRQ_EnableGlobal();
    GPIO_Pin_Init(2, 2u, &LED10_P2_2_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(3, 7u, &SW2_P3_7_config, HSIOM_SEL_GPIO);
    // GPIO_Pin_Init((GPIO_PRT_Type *)CYREG_GPIO_PRT2_DR, 0u, &LED9_P2_0_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(2, 0u, &SW_LED9_P2_0_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(1, 6u, &LED8_P1_6_config, HSIOM_SEL_GPIO);

    NVIC_SetPriority(3u, 1u);
    NVIC_SetPriority(2u, 1u);
    /* Clearing and enabling the GPIO interrupt in NVIC */
    NVIC_ClearPendingIRQ(3u);
    NVIC_ClearPendingIRQ(2u);
    NVIC_EnableIRQ(3u);
    NVIC_EnableIRQ(2u);

    for(;;)
    {
        GPIO_Set(1, 6u);
        Delay(60000);

        GPIO_Clr(1, 6u);
        Delay(60000);  
 
    }
    return 0;
}

void Delay(int32_t delayNumber)
{
    for(int32_t i=0; i<delayNumber; i++);
    for(int32_t i=0; i<delayNumber; i++);
}

void ioss_interrupts_gpio_3_IRQHandler(void)
{

    GPIO_ClearInterrupt(3, 7u);
    Delay(20000);

    if((GPIO_Read(3, 7u) == 0u))
    {
        GPIO_Inv(2, 2u);
    }  

}

void ioss_interrupts_gpio_2_IRQHandler(void)
{

    GPIO_ClearInterrupt(2, 0u);
    Delay(20000);

    if((GPIO_Read(2, 0u) == 0u))
    {
        GPIO_Inv(2, 2u);
    }  

}