/* ========================================
 *
 * GPIO Application Example
 *
 * ========================================
*/

#include <stdint.h>
#include "./../Device_Drivers/IRQ/irq_mgmt.h"
#include "./../Device_Drivers/GPIO/gpio.h"

#define HSIOM_SEL_GPIO          0u

const gpio_pin_config_t LED_config = 
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

void Delay(int32_t);

int main()
{
    IRQ_EnableGlobal();

    GPIO_Pin_Init(2, 2u, &LED_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(1, 6u, &LED_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(3, 7u, &SW2_P3_7_config, HSIOM_SEL_GPIO);

    NVIC_SetPriority(3u, 1u);
    NVIC_ClearPendingIRQ(3u);
    NVIC_EnableIRQ(3u);

    for(;;)
    {
        GPIO_Set(1, 6u);
        Delay(150000);

        GPIO_Clr(1, 6u);
        Delay(150000);  

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