#include "irq_mgmt.h"
#include <stdint.h>

/*
 * Uses the Thumb-2 "CPSIE i" instruction.
 */
void IRQ_EnableGlobal(void) {
    __asm volatile ("cpsie i" : : : "memory");
}

/*
 * Uses the Thumb-2 "CPSID i" instruction.
 */
void IRQ_DisableGlobal(void) {
    __asm volatile ("cpsid i" : : : "memory");
}

void NVIC_SetPriority(uint32_t IRQn, uint32_t priority)
{
    if ((int32_t)(IRQn) >= 0){
        uint32_t IRQ_LDX = IRQn >> 2u;
        NVIC->IPR[IRQ_LDX] = (NVIC->IPR[IRQ_LDX] & ~(0xFFUL << ((IRQn & 0x03) << 3))) | (((priority & 0x03) << 6) << ((IRQn & 0x03) << 3));
    }
}

void NVIC_ClearPendingIRQ(uint32_t IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->ICPR = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
  }
}

void NVIC_EnableIRQ(uint32_t IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    __asm volatile("":::"memory");
    NVIC->ISER = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
    __asm volatile("":::"memory");
  }
}

void NVIC_DisableIRQ(uint32_t IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->ICER = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
    __asm volatile ("dsb 0xF":::"memory");
    __asm volatile ("isb 0xF":::"memory");
  }
}



