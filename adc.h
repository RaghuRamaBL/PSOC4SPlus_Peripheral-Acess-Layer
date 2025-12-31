#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

typedef struct {
    __IOM uint32_t CTRL;                /*!< 0x0000 Global control */
  __IOM uint32_t SAMPLE_CTRL;         /*!< 0x0004 Sample control */
  
  /* Gap: 0x0008 to 0x000F (8 bytes) */
  uint32_t       RESERVED0[2];        /*!< 0x0008 Reserved */
  
  __IOM uint32_t SAMPLE_TIME01;       /*!< 0x0010 Sample time 0 & 1 */
  __IOM uint32_t SAMPLE_TIME23;       /*!< 0x0014 Sample time 2 & 3 */
  __IOM uint32_t RANGE_THRES;         /*!< 0x0018 Range threshold */
  __IOM uint32_t RANGE_COND;          /*!< 0x001C Range condition */
  __IOM uint32_t CHAN_EN;             /*!< 0x0020 Channel enable */
  __IOM uint32_t START_CTRL;          /*!< 0x0024 Start conversion */
  
  /* Gap: 0x0028 to 0x002F (8 bytes) */
  uint32_t       RESERVEDLines[2];    /*!< 0x0028 Reserved */
  
  __IOM uint32_t DFT_CTRL;            /*!< 0x0030 DFT and test control */
  
  /* Gap: 0x0034 to 0x007F (76 bytes)
     Calculation: (0x0080 - 0x0034) = 0x4C bytes = 19 uint32_t elements */
  uint32_t       RESERVED1[19];       /*!< 0x0034 Reserved padding to 0x0080 */

  /* Channel Configuration Array */
  __IOM uint32_t CHAN_CONFIG[16];     /*!< 0x0080-0x00BC (Ch 0 to 15) */
  
  /* Gap: 0x00C0 to 0x00FF (64 bytes)
     Calculation: (0x0100 - 0x00C0) = 0x40 bytes = 16 uint32_t elements */
  uint32_t       RESERVED2[16];       /*!< 0x00C0 Reserved padding to 0x0100 */

  /* Working Data Array (Result of most recent conversion) */
  __IM  uint32_t CHAN_WORK[16];       /*!< 0x0100-0x013C (Ch 0 to 15) */

  /* Gap: 0x0140 to 0x017F (64 bytes)
     Calculation: (0x0180 - 0x0140) = 0x40 bytes = 16 uint32_t elements */
  uint32_t       RESERVED3[16];       /*!< 0x0140 Reserved padding to 0x0180 */

  /* Final Result Data Array */
  __IM  uint32_t CHAN_RESULT[16];     /*!< 0x0180-0x01BC (Ch 0 to 15) */

  /* Gap: 0x01C0 to 0x01FF (64 bytes)
     Calculation: (0x0200 - 0x01C0) = 0x40 bytes = 16 uint32_t elements */
  uint32_t       RESERVED4[16];       /*!< 0x01C0 Reserved padding to 0x0200 */

  /* Validation and Status Registers */
  __IM  uint32_t CHAN_WORK_VALID;     /*!< 0x0200 Work data valid bits */
  __IM  uint32_t CHAN_RESULT_VALID;   /*!< 0x0204 Result data valid bits */
  __IM  uint32_t STATUS;              /*!< 0x0208 ADC status */
  __IM  uint32_t AVG_STAT;            /*!< 0x020C Averaging status */

  /* Primary Interrupt Management */
  __IOM uint32_t INTR;                /*!< 0x0210 Interrupt request */
  __IOM uint32_t INTR_SET;            /*!< 0x0214 Interrupt set */
  __IOM uint32_t INTR_MASK;           /*!< 0x0218 Interrupt mask */
  __IM  uint32_t INTR_MASKED;         /*!< 0x021C Interrupt masked status */

  /* Saturation Interrupts */
  __IOM uint32_t SATURATE_INTR;       /*!< 0x0220 Saturation interrupt */
  __IOM uint32_t SATURATE_INTR_SET;   /*!< 0x0224 Saturation set */
  __IOM uint32_t SATURATE_INTR_MASK;  /*!< 0x0228 Saturation mask */
  __IM  uint32_t SATURATE_INTR_MASKED;/*!< 0x022C Saturation masked status */

  /* Range Interrupts */
  __IOM uint32_t RANGE_INTR;          /*!< 0x0230 Range interrupt */
  __IOM uint32_t RANGE_INTR_SET;      /*!< 0x0234 Range set */
  __IOM uint32_t RANGE_INTR_MASK;     /*!< 0x0238 Range mask */
  __IM  uint32_t RANGE_INTR_MASKED;   /*!< 0x023C Range masked status */

  __IM  uint32_t INTR_CAUSE;          /*!< 0x0240 Combined interrupt cause */

/* Gap: 0x0244 to 0x027F (60 bytes)
     Calculation: (0x0280 - 0x0244) = 0x3C bytes = 15 uint32_t elements */
  uint32_t       RESERVED5[15];       /*!< 0x0244 Reserved padding to 0x0280 */

  /* Injection Channel Registers */
  __IOM uint32_t INJ_CHAN_CONFIG;     /*!< 0x0280 Injection config */
  
  /* Gap: 0x0284 to 0x028F (12 bytes) = 3 uint32_t elements */
  uint32_t       RESERVED6[3];        /*!< 0x0284 Reserved padding to 0x0290 */
  
  __IM  uint32_t INJ_RESULT;          /*!< 0x0290 Injection result */

/* Gap: 0x0294 to 0x02FF (108 bytes) 
     Calculation: (0x0300 - 0x0294) = 0x6C bytes = 27 uint32_t elements */
  uint32_t       RESERVED7[27];       /*!< 0x0294 Reserved padding to 0x0300 */

  /* Mux and Switch Control Registers */
  __IOM uint32_t MUX_SWITCH0;         /*!< 0x0300 Mux switch control 0 */
  __IOM uint32_t MUX_SWITCH_CLEAR0;   /*!< 0x0304 Mux switch clear 0 */
  __IOM uint32_t MUX_SWITCH1;         /*!< 0x0308 Mux switch control 1 */
  __IOM uint32_t MUX_SWITCH_CLEAR1;   /*!< 0x030C Mux switch clear 1 */

  /* Gap: 0x0310 to 0x033F (48 bytes)
     Calculation: (0x0340 - 0x0310) = 0x30 bytes = 12 uint32_t elements */
  uint32_t       RESERVED8[12];       /*!< 0x0310 Reserved padding to 0x0340 */

  __IOM uint32_t MUX_SWITCH_HW_CTRL;  /*!< 0x0340 Mux switch hardware control */
  
  /* Gap: 0x0344 to 0x0347 (4 bytes) = 1 uint32_t element */
  uint32_t       RESERVED9[1];        /*!< 0x0344 Reserved padding to 0x0348 */
  
  __IM  uint32_t MUX_SWITCH_STATUS;   /*!< 0x0348 Mux switch status */

  /* Gap: 0x034C to 0x037F (52 bytes)
     Calculation: (0x0380 - 0x034C) = 0x34 bytes = 13 uint32_t elements */
  uint32_t       RESERVED10[13];      /*!< 0x034C Reserved padding to 0x0380 */

  __IOM uint32_t PUMP_CTRL;           /*!< 0x0380 Charge pump control */

  /* Massive Gap: 0x0384 to 0x0EFF (2940 bytes)
     Calculation: (0x0F00 - 0x0384) = 0xB7C bytes = 735 uint32_t elements */
  uint32_t       RESERVED11[735];     /*!< 0x0384 Reserved padding to 0x0F00 */

  /* Factory Trim and Wounding Registers */
  __IOM uint32_t ANA_TRIM;            /*!< 0x0F00 Analog trim register */
  __IM  uint32_t WOUNDING;            /*!< 0x0F04 Wounding register */
} SAR_Type;

#define ADC_BASE  0x403A0000u
#define ADC       ((SAR_Type *)ADC_BASE)

#define ADC_CTRL_MASK               0xC0000EF0UL
#define ADC_CHANAL_CONFIG_MASK      0x00003277UL


void ADC_Init(uint8_t VrefSelect, uint8_t PositiveInput, uint8_t NegativeInput, uint8_t ContinuousMode);
uint16_t ADC_Read(uint8_t channel);
void ADC_StartConversion(void);
void ADC_ChannelConfig(uint8_t channel, uint8_t subresolution, uint8_t sampleTime, uint8_t PortSelect, uint8_t PinNumber);
void ADC_SetSubResolution(uint8_t resolution);
void ADC_ChannelEnable(uint8_t channel);
void ADC_ChannelDisable(uint8_t channel);


#endif 