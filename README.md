# Pulse Width Modulation (PWM) and Hardware Timers 

In this lecture we are going to look a bit deeper at some of the hardware peripherals within our ESP32 devices. In particular, we will explore the Pulse Width Modulation (PWM) hardware peripherals and the hardware timer modules.


# Digital-to-Analogue conversion
There are two main ways to perform digital-to-analogue conversion:

* Using a resistor-ladder network.
* Using a PWM module

A resistor-ladder based Digital-to-Analogue Converter (DAC) uses a chain of resistors to produce a current that is weighted based on the significance of the bit that is fed into the DAC. In the last lecture, we explored how analogue to digital conversion is performed with the FlashADC circuit, you can think of the resistor-ladder network DAC as the inverse of this.

A PWM-based DAC is different; unlike the previous approach where the output voltage can vary between a range of values, the output of a PWM can only be high or low. However, what changes is the amount of time the output spends high or low depending on the output of the circuit. Varying the rate at which the circuit is switched on and off has the effect of varying the average voltage, and therfor average power consumption, experienced at the output pin. Essentially a PWM generates a simulated analogue signal. PWM based approaches are frequently the most common seen in microcontrollers, and are typically used to control LEDs, motors, and other external actuators.  

Let's look at this is a bit more detail:


## R-2R resistor ladder network

![](imgs/2r-r-ladder.gif)

A resistor-ladder based DAC will produce a varying output voltage at discrete intervals between 0V and the supply. Essentially how this works is that 

__TODO: think a nice way to explain this!__


