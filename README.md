# SoftLED-01

An Arduino program to make LEDs flash softly.

This tiny program is intended to be used with Danish model railroading. It is setup to control LEDs with a soft turn on/off and is suited for signals used for level crossings.

With this program low cost has been the most important objective. I have other projects underway with more advanced capabilities at a higher cost. The code should run on most Arduinos. It has been tested on Uno and Nano; Nano in particular can be obtained from China at very low cost.

The program allows direct connection of one or more LEDs (using proper resistors) to the PWM outputs of the Arduino. Uno/Nano has six such outputs, but the exact number varies from model to model. On Uno/Nano pin 3, 5, 6, 9, 10, and 11 can be used for PWM.

## Danish level crossings

A Danish level crossing without booms typically has a red flashing light only to warn people when a train is approaching.

If the allowed speed is more than 75 km/h, an unorder signal (Uordenssignal in Danish) is show next to the track. This signal has two fixed yellow lights when the crossing _isn't_ protected; this light is turned off when the warning lights are flashing.

Since 1986 the unorder signal has had an additional fixed white light to tell the train driver that the crossing is secured.

All these light combinations can be created with this program. It will turn the LEDs slowly on and off, and it will ensure that a flash sequence is completed before changing state even if the input is turned off in the middle of a flash.

## The program

This program needs a single input pin on the Arduino to control action. The input can be provided in many ways using a mechanical switch or a DCC accessory decoder for example. The pin can be set using a constant at the top of the code.

Likewise the PWM output pins for the LEDs can be selected. You can have a maximum of six PWM outputs, but you can also decrease the numbers should you want to use some of the outputs for other purposes. You can decrease the count to zero without interfering with the functionality; a use case could be for low speed branch lines without the unorder signal.

It can be beneficial to use more than one output pin for the same signal: An Arduino allows a maximum of 40 mA current drawn through any output, so using more than one output to ensure sufficient current will often be needed.

Besides the maximum current of 40 mA for each output, you should also observe the overall limit of 200 mA for all outputs combined.

The limits mentioned here are true for Uno and Nano. Similar - but different - limits exists for other models.

Below the pin settings in the code you'll find settings for timing of the various stages of the program.

## Examples

An example setup could be as shown in this Fritzing schematic:

![Schematic](/Schematic.png)

Components could be connected like this:

![Components](/Components.png)

You can run a test of the code supplying power directly from your computer using the USB cable. An external 5V power supply is highly recommended, though, when a lot of LEDs are connected. This writer has tried blowing up a laptop motherboard by drawing too much current from the USB port.
