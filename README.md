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

The most siginificant bits (MSBs) will contibute the most current, as the least significant bits will contribute the least (LSBs). They are also carefully weighted by the R-2R ladder network so that each bit will increase the current contribution by a power of 2.  

There are however, problems with this approach. Putting resistors in silicon is seriously expensive in both power and area. Resistors tend to dissapate lots of power as they resist current, and manufacturing them into the silicon requires special techniques, with additional manufacturing steps, that uses up lots of silicon resources. For these reasons, 2R-R ladder based DACs in microcontrollers tend to either be:

* Small -- on the ESP32 we only have two 8-bit DACs
* Non-exsistent, with microcontrollers generally only relying on PWM hardware instead. For instance, the Arduino Uno does not have a DAC, only PWMs.

## Example of using the DAC on the TinyPico

![](imgs/ESP32_Function_Block_Diagram_highlight_DAC.svg)

As mentioned above the ESP32 has two DAC channels that we can use that I'll quickly demonstrate.
For this course we wont go too deep into the configuration of the DAC hardware, instead we will use the Arduino ``dacWrite()`` libraries provided for interfacing with it.

However, if you are keen to explore further, then on page 637 of the [[ESP32 TRM](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)] details are given for the DAC hardware module. The ESP32 DAC has some interesting features, such as a cosine wave generator, which is a block of hardware that can be configured to generate cosine waves by itself.

![](imgs/tinypico_pinout_dac.jpg.svg)

From the pinout of the TinyPico we can see that pins 25 and 26 of the PCB have been connected up to the two DAC outputs. Let's try and use the ``dacWrite()`` function to drive this pin.

```C
const unsigned int dacPIN = 25;

void setup() {

}

uint8_t value = 0;

void loop() {
        value = value + 1;
        dacWrite(dacPIN, value); 
}
```

In this program we are using pin 25 as the DAC output channel and we are writing to it with the ``dacWrite()`` arduino function. From reading the [[ESP32 TRM](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)] we can see that the DAC accepts an 8-bit number as an input, hence, we are incrementing an 8-bit unsigned (``uint8_t value``) integer which we are using to drive the DAC. However, when we look at the output of 25 on an oscilliscope we get the following:

![](imgs/dac_saturation.png)

We can see that the voltage steadily increases as we increment the ``value`` variable, and that it wraps around when the overflow occurs. However, we can also see that on the high values the output saturates (stops increasing and stays level). I suspect the likely reason for this is that I am powering my TinyPico off 3.3v (instead of 5V) which is limiting the maximum voltage that we can output. After some experimenting and changing the ``loop()`` function to the following: 

```C
void loop() {
        value = value + 1;
        if(value >= 200)  
                value = 0;  
        dacWrite(dacPIN, value); 
}
```

We get the following oscilloscope output:

![](imgs/dac_good_range.png)

Which avoids the saturation region and looks much nicer.

### Generating a sine wave

To demonstrate this further I would like to use the DAC output to generate a sin wave. We can use the builtin Arduino ``sin`` function to do this.

```C
const unsigned int dacPin = 25;

const float conversionFactor = (2*PI)/200; 

void setup() {

}

uint8_t value=0;

void loop() {
        value = value + 1;
        if(value >= 200) 
                value = 0;

        dacWrite(dacPin, sin(value*conversionFactor)*100 + 100);
}
```

In the code above we are using turning the value signal into an angle (in radians) using the variable ``float conversionFactor``. Since we know we have 200 possible values, we have worked out what each unit of the ``2*PI`` circle we want to give each value. __TODO: better explanation here__. 
The next thing that we need to do is shift the signals up as our DAC will not understand negative values and ``sin`` will produce negative values. ``sin(value * conversionFactor)`` will produce a value between ``0-1`` so we multiply this by ``100`` and then add ``100`` to move it into the middle of the range. 

This produces the following output on the oscilliscope:

![](imgs/slow_sine.png)

The scope output looks okay, however, the frequency is a little low, just 300Hz. Any ideas how we can get it faster?

The culprit for limiting our output frequency is the ``sin(value * conversionFactor)*100 + 100`` bit of code. Trignometric functions, such as `sin`, are quite computationally expensive to calculate. On top of that floating point operations, such as ``value *conversionFactor``, are also quite expensive. One trick that you will see constantly used in embedded systems is to use a look up table.

