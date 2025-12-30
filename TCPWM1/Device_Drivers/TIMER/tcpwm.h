#ifndef TCPWM_H
#define TCPWM_H

#include <stdint.h>

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

typedef struct {
    __IOM uint32_t CTRL;            /*!< 0x00: Counter control: sets Mode (Timer/PWM), Prescaler, and Alignment */
    __IM  uint32_t STATUS;          /*!< 0x04: Counter status: indicates current count direction and running state */
    __IOM uint32_t COUNTER;         /*!< 0x08: Counter count: the current 16/32-bit counter value */
    __IOM uint32_t CC;              /*!< 0x0C: Counter compare/capture: holds the PWM duty cycle or captured timestamp */
    __IOM uint32_t CC_BUFF;         /*!< 0x10: Counter buffered compare/capture: double buffer for glitch-free updates */
    __IOM uint32_t PERIOD;          /*!< 0x14: Counter period: defines the overflow/wrap-around value (frequency) */
    __IOM uint32_t PERIOD_BUFF;     /*!< 0x18: Counter buffered period: double buffer for period updates */
    __IM  uint32_t RESERVED;        /*!< 0x1C: Reserved */
    __IOM uint32_t TR_CTRL0;        /*!< 0x20: Counter trigger control 0: selects input triggers (Start, Stop, etc.) */
    __IOM uint32_t TR_CTRL1;        /*!< 0x24: Counter trigger control 1: sets the edge detection for triggers */
    __IOM uint32_t TR_CTRL2;        /*!< 0x28: Counter trigger control 2: defines the state of the 'line' output signals */
    __IM  uint32_t RESERVED1;       /*!< 0x2C: Reserved */
    __IOM uint32_t INTR;            /*!< 0x30: Interrupt request: hardware sets bits here on TC or CC match */
    __IOM uint32_t INTR_SET;        /*!< 0x34: Interrupt set: software can force an interrupt by writing here */
    __IOM uint32_t INTR_MASK;       /*!< 0x38: Interrupt mask: enables/disables propagation to the NVIC */
    __IM  uint32_t INTR_MASKED;     /*!< 0x3C: Interrupt masked: logical AND of INTR and INTR_MASK */
} TCPWM_CNT_Type;   


typedef struct {
    __IOM uint32_t CTRL;            /*!< 0x00: Global control: enables/disables the clock for each counter block */
    __IM  uint32_t RESERVED;        /*!< 0x04: Reserved */
    __IOM uint32_t CMD;             /*!< 0x08: Global command: simultaneous Start/Stop/Reload of multiple counters */
    __IM  uint32_t INTR_CAUSE;      /*!< 0x0C: Global interrupt cause: identifies which counter (0-7) fired an IRQ */
    __IM  uint32_t RESERVED1[60];   /*!< 0x10 - 0xFC: Padding to align the first counter at offset 0x100 */
    TCPWM_CNT_Type CNT[8];          /*!< 0x100: Array of 8 individual counter modules (CNT0 - CNT7) */
} TCPWM_Type;

#define TCPWM_BASE                              0x40200000u
#define TCPWM                                   ((TCPWM_Type *)TCPWM_BASE) 
#define GET_TCPWM_NUM(p)                         ((TCPWM_CNT_Type*) &TCPWM->CNT[p])            

typedef struct {
    uint8_t  mode;        // 0=Timer, 2=capture, 4=PWM
    uint32_t period;      // The "Max" value (16-bit: 0 to 65535)
    uint32_t compare;     // For PWM duty cycle or Compare Match
    uint8_t  intrMask;    // Which events ring the CPU alarm (TC or CC) 0:Disable 1:TC, 2:CC
    uint8_t  prescaler;   // Divide clock by 1, 2, 4... up to 128
} TCPWM_Config_t;

void TCPWM_Init(uint8_t tcpwm_Num, TCPWM_Config_t *config);

void TCPWM_Enable(uint8_t tcpwm_Num);
void TCPWM_Disable(uint8_t tcpwm_Num);

void TCPWM_Start(uint8_t tcpwm_Num);
void TCPWM_Stop(uint8_t tcpwm_Num);

uint32_t TCPWM_GetCounter(uint8_t tcpwm_Num);
void TCPWM_SetCounter(uint8_t tcpwm_Num, uint32_t count);

void TCPWM_ClearInterrupt(uint8_t tcpwm_Num, uint8_t intrMask);






#endif