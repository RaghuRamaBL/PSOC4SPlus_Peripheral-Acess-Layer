#include <stdint.h>
#include "gpio.h"

void GPIO_Set(uint8_t portNum, uint32_t pinNum)
{
    GPIO_PRT_Type* base;
    base = GET_GPIO_BASE(portNum);
    GPIO_PRT_OUT_SET(base) = GPIO_DR_MASK << pinNum;
}

void GPIO_Clr(uint8_t portNum, uint32_t pinNum)
{
    GPIO_PRT_Type* base;
    base = GET_GPIO_BASE(portNum);
    GPIO_PRT_OUT_CLR(base) = GPIO_DR_MASK << pinNum;
}

void GPIO_Inv(uint8_t portNum, uint32_t pinNum)
{
    GPIO_PRT_Type* base;
    base = GET_GPIO_BASE(portNum);
    GPIO_PRT_OUT_INV(base) = GPIO_DR_MASK << pinNum;
}

uint32_t GPIO_Read(uint8_t portNum, uint32_t pinNum)
{
    GPIO_PRT_Type* base;
    base = GET_GPIO_BASE(portNum);
    return (GPIO_PRT_IN(base) >> (pinNum)) & GPIO_PS_MASK;
}

void GPIO_SetDrivemode(uint8_t portNum, uint32_t pinNum, uint32_t value)
{
    uint32_t tempReg;
    uint32_t pinLoc;

    GPIO_PRT_Type* base;
    base = GET_GPIO_BASE(portNum);

    pinLoc = pinNum * GPIO_DRIVE_MODE_OFFSET;
    tempReg = ((((GPIO_PRT_Type*)(base))->PC) & ~(GPIO_PC_DM_MASK << pinLoc));
    (((GPIO_PRT_Type*)(base))->PC) = tempReg | ((value & GPIO_PC_DM_MASK) << pinLoc);

    tempReg = ((((GPIO_PRT_Type*)(base))->PC2) & ~(GPIO_PC_DM_IBUF_MASK << pinNum));
    (((GPIO_PRT_Type*)(base))->PC2) = tempReg | (((value & GPIO_DM_VAL_IBUF_DISABLE_MASK) >> GPIO_DRIVE_MODE_OFFSET) << pinNum);
}


void GPIO_SetHSIOM(uint8_t portNum, uint32_t pinNum, uint8_t value)
{
    uint32_t HSIOM_portNum;
    uint32_t tempReg;
    HSIOM_PRT_Type* portAddrHSIOM;
    
    GPIO_PRT_Type* base;
    base = GET_GPIO_BASE(portNum);

    HSIOM_portNum = ((uint32_t)(base) - 0x40040000UL) / 0x00000100UL;
    portAddrHSIOM = (HSIOM_PRT_Type*)(0x40020000UL + (0x00000100UL * HSIOM_portNum));

    tempReg = (((HSIOM_PRT_Type *)(portAddrHSIOM))->PORT_SEL) & ~(GPIO_HSIOM_MASK << (pinNum << GPIO_HSIOM_OFFSET));
    (((HSIOM_PRT_Type *)(portAddrHSIOM))->PORT_SEL) = tempReg | (( (uint32_t) value & GPIO_HSIOM_MASK) << (pinNum << GPIO_HSIOM_OFFSET));
}

void GPIO_SetInterruptEdge(uint8_t portNum, uint32_t pinNum, uint32_t value)
{
    uint32_t tempReg;
    uint32_t pinLoc;

    GPIO_PRT_Type* base;
    base = GET_GPIO_BASE(portNum);

    pinLoc = pinNum << 0x1u;
    tempReg = (((GPIO_PRT_Type*)(base))->INTR_CFG) & ~(0x3u << pinLoc);
    (((GPIO_PRT_Type*)(base))->INTR_CFG) = tempReg | ((value & 0x3u) << pinLoc);
}

void GPIO_Pin_Init(uint8_t portNum, uint32_t pinNum, const gpio_pin_config_t *config, uint8_t hsiom)
{
    ((config -> outVal) == 0) ? GPIO_Clr(portNum, pinNum) : GPIO_Set(portNum,pinNum);
    GPIO_SetDrivemode(portNum, pinNum, config->driveMode);
    GPIO_SetHSIOM(portNum, pinNum, hsiom);
    GPIO_SetInterruptEdge(portNum, pinNum, config->intEdge);
}

void GPIO_ClearInterrupt(uint8_t portNum, uint32_t pinNum)
{
    GPIO_PRT_Type* base;
    base = GET_GPIO_BASE(portNum);
    /* Any INTR MMIO registers AHB clearing must be preceded with an AHB read access */
    (void)(((GPIO_PRT_Type*)(base))->INTR);

    (((GPIO_PRT_Type*)(base))->INTR) = (0x01UL) << pinNum;

    /* This read ensures that the initial write has been flushed out to the hardware */
    (void)(((GPIO_PRT_Type*)(base))->INTR);
}