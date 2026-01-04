## GPIO in PSoC™ 4100S Plus

The **PSoC™ 4100S Plus** supports **up to 54 GPIO pins**, distributed across **8 GPIO ports**.


## Key GPIO Configuration Focus Areas

While working with GPIO, we will mainly focus on the following three configurations:

- **Drive Mode**
- **Interrupt Edge**
- **HSIOM (High-Speed I/O Matrix)**


## GPIO Drive Modes

The **PSoC™ 4100S Plus** supports **eight GPIO drive modes**:

- **0x0: OFF**  
  Analog mode  
  Output buffer off (High-Z)  
  Input buffer off  

- **0x1: INPUT**  
  Output buffer off (High-Z)  
  Input buffer on  

- **0x2: PU**  
  Strong pull-down ('0')  
  Weak/resistive pull-up (PU)  
  Input buffer on  

- **0x3: PD**  
  Weak/resistive pull-down (PD)  
  Strong pull-up ('1')  
  Input buffer on  

- **0x4: OZ**  
  Strong pull-down ('0')  
  Open drain (pull-up off)  
  Input buffer on  

- **0x5: ZI**  
  Open drain (pull-down off)  
  Strong pull-up ('1')  
  Input buffer on  

- **0x6: I**  
  Strong pull-down ('0')  
  Strong pull-up ('1')  
  Input buffer on  

- **0x7: PD_PU**  
  Weak/resistive pull-down (PD)  
  Weak/resistive pull-up (PU)  
  Input buffer on  


## Interrupt Edge

**Interrupt edge** is used to define **how an interrupt is triggered on a GPIO pin**  
(i.e., when the interrupt should occur).

The following interrupt edge options are supported:

- **0x0: DISABLE**  
  Interrupt disabled  

- **0x1: RISING**  
  Interrupt triggered on a rising edge  

- **0x2: FALLING**  
  Interrupt triggered on a falling edge  

- **0x3: BOTH**  
  Interrupt triggered on both rising and falling edges  


## HSIOM (High-Speed I/O Matrix)

The **High-Speed I/O Matrix (HSIOM)** is a group of high-speed switches that routes GPIO pins to the internal peripherals of the device.

HSIOM determines whether a pin functions as:

- A **GPIO**
- Or is connected to a **peripheral signal** (UART, SPI, I2C, etc.)

The following HSIOM selection values are supported:

- **0x0: GPIO**  
  Software-controlled GPIO  

- **0x1: GPIO_DSI**  
  Software-controlled GPIO or DSI-controlled GPIO  

- **0x2: DSI_DSI**  
  DSI-controlled input and output  

- **0x3: DSI_GPIO**  
  DSI-controlled input, software-controlled output  

- **0x4: CSD_SENSE**  
  CSD sense connection (analog mode)  

- **0x5: CSD_SHIELD**  
  CSD shield connection (analog mode)  

- **0x6: AMUXA**  
  AMUXBUS A connection  

- **0x7: AMUXB**  
  AMUXBUS B connection  
  This mode is also used for CSD GPIO charging.  
  When CSD GPIO charging is enabled in `CSD_CONTROL`, the pin is connected to the *toad_charge* signal.  

- **0x8: ACT_0**  
  Chip-specific Active source 0 connection  

- **0x9: ACT_1**  
  Chip-specific Active source 1 connection  

- **0xA: ACT_2**  
  Chip-specific Active source 2 connection  

- **0xB: ACT_3**  
  Chip-specific Active source 3 connection  

- **0xC: LCD_COM**  
  LCD common connection  
  This mode provides DeepSleep functionality (the LCD block must be enabled and properly configured)  

- **0xD: LCD_SEG**  
  LCD segment connection  
  This mode provides DeepSleep functionality (the LCD block must be enabled and properly configured)  

- **0xE: DS_1**  
  Chip-specific DeepSleep source 1 connection  

- **0xF: DS_2**  
  Chip-specific DeepSleep source 2 connection  


> **Note:**  
> Please refer to **Table 2: Alternate Pin Functions** in the device datasheet for detailed information on pin selection.  
> Refer to the **example files** to understand the usage of the GPIO APIs.
