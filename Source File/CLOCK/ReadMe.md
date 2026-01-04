## PSoC™ 4 Clocking System

The PSoC™ 4 clock system provides flexible clock generation and distribution for the CPU and peripherals.  
It supports multiple internal and external clock sources, which are used to derive the system and peripheral clocks.


## Clock Sources

### Internal Clock Sources

PSoC™ 4 provides the following internal clock sources:

- **Internal Main Oscillator (IMO)**  
  Frequency range: **24–48 MHz**

- **Internal Low-Speed Oscillator (ILO)**  
  Frequency: **40 kHz**


### External Clock Sources

PSoC™ 4 also supports the following external clock sources:

- **EXTCLK**  
  External clock generated using a signal from an I/O pin

- **ECO (External Crystal Oscillator)**  
  External crystal oscillator with frequency range **4–33 MHz**

- **WCO (Watch Crystal Oscillator)**  
  External **32-kHz** watch crystal oscillator


## Derived Clock Signals

Using the above clock sources, the following clocks are generated:


### High-Frequency Clock (HFCLK)

- Maximum frequency: **48 MHz**
- Can be sourced from:
  - IMO
  - ECO
  - EXTCLK
  - PLL

HFCLK is the primary high-speed clock used in the system.


### Low-Frequency Clock (LFCLK)

- Used for low-power and timing-related functions
- Can be sourced from:
  - ILO
  - WCO


### System Clock (SYSCLK)

- Derived from **HFCLK**
- Maximum frequency: **48 MHz**
- Uses a **dedicated prescaler** to generate the required system clock frequency

SYSCLK is used to clock the CPU and core system logic.


## Peripheral Clocking

The **HFCLK** is used to clock peripherals through:

- **Peripheral clock dividers**
- **Dedicated peripheral clocks (PERICLK)**

Each peripheral can be clocked independently by selecting appropriate dividers, allowing flexible performance and power optimization.


## Default Clock Configuration

In this setup, we are **not modifying the default clock configuration**.

By default, the **Internal Main Oscillator (IMO)** is set to **24 MHz**.  
This clock is directly selected as the **High-Frequency Clock (HFCLK)** without any modification.

The HFCLK is then used further in the system through **clock dividers**.


## Peripheral Clock Dividers

PSoC™ 4 provides multiple peripheral clock dividers to generate different clock frequencies for peripherals.

The available peripheral clock dividers are:

- **Twelve 16-bit integer dividers**
- **Six fractional dividers**, which include:
  - **Five 16.5-bit fractional dividers**
  - **One 24.5-bit fractional divider**

Fractional dividers are used when accurate or non-integer clock frequencies are required.


## Divider Usage in This Configuration

In this configuration:

- Only the **12 × 16-bit integer peripheral clock dividers** are enabled
- Fractional dividers are **not used**
- The peripheral clocks are derived directly from **HFCLK (24 MHz)** using the integer dividers

This keeps the clock configuration **simple and predictable**, while still providing sufficient flexibility for peripheral operation.


> **Note:**  
> Please refer to **Table 9-11: PSoC 4100S and PSoC 4100S Plus Devices Peripheral Clock Multiplexer Output Mapping**  
> for understanding how the **peripheral clocks are mapped to dedicated peripherals**.


> **Additional Information:**  
> The current implementation uses only the **default clock source and integer clock dividers**.  
>  
> Additional features such as:
> - Changing the clock frequency  
> - Selecting a different clock source  
> - Using fractional clock dividers  
>  
> can be added by **extending the clock header file**.  
>  
> At present, these features are **not implemented**.
>  
> Refer to the **example files** for a better understanding of the clock configuration and usage.



