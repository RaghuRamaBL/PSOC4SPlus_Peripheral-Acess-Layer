## ADC (SAR ADC)

The **PSoC® 4** device includes one **Successive Approximation Register Analog-to-Digital Converter (SAR ADC)**.

The SAR ADC core is a **fast 12-bit ADC** with a maximum sampling rate of **1 Msps**.


## SAR ADC Architecture

Preceding the SAR ADC core is the **SARMUX**, which is used to route input signals to the ADC.

The SARMUX can route:
- External pins
- Internal signals such as:
  - **AMUXBUS-A / AMUXBUS-B**
  - **CTBm**
  - **Temperature sensor output**

These signals are routed to the **16 internal channels** of the SAR ADC.

The **SARREF** block is used to select the ADC reference voltage and supports multiple reference options.


## ADC Channels

The SAR ADC provides **sixteen individually configurable channels**.

Each channel supports the following features:

- Input from:
  - External pin  
    - Only **eight channels** support single-ended mode  
    - Only **four channels** support differential mode  
  - Internal signals (AMUXBUS / CTBm / temperature sensor)

- **Programmable acquisition time**
- **Selectable resolution**:
  - 8-bit
  - 10-bit
  - 12-bit
- **Single-ended or differential measurement**
- **Averaging**
- **Double-buffered results**
- **Left or right result alignment**

## SAR ADC Configuration Fields

This section describes key SAR ADC configuration fields used while configuring ADC channels.


## VREF_SEL — SAR ADC Reference Selection

The **VREF_SEL** field is used to select the reference voltage for the SAR ADC.

**Default Value:** 0

The supported reference selections are:

- **0x0: VREF0**  
  Reserved

- **0x1: VREF1**  
  Reserved

- **0x2: VREF2**  
  Reserved

- **0x3: VREF_AROUTE**  
  Reserved

- **0x4: VBGREF**  
  Internal bandgap reference (1.2 V, VREF buffer ON)

- **0x5: VREF_EXT**  
  External reference from pin **P1[7]**

- **0x6: VDDA_DIV_2**  
  VDDA / 2 (VREF buffer ON)

- **0x7: VDDA**  
  VDDA used as reference voltage


## NEG_SEL — Negative Input Selection

The **NEG_SEL** field is used to select the **negative input** for single-ended ADC conversion.

**Default Value:** 0

The supported negative input selections are:

- **0x0: VSSA_KELVIN**  
  Negative input connected to internal VSSA

- **0x1: ART_VSSA**  
  Reserved

- **0x2: P1**  
  Negative input connected to **P2[1]** if SARMUX is used

- **0x3: P3**  
  Negative input connected to **P2[3]** if SARMUX is used

- **0x4: P5**  
  Negative input connected to **P2[5]** if SARMUX is used

- **0x5: P7**  
  Negative input connected to **P2[7]** if SARMUX is used

- **0x6: ACORE**  
  Reserved

- **0x7: VREF**  
  Selected SAR ADC VREF used as negative input


## PORT_ADDR — Input Port Selection

The **PORT_ADDR** field selects the port that contains the pin to be sampled by the ADC channel.

**Default Value:** 0

The supported port selections are:

- **0x0: SARMUX**  
  SARMUX pins (Port 2)

- **0x1: CTB0**  
  CTBm 0

- **0x2: CTB1**  
  Reserved

- **0x3: CTB2**  
  Reserved

- **0x4: CTB3**  
  Reserved

- **0x5: AROUTE_VIRT2**  
  Reserved

- **0x6: AROUTE_VIRT1**  
  Reserved

- **0x7: SARMUX_VIRT**  
  SARMUX virtual port (**VPORT0**)  
  Used for the on-chip temperature sensor

> **Note:**  
> The SAR ADC provides many advanced features.  
> In the current implementation, only the **basic configuration options** required to use the ADC functionality are enabled.  
>  
> Additional features can be added and extended on top of this implementation as required.  
>  
> Refer to the **code examples** to gain a better understanding of how the ADC header and APIs are used.
