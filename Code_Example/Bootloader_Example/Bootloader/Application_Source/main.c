#include <stdint.h>
#include "./../Device_Drivers/GPIO/gpio.h"
#include "./../Special_Libraries/cmsis_gcc.h"

#define CYREG_GPIO_PRT3_DR      0x40040300u
#define CYREG_GPIO_PRT3_PC      0x40040308u
#define HSIOM_SEL_GPIO          0u

#define APP_A_ADDRESS 0x00008000U
#define APP_B_ADDRESS 0x00014000U

#define VTOR (*(volatile uint32_t *)0xE000ED08)

typedef void (*pFunction)(void);

void BootApplication(uint32_t app_address);

const gpio_pin_config_t SW2_P3_7_config = 
{
    .outVal = 1,
    .driveMode = 0x02UL, // 1 - HZ, 2 - PU, 3 - PD, 6 - SD
    .intEdge = 0x00UL, // 0 - disabled, 1 - Rising, 2 - Falling, 3 - Both
};


int main(void)
{
    uint32_t app_address;

    GPIO_Pin_Init((GPIO_PRT_Type *)CYREG_GPIO_PRT3_DR, 7u, &SW2_P3_7_config, HSIOM_SEL_GPIO);

    if((GPIO_Read((GPIO_PRT_Type *)CYREG_GPIO_PRT3_DR, 7u) == 0u))
    {
        app_address = APP_B_ADDRESS;
    }
    else
    {
        app_address = APP_A_ADDRESS;
    }

    BootApplication(app_address);


    while(1);
}

void BootApplication(uint32_t app_address)
{
    uint32_t app_sp;
    uint32_t app_reset;

    app_sp    = *(volatile uint32_t *)(uintptr_t)app_address;
    app_reset = *(volatile uint32_t *)(uintptr_t)(app_address + 4U);

    /* Validity check */
    if((app_sp < 0x20000000U) ||
       (app_sp > 0x20004000U))
    {
        while(1);
    }

    if((app_reset == 0xFFFFFFFFU) ||
       (app_reset == 0x00000000U))
    {
        while(1);
    }

    disable_irq();

    VTOR = app_address;

    __asm volatile ("dsb");
    __asm volatile ("isb");

    // Set the main stack pointer to the application's stack pointer
    __asm volatile ("msr msp, %0" :: "r" (app_sp));

    __asm volatile ("dsb");
    __asm volatile ("isb");

    pFunction JumpToApp =
        (pFunction)(uintptr_t)app_reset;

    JumpToApp();

    while(1);
}