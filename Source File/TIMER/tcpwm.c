#include <stdint.h>
#include "tcpwm.h"

void TCPWM_Init(uint8_t tcpwm_Num, TCPWM_Config_t *config)
{
    uint32_t mask;
    if(tcpwm_Num > 7){
        return;
    }
    TCPWM_CNT_Type* tcpwm;
    tcpwm = GET_TCPWM_NUM(tcpwm_Num);
    
    TCPWM_Disable(tcpwm_Num);
    TCPWM_SetCounter(tcpwm_Num, 0u);
    
    mask = (0x7u << 24u) | (0x7u << 8u);
    tcpwm->CTRL = (tcpwm->CTRL & ~mask) | ((config->mode & 0x07u) << 24u) | ((config->prescaler & 0x07u) << 8u);
    if(config->mode == 4u){
        tcpwm->TR_CTRL2 = 0x31; 
    }
    tcpwm->PERIOD = config->period - 1u;
    if (config->compare == 0u) {
        tcpwm->CC = 0xFFFFu;
    } else {
        tcpwm->CC = config->compare - 1u;
    }
    tcpwm->INTR_MASK = config->intrMask & 0x03;
    TCPWM_Enable(tcpwm_Num);
}

void TCPWM_Enable(uint8_t tcpwm_Num)
{
    if(tcpwm_Num > 7){
        return;
    }
    TCPWM->CTRL |= (1u << tcpwm_Num);
}

void TCPWM_Disable(uint8_t tcpwm_Num)
{
    if(tcpwm_Num > 7){
        return;
    }
    TCPWM->CTRL &= ~(1u << tcpwm_Num);
}

void TCPWM_Start(uint8_t tcpwm_Num)
{
    if(tcpwm_Num > 7){
        return;
    }
    TCPWM->CMD |= (1u << (24u + tcpwm_Num));
}

void TCPWM_Stop(uint8_t tcpwm_Num)
{
    if(tcpwm_Num > 7){
        return;
    }
    TCPWM->CMD |= (1u << (16u + tcpwm_Num));
}

uint32_t TCPWM_GetCounter(uint8_t tcpwm_Num)
{
    if(tcpwm_Num > 7){
        return 0;
    }
    TCPWM_CNT_Type* tcpwm;
    tcpwm = GET_TCPWM_NUM(tcpwm_Num);
    return (uint32_t)(tcpwm->COUNTER & 0x0000FFFFu);
}

void TCPWM_SetCounter(uint8_t tcpwm_Num, uint32_t count)
{
    if(tcpwm_Num > 7){
        return;
    }
    TCPWM_CNT_Type* tcpwm;
    tcpwm = GET_TCPWM_NUM(tcpwm_Num);
    tcpwm->COUNTER = count;
}

void TCPWM_ClearInterrupt(uint8_t tcpwm_Num, uint8_t intrMask)
{
    if(tcpwm_Num > 7){
        return;
    }
    TCPWM_CNT_Type* tcpwm;
    tcpwm = GET_TCPWM_NUM(tcpwm_Num);
    tcpwm->INTR = intrMask & 0x03;
}

void TCPWM_SetCompare(uint8_t tcpwm_Num, uint32_t compare)
{
    if(tcpwm_Num > 7){
        return;
    }
    TCPWM_CNT_Type* tcpwm;
    tcpwm = GET_TCPWM_NUM(tcpwm_Num);
    
    TCPWM_Disable(tcpwm_Num);
    TCPWM_SetCounter(tcpwm_Num, 0u);
    if (compare == 0u) {
        tcpwm->CC = 0xFFFFu;
    } else {
        tcpwm->CC = compare - 1u;
    }
    TCPWM_Enable(tcpwm_Num);
    TCPWM_Start(tcpwm_Num);

}