A lookup table is essentially a precomputed table of values that you can use to lookup an answer to an expensive function. Think back to primary school when you would use multiplication tables to look up the answer to a problem, it's the same principle here. We can precompute the values for our expensive ``sin()`` operation in an array in the setup function, and then just use an array lookup to get the value we need in the ``loop()`` function.

```C
const unsigned int dacPIN = 25;

uint8_t sine_lookup[200];
const float conversionFactor = (2*PI)/200;

void setup() {
        float radAngle;
        for(unsigned int angle=0; angle<200; angle++){
                radAngle = angle*conversionFactor;
                sine_lookup[angle] = (sin(radAngle)*100) + 100;
        }
}

uint8_t idx = 0;

void loop() {
        idx = idx + 1;
        if(idx >= 200)
                idx = 0;
        dacWrite(dacPIN, sine_lookup[idx]);
}
```

The code about produces the following oscilloscope trace on pin 25:

![](imgs/fast_sine.png)

Which is now oscillating at 900Hz.

Sometimes embedded systems are quite constrained in terms of instruction space. For these reasons you will often see such lookup tables precomputed at compile time (rather than in the ``setup()`` function).

```C
const unsigned int dacPIN = 25;

uint8_t sine_lookup[200] = {100,103,106,109,112,115,118,121,124,127,130,133,136,139,142,145,148,150,153,156,158,161,163,166,168,170,172,175,177,179,180,182,184,186,187,189,190,191,192,194,195,196,196,197,198,198,199,199,199,199,200,199,199,199,199,198,198,197,196,196,195,194,192,191,190,189,187,186,184,182,180,179,177,175,172,170,168,166,163,161,158,156,153,150,148,145,142,139,136,133,130,127,124,121,118,115,112,109,106,103,99,96,93,90,87,84,81,78,75,72,69,66,63,60,57,54,51,49,46,43,41,38,36,33,31,29,27,24,22,20,19,17,15,13,12,10,9,8,7,5,4,3,3,2,1,1,0,0,0,0,0,0,0,0,0,1,1,2,3,3,4,5,7,8,9,10,12,13,15,17,19,20,22,24,27,29,31,33,36,38,41,43,46,49,51,54,57,60,63,66,69,72,75,78,81,84,87,90,93,96};

void setup() {

}

uint8_t idx = 0;

void loop() {
        idx = idx + 1;
        if(idx >= 200)
                idx = 0;
        dacWrite(dacPIN, sine_lookup[idx]); 
}
```

What is really cool about this, is that we can now generate arbitrary functions with a bit of C or Python, or Matlab, and generate arbitrary functions with out TinyPico. Without having to change any of our code, just the values that are loaded into the table.

The DAC is a really neat feature that is built into the ESP32, however, this is not common for microcontrollers. As mentioned eariler, including a resistor ladder in the silicon is expensive in terms of the area it uses up. It is also quite low resolution (8-bits), and is quite power hungry when in use. For higher fidelity DACs, for say audio output, generally one would use an external DAC. Typically these are communicated with using a communication protocol such as SPI and have a much higher resolution (but also significant power cost) [[example here](https://www.mouser.co.uk/new/texas-instruments/ti-dac8568-dacs/?gclid=CjwKCAiAyc2BBhAaEiwA44-wW9Pm1TTkPOvvbcvsJGESbIGZRq1hzLzL86xahTTJkDE292uPMku9MxoC8dQQAvD_BwE)].

Generally, when we want to generate an analogue signal from our microcontroller we will simulate an analogue output with something known as a pulse width modulation (PWM).

## Pulse Width Modulation (PWM)

A PWM signal is a digital signal that simulates an analogue one by varying the amount of time that it is spent high and low. If the PWM signal spends a higher porportion of it's time is high, then the average voltage on that output is high, simulating a higher analogue voltage. If the PWM signal spends most of it's time off, then the average voltage is lower, simulating a lower analogue voltage.

In [[Lecture 3](https://github.com/STFleming/EmSys_GPIO_and_Abstraction_Costs)] we looked at the speed at which we could pulse GPIO pins from software and I briefly mentioned something called the duty-cycle, let's recap that here.
