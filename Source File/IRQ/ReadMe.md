## Interrupt Enable Levels

In the **PSoC™ 4100S Plus**, interrupt enabling is done at **three levels**:

1. **Peripheral level**
2. **NVIC level**
3. **CPU level**

All three levels must be enabled for an interrupt to be serviced.


## Peripheral-Level Interrupt Enable

At the peripheral level, the interrupt source must be configured and enabled inside the peripheral itself  
(for example, configuring the GPIO interrupt edge and enabling the interrupt for that pin).

If the peripheral interrupt is not enabled, the interrupt request will never reach the NVIC.


## NVIC (Nested Vectored Interrupt Controller)

The **Nested Vectored Interrupt Controller (NVIC)** manages all interrupt requests coming from peripherals and forwards them to the CPU.

In **PSoC™ 4 S+**, the NVIC provides registers to **individually enable and disable 28 interrupts** in software.

These interrupts are numbered:
- **IRQ0 to IRQ27**


### NVIC Interrupt Priority

The priority of the 28 interrupts (**IRQ0 to IRQ27**) can be configured.

- Supported priority levels: **0 to 3**
- **0** → Highest priority  
- **3** → Lowest priority  

Interrupt priority determines which interrupt is serviced first when multiple interrupts occur simultaneously.


## CPU-Level Interrupt Enable

At the CPU level, **global interrupts must be enabled**.

If global interrupts are disabled, the CPU will not respond to any interrupt requests, even if both the peripheral and NVIC levels are enabled.


> **Note:**  
> Refer to **Table 6-3: PSoC 4100S Plus Interrupt Sources** in the  
> *PSoC 4100S and PSoC 4100S Plus – PSoC 4 Architecture Technical Reference Manual (TRM)*  
> for detailed information on interrupts and exception numbers.  
>  
> Refer to the **GPIO interrupt code examples** to understand the complete interrupt working flow.
