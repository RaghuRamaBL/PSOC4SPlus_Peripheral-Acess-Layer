#include <stdint.h>
#include "peri_clock.h"

void PeriClock_DisableDivider(uint8_t dividerType, uint32_t dividerNum)
{
    uint32_t tempreg;
    tempreg = PERI->DIV_CMD;
    tempreg &= ~(PERI_DISABLE_MSK);
    tempreg |= (1<<30) |(dividerType<<6) | (dividerNum << 0);
    PERI->DIV_CMD = tempreg;
}

void PeriClock_EnableDivider(uint8_t dividerType, uint32_t dividerNum)
{
    uint32_t tempreg;
    tempreg = PERI->DIV_CMD;
    tempreg &= ~(PERI_ENABLE_MSK);
    tempreg |= (1<<31) |(dividerType<<6) | (dividerNum << 0);
    PERI->DIV_CMD = tempreg;
}

void PeriClock_Set_16_Divider(uint32_t dividerNum, uint32_t dividerValue)
{
    PERI->DIV_16_CTL[dividerNum] = (dividerValue - 1) << 8;
}

void PeriClock_AssignDivider(uint8_t periphNum, uint8_t dividerType, uint32_t dividerNum)
{
    uint32_t tempreg;
    tempreg = PERI->PCLK_CTL[periphNum];
    tempreg &= ~(0xFFUL);
    tempreg |= (dividerType << 6) | dividerNum;
    PERI->PCLK_CTL[periphNum] = tempreg;    
}

