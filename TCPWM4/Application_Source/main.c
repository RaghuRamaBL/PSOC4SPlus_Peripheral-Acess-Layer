#include "./../Device_Drivers/TIMER/tcpwm.h"
#include "./../Device_Drivers/CLOCK/Sys_Clock.h"
#include "./../Device_Drivers/IRQ/irq_mgmt.h"
#include "./../Device_Drivers/GPIO/gpio.h"

void timer_Delay_uS(uint32_t delay_uS);
void Delay(int32_t delayNumber);

/* Timer period in milliseconds */
#define TIMER_PERIOD_MSEC   32768U

#define HSIOM_SEL_GPIO          0u

void init_peri_Clock_Config();

const TCPWM_Config_t tcpwm2_config =
{
    .mode = 0,               // Timer mode
    .period = TIMER_PERIOD_MSEC,  // Period value
    .compare = 0,            // Not used in Timer mode
    .intrMask = 0,          // Enable Terminal Count interrupt
    .prescaler = 0          // Divide by 1
};

const gpio_pin_config_t LED8_P1_6_config = 
{
    .outVal = 1,
    .driveMode = 0x06UL,
    .intEdge = 0x00UL,
};

uint32_t Time_CodeProfile = 0;

int main()
{  
    /* HF CLOCK divider init*/
    Cy_SysClk_ClkHfSetDivider(0u); //0 - No Divider, 1 - DIV by 2, 2 = DIV by 4, 3 = DIV by 8

    /*GPIO pin init*/
    // *((uint32_t *)0x40040100) = (1 << 4); // Set default output value of P1.4 to 1 in GPIO_PRT1_DR
    // *((uint32_t *)0x40040108) = (6 << 12); // Set drive mode of P1.4 to Digital OP Push Pull in GPIO_PRT1_PC

    GPIO_Pin_Init(1, 6u, &LED8_P1_6_config, HSIOM_SEL_GPIO);
    GPIO_Pin_Init(1, 4u, &LED8_P1_6_config, HSIOM_SEL_GPIO);

    /* Peripheral clock initializatio*/
    init_peri_Clock_Config();

    NVIC_SetPriority(19u, 1u);
    NVIC_ClearPendingIRQ(19u);
    NVIC_EnableIRQ(19u);

    // /*TIMER 2 - INTERRUPT*/
	// *((uint32_t *)0xE000E410) = (1 << 30); //Timer 2 IRQn=19 Priority  1 set
	// *((uint32_t *)0xE000E280) = 0xFFFFFFFF; //NVIC Clear Pending IRQs
	// *((uint32_t *)0xE000E100) = (1 << 19); //NVIC_EnableIRQ 19 


	TCPWM_Init(0, (TCPWM_Config_t *)&tcpwm2_config);
    // TCPWM_Start(2);

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
    GPIO_Clr(1, 4u);
     for(;;)
    {

        /*Code profiling Code - measures the time required for execution */

        TCPWM_SetCounter(0, 0); //Reset the counter value to 0
        TCPWM_Start(0); //Start the Timer
        Delay(5000);  //Processing time to be measured
        TCPWM_Stop(0); //Stop the Timer
        Time_CodeProfile = TCPWM_GetCounter(0);
        
        
      //  *((uint32_t *)0x40200108) = 0; //Clear the counter register of  TCPWM0 TCPWM_CNT0_COUNTER Register

      //  *((uint32_t *)0x40200008) |= (1<<24); //Trigger start for Timer0 in TCPWM_CMD REgister 

      //  Delay(5000);   //Processing time to be measured 

     //   *((uint32_t *)0x40200008) |= (1<<16); //Trigger stop for Timer0 in TCPWM_CMD REgister 
       	
      //  Time_CodeProfile = *((uint32_t *)0x40200108);
      
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
    // *((uint32_t *)0x40200108) = 0; //Clear the counter register of  TCPWM0 TCPWM_CNT0_COUNTER Register
    // *((uint32_t *)0x40200008) |= (1<<24); //Trigger start for Timer0 in TCPWM_CMD REgister 
    // while (delay_uS > (*((uint32_t *)0x40200108)));// Keep Checking the TCPWM_CNT0_COUNTER Register 
    while (delay_uS > TCPWM_GetCounter(0)); // Keep Checking the TCPWM_CNT2_COUNTER Register
    TCPWM_Stop(0); //Stop the Timer
}

// /*Timer 2 interrupt*/
// void tcpwm_interrupts_2_IRQHandler(void)
// {
//     TCPWM_ClearInterrupt(2, 1);
//     GPIO_Inv(1, 4u);
//     GPIO_Inv(1, 6u);
//     // /* Clear the terminal count interrupt */
//     // *((uint32_t *)0x402001B0) |= (1<<0); //clear interrupt  interrupt in TCPWM_CNT2_INTR
//     /* Toggle the LED */
//     // *((uint32_t *)0x40040148) |= (1<<4); //Toggle the LED on P1.4 by inverting in GPIO_PRT1_DR_INV Register 
// }
/*Peripheral clock initilizations*/
void init_peri_Clock_Config()
{
    //TIMER 0 for Code profiling
    *((uint32_t *)0x40010000) = (1<<30); // Disable the Divider using PERI_DIV_CMD 

    *((uint32_t *)0x40010304) = (24 - 1) << 8 ; //Set the divider value in PERI_DIV_16_CTL1, We are configuring Divider 1 

    *((uint32_t *)0x40010000) |= (1<<31) |(3<<14) |(63<<8) |(1<<6) | (1 << 0); //PERI_DIV_CMD 
    //Enable the divder 31:bit, Keep 3 at 15:14 and 63 13:8 this selects the HFCLK as reference , Select 16 bit divider 7:6, and Select the divider no 1 using 5:0;

    *((uint32_t *)0x40010118) = (1<<6)|(1<<0); // Specify Divider type 7:6 and Selected Divider 3:0 in register PERI_PCLK_CTL6 TCPWM0 is PERIPHERAL 6

}