#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>

#define GPIO_BASE            0x40040000u
#define GPIO_PORT_STEP       0x00000100u

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

typedef struct {
  __IOM uint32_t DR;           /*!< 0x00: Data Register */
  __IM  uint32_t PS;           /*!< 0x04: Pad State Register */
  __IOM uint32_t PC;           /*!< 0x08: Configuration Register */
  __IOM uint32_t INTR_CFG;     /*!< 0x0C: Interrupt Config Register */
  __IOM uint32_t INTR;         /*!< 0x10: Interrupt Status Register */
  
  /* Gap 1: 0x14 is missing in your image list */
  __IM  uint32_t RESERVED0[1]; /*!< 0x14: Reserved */
  
  __IOM uint32_t PC2;          /*!< 0x18: Configuration Register 2 */
  
  /* Gap 2: From 0x1C to 0x3C (9 registers) */
  __IM  uint32_t RESERVED1[9]; /*!< 0x1C - 0x3C: Reserved */
  
  __IOM uint32_t DR_SET;       /*!< 0x40: Data Set Register */
  __IOM uint32_t DR_CLR;       /*!< 0x44: Data Clear Register */
  __IOM uint32_t DR_INV;       /*!< 0x48: Data Invert Register */
  
  /* Gap 3: Padding to reach the next port start (0x100) */
  /* 0x100 - 0x4C = 0xB4 bytes / 4 = 45 registers */
  __IM  uint32_t RESERVED2[45]; 
} GPIO_PRT_Type;


typedef struct
{
    uint32_t outVal;         /**< Pin output state */
    uint32_t driveMode;      /**< Drive mode */
    uint32_t intEdge;        /**< Interrupt Edge type */
} gpio_pin_config_t;

typedef struct {
    __IOM uint32_t PORT_SEL;       /*!< 0x00: HSIOM Port Selection Register */
    __IM  uint32_t RESERVED[63];   /*!< 0x04 - 0xFC: Padding to reach 256 bytes (0x100) */
} HSIOM_PRT_Type;

/* Total size of HSIOM_PRT_Type = 64 * 4 bytes = 256 bytes (0x100) */

/* Macro to get base pointer */
#define GET_GPIO_BASE(p)    ((GPIO_PRT_Type*)(GPIO_BASE + ((p) * GPIO_PORT_STEP)))

#define GPIO_PRT_OUT_CLR(base)             (((GPIO_PRT_Type*)(base))->DR_CLR)
#define GPIO_PRT_OUT_SET(base)             (((GPIO_PRT_Type*)(base))->DR_SET)
#define GPIO_PRT_OUT_INV(base)             (((GPIO_PRT_Type*)(base))->DR_INV)
#define GPIO_PRT_IN(base)                  (((GPIO_PRT_Type*)(base))->PS)

/* GPIO Masks */
#define GPIO_HSIOM_MASK                     (0x0FUL)   /**< HSIOM selection mask */
#define GPIO_DR_MASK                        (0x01UL)   /**< Single pin mask for DR register */
#define GPIO_PS_MASK                        (0x01UL)   /**< Single pin mask for PS register */
#define GPIO_PC_DM_MASK                     (0x07UL)   /**< Single pin mask for drive mode in PC register */
#define GPIO_PC_DM_IBUF_MASK                (0x01UL)   /**< Single pin mask for input buffer state in PC2 register */
#define GPIO_DM_VAL_IBUF_DISABLE_MASK       (0x08UL)   /**< The input buffer disable mask for the Pin Drive mode value */

/* Offsets */
#define GPIO_HSIOM_OFFSET                   (2UL)      /**< Offset for HSIOM */
#define GPIO_DRIVE_MODE_OFFSET              (3UL)      /**< Offset for Drive mode */

void GPIO_Set(uint8_t portNum, uint32_t pinNum);

void GPIO_Clr(uint8_t portNum, uint32_t pinNum);

void GPIO_Inv(uint8_t portNum, uint32_t pinNum);

uint32_t GPIO_Read(uint8_t portNum, uint32_t pinNum);

void GPIO_SetDrivemode(uint8_t portNum, uint32_t pinNum, uint32_t value);

void GPIO_SetHSIOM(uint8_t portNum, uint32_t pinNum, uint8_t value);

void GPIO_SetInterruptEdge(uint8_t portNum, uint32_t pinNum, uint32_t value);

void GPIO_Pin_Init(uint8_t portNum, uint32_t pinNum, const gpio_pin_config_t *config, uint8_t hsiom);

void GPIO_ClearInterrupt(uint8_t portNum, uint32_t pinNum);

#endif