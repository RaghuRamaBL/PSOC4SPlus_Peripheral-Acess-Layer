#ifndef PERI_CLOCK_H
#define PERI_CLOCK_H

#include <stdint.h>

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

/**
 * @brief Peripheral Clock Control (PERI) Register Structure
 * Base Address: 0x40010000
 */
typedef struct {
    __IOM uint32_t DIV_CMD;                             /* 0x0000: Divider command register */
    uint32_t RESERVED0[63];                             /* 0x0004 - 0x00FC: Reserved padding */
    __IOM uint32_t PCLK_CTL[19];                        /* 0x0100: Peripheral clock control registers 0-18 */
    uint32_t RESERVED1[109];                            /* 0x014C - 0x02FC: Reserved padding */
    __IOM uint32_t DIV_16_CTL[12];                      /* 0x0300: 16-bit integer divider control 0-11 */
    uint32_t RESERVED2[52];                             /* 0x0330 - 0x03FC: Reserved padding */
    __IOM uint32_t DIV_16_5_CTL[5];                     /* 0x0400: 16.5-bit fractional divider control 0-4 */
    uint32_t RESERVED3[59];                             /* 0x0414 - 0x04FC: Reserved padding */
    __IOM uint32_t DIV_24_5_CTL[1];                     /* 0x0500: 24.5-bit fractional divider control 0 */
    uint32_t RESERVED4[63];                             /* 0x0504 - 0x05FC: Reserved padding */
    __IOM uint32_t TR_CTL;                              /* 0x0600: Trigger control register */
} PERI_Type;

#define PERI       ((PERI_Type *) 0x40010000)                 /* PERI base address pointer */

#define PERI_ENABLE_MSK                    0x800000FFUL
#define PERI_DISABLE_MSK                   0x400000FFUL

void PeriClock_DisableDivider(uint8_t dividerType, uint32_t dividerNum);
void PeriClock_EnableDivider(uint8_t dividerType, uint32_t dividerNum);
void PeriClock_Set_16_Divider(uint32_t dividerNum, uint32_t dividerValue);
void PeriClock_AssignDivider(uint8_t periphNum, uint8_t dividerType, uint32_t dividerNum);

#endif 