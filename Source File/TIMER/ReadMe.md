## TCPWM Operating Mode Selection

The TCPWM block supports multiple operating modes, which are selected using the mode configuration field.

## TCPWM (Timer, Counter, PWM) Block

In **PSoC™ 4 S+**, there are **up to 8 TCPWM blocks**, each of which can operate as a **16-bit timer, counter, or pulse-width modulator (PWM)**.

The TCPWM block is a flexible peripheral used for timing, event counting, and waveform generation.


## TCPWM Operating Modes

The TCPWM block supports the following operational modes:

- **0x0: TIMER**  
  Timer mode

- **0x2: CAPTURE**  
  Capture mode

- **0x3: QUAD**  
  Quadrature decoding mode

- **0x4: PWM**  
  Pulse-width modulation (PWM) mode

- **0x5: PWM_DT**  
  PWM with dead-time insertion mode

- **0x6: PWM_PR**  
  Pseudo-random pulse-width modulation mode


## Clocking and Prescaling

The TCPWM block supports **clock prescaling**, allowing the input clock to be divided by:

- **0x0: DIVBY1**  
  Divide by 1

- **0x1: DIVBY2**  
  Divide by 2

- **0x2: DIVBY4**  
  Divide by 4

- **0x3: DIVBY8**  
  Divide by 8

- **0x4: DIVBY16**  
  Divide by 16

- **0x5: DIVBY32**  
  Divide by 32

- **0x6: DIVBY64**  
  Divide by 64

- **0x7: DIVBY128**  
  Divide by 128

This allows fine control over the timer resolution and counting period.


## TCPWM Interrupt Support

The TCPWM block supports interrupts on the following events:

- **Terminal Count**  
  Triggered when the counter reaches its final value

- **Capture / Compare**  
  Triggered when:
  - The counter value is captured into the capture/compare register, or  
  - The counter value matches the compare value


## Additional TCPWM Features

- **Complementary line output** support for PWM generation  
- **Selectable start, reload, stop, count, and capture event signals** for flexible TCPWM control


> **Note:**  
> There are additional counter features that can be configured, such as **up-count mode**, **down-count mode**, and other advanced options.  
>  
> In the current implementation, only the features described above are enabled.  
> Further enhancements can be built on top of this in the future as required.  
>  
> Refer to the **code examples** to gain a better understanding of the TCPWM configuration and usage.

