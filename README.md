# ADC-in-STM32
ADC | STM32F103C8T6 - ARM Cortex M3 (KEIL IDE)

Blog post: https://embedded-electronicsproject.blogspot.com/2025/01/adc-stm32f103c8t6-arm-cortex-m3.html


ADC in STM32F103C8T6 is 12-bit and a successive approximation analog-to-digital converter.
 
It has up to 18 multiplexed channels allowing it measure signals from sixteen external and two internal sources. The result of the ADC is stored in a left-aligned or right-aligned 16-bit data register.


ADC converter supports several conversion modes:


1. Single mode, which converts only one channel, in single-shot or continuous mode. 

2. Scan mode, which converts a complete set of pre-defined programmed input channels, in single-shot or continuous mode.     

3. Discontinuous mode, converts only a single channel at each trigger signal from the list of pre-defined programmed input channels.

In each mode, there are regular and injected channels modes

- In regular mode, ADC read channels sequentially in a loop and convert them regularly.
  
- In injected mode conversion is triggered by an external event or by software. An injected conversion has higher priority in comparison to a regular conversion and  thus interrupts the regular conversions.

