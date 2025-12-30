#include "./../Device_Drivers/TIMER/tcpwm.h"
#include "./../Device_Drivers/CLOCK/Sys_Clock.h"
#include "./../Device_Drivers/IRQ/irq_mgmt.h"
#include "./../Device_Drivers/GPIO/gpio.h"

// void timer_Delay_uS(uint32_t delay_uS);

/* Timer period in milliseconds */
#define TIMER_PERIOD_MSEC   10000U
#define TIMER_COMPARE_MSEC   5000U

#define HSIOM_SEL_ACT_0          8u

void init_peri_Clock_Config();

const TCPWM_Config_t tcpwm3_config =
{
    .mode = 4,               // Timer mode
    .period = TIMER_PERIOD_MSEC,  // Period value
    .compare = TIMER_COMPARE_MSEC,            // Not used in Timer mode
    .intrMask = 0,          // Enable Terminal Count interrupt
    .prescaler = 0          // Divide by 1
};

const gpio_pin_config_t LED8_P1_6_config = 
{
    .outVal = 1,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

int main()
{  
    /* HF CLOCK divider init*/
    Cy_SysClk_ClkHfSetDivider(0u); //0 - No Divider, 1 - DIV by 2, 2 = DIV by 4, 3 = DIV by 8

    /*GPIO pin init*/
    // *((uint32_t *)0x40040100) = (1 << 4); // Set default output value of P1.4 to 1 in GPIO_PRT1_DR
    // *((uint32_t *)0x40040108) = (6 << 12); // Set drive mode of P1.4 to Digital OP Push Pull in GPIO_PRT1_PC

    GPIO_Pin_Init(1, 2u, &LED8_P1_6_config, HSIOM_SEL_ACT_0);

    /* Peripheral clock initializatio*/
    init_peri_Clock_Config();

    NVIC_SetPriority(19u, 1u);
    NVIC_ClearPendingIRQ(19u);
    NVIC_EnableIRQ(19u);

    // /*TIMER 2 - INTERRUPT*/
	// *((uint32_t *)0xE000E410) = (1 << 30); //Timer 2 IRQn=19 Priority  1 set
	// *((uint32_t *)0xE000E280) = 0xFFFFFFFF; //NVIC Clear Pending IRQs
	// *((uint32_t *)0xE000E100) = (1 << 19); //NVIC_EnableIRQ 19 


	TCPWM_Init(3, (TCPWM_Config_t *)&tcpwm3_config);
    TCPWM_Start(3);

    // /*TIMER 2 - INIT*/
    // *((uint32_t *)0x40200000) &=~ (1<< 2); //Disable Timer 2  in TCPWM_CTRL Register

    // *((uint32_t *)0x40200188) = 0; //Clear the counter register of  TCPWM2 TCPWM_CNT2_COUNTER Register

    // *((uint32_t *)0x402001A8) = 0; //Clear the  register of  TCPWM2 TCPWM_CNT2_TR_CTRL2 Register

	// *((uint32_t *)0x40200194)  = (TIMER_PERIOD_MSEC-1); //Set the Period Register of TCPWM2 TCPWM_CNT2_PERIOD Register

	// *((uint32_t *)0x40200180) |= (0 << 24); //Mode configuration of for TCPWM2, TCPWM_CNT2_CTRL Regsiter 

	// *((uint32_t *)0x402001B8) |= (1 << 0); // Set  interrupt mask  by enabling the Interrupt in TCPWM_CNT2_INTR_MASK Register for TCPWM2

	// *((uint32_t *)0x402001B0) |= (1<<0); //Clear any previoius interrupt in  interrupt register in TCPWM_CNT2_INTR Register of TCPWM2 

    // *((uint32_t *)0x40200000) |= (1<< 2); //Enable Timer 2  in TCPWM_CTRL Register

    // *((uint32_t *)0x40200008) = (1 << 26); //Triger start Timer 2  in TCPWM_CTRL Register
    /* Enable Interrupts at CPU level */
    // enable_irq();
    IRQ_EnableGlobal();
    // GPIO_Clr(1, 4u);
     for(;;)
    {
        // GPIO_Inv(1, 4u);
        // GPIO_Inv(1, 6u);
        // timer_Delay_uS(6000u);

        // GPIO_Inv(1, 4u);
        // GPIO_Inv(1, 6u);
        // timer_Delay_uS(6000u);


    }

    return 0;
}

void init_peri_Clock_Config()
{
    //TIMER 3 PWM- CLOCK
    *((uint32_t *)0x40010000) = (1<<30); // Disable the Divider using PERI_DIV_CMD 

    *((uint32_t *)0x40010300) = (240 - 1) << 8 ; //Set the divider value in PERI_DIV_16_CTL0, We are configuring Divider 0 

    *((uint32_t *)0x40010000) |= (1<<31) |(3<<14) |(63<<8) |(1<<6) ; //PERI_DIV_CMD 
    //Enable the divder 31:bit, Keep 3 at 15:14 and 63 13:8 this selects the HFCLK as reference , Select 16 bit divider 7:6, and Select the divider no 0 using 5:0 so not writing 0 bit

    *((uint32_t *)0x40010124) = (1<<6); // Specify Divider type 7:6 and Selected Divider 3:0 (Divider 0) in register PERI_PCLK_CTL9 TCPWM2 is PERIPHERAL 9

}