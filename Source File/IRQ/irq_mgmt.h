#ifndef IRQ_MGMT_H
#define IRQ_MGMT_H

#include <stdint.h>

#define NVIC_BASE  0xE000E100u
#define NVIC      ((NVIC_Type *)NVIC_BASE)

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

/**
 * NVIC Register Map for PSoC 4S+ (Cortex-M0+)
 * Base Address: 0xE000E100
 */
typedef struct {
    __IOM uint32_t ISER;       /* Interrupt Set-Enable Register */
    uint32_t RESERVED0[31];
    __IOM uint32_t ICER;       /* Interrupt Clear-Enable Register */
    uint32_t RESERVED1[31];
    __IOM uint32_t ISPR;       /* Interrupt Set-Pending Register */
    uint32_t RESERVED2[31];       
    __IOM uint32_t ICPR;       /* Interrupt Clear-Pending Register */
    uint32_t RESERVED3[95];
    __IOM uint32_t IPR[8];     /* Interrupt Priority Registers */
} NVIC_Type;

/* Function Prototypes */
void IRQ_EnableGlobal(void);
void IRQ_DisableGlobal(void);
void NVIC_EnableIRQ(uint32_t IRQn);
void NVIC_ClearPendingIRQ(uint32_t IRQn);
void NVIC_SetPriority(uint32_t IRQn, uint32_t priority); // Up to 3 priority levels (0-3)
void NVIC_DisableIRQ(uint32_t IRQn);

#endif