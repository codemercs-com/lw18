# LED-Warrior18 - Dual 16 bit PWM controller with I²C interface
LED-Warrior18 is a two channel PWM controller for control of LED stripes and other constant voltage LEDs via I²C with any I²C host controller. You can use every platform for control the LW18 (Arduino, Raspberry PI, STMicrochip, Atmel, and many more) which supports I²C interface as a host.
&nbsp;

The LED outputs and VCC input are designed for terminal blocks, the I2C interface for a 4-pin header. You can  solder the wires direct on the solder pads or install terminal blocks and headers. The modules come without terminal blocks or headers. The controller needs a 5 V supply from the processor side. The I²C is compatible with 3.3 V.
&nbsp;

All information found in this page may be changed during the [KICKSTARTER](https://www.kickstarter.com/projects/thebug/led-dimming-done-right-pwm-with-i2c/description) campaign.
&nbsp;

---
&nbsp;
## Tested plattforms
The LW18 is designed to work with any plattform that has an I²C host function. In this repository you can find some sample projects for some of the most popular platforms.  

---
&nbsp;
## USB to I²C interface
With the [IO-Warrior Dongles](https://codemercs.com/de/dongles) I²C dongle we deliver our own USB to I²C interfaces. With our SDK you can use the LW18 on Windows, Linux or OSX. For more information please visit out [website](https://www.codemercs.com).

---
&nbsp;
## Features
The basic features of the LW18 module:
- I2C to dual PWM LED driver
- PWM output at 730 Hz
- 2 x 16 bit PWM ranging from 0.0015% to 100%
- 8 Bit data to log and lin mapping availabe
- Synch mode for controlling multiple units
- Default power on status progammable
- Minimal external circuitry
- 5 V supply

---
&nbsp;
## Usage
The LW18 uses the I²C interface for communication. The basic 7-bit I²C address is *0x20*, but can be changed by software. You can use multiple LW18 devices on the same I²C bus (max. 127 devices) by changing the device address.  
Based on your platform you have to write/read the I²C commands via the appropriate API (i.e. for Arduino 'Wire.h', for Raspberry PI with python 'SMBus'). Please refer to the documentation of your platform.  
The programming language can be selected based on your platform and preferences (C/C++, microPython, ASM).
If your platform does not have an I²C host included but has USB you can use our [IO-Warrior Dongle](https://codemercs.com/de/dongles) to have access to I²C and the LW18. 
&nbsp;

### Address register table
| Register Address | Byte Count | R/W | Description |
| ----------- | ----------- | ----------- | ----------- |
| 0x00 | 1 | R/W | Command |
| 0x01 | 4 | R/W | 2 channel 16 bit PWM|
| 0x02 | 2 | W | 2 channel 8 bit PWM |
| 0xF0 | 6 | R | Signature |
| 0xFE | 2 | W | Set I²C address |
&nbsp;

A detailed description of the registers will be in the datasheet of LW18 which is coming soon.

---
&nbsp;
## Technical details, maximum ratings and specifications of LED-Warrior18 module
| Type | Rating |
| ----------- | ----------- |
| LED Voltage (PWM1/PWM2 relative to COMM) | +40V max. |
| Load sink current | 4A max. |
| Supply voltage (Vcc realive to GND) | 4.5V to 5.25V |
| Operation temparture | -40°C to +85°C))|
| Supply current (for logic part) | 8 mA max. |
| Input low voltage | 0.8V max |
| Input high voltage | 2.1V min |

---
&nbsp;
## Important Note
LED-Warrior18 is still under development and can be supported on [KICKSTARTER](https://www.kickstarter.com/projects/thebug/led-dimming-done-right-pwm-with-i2c/description).

---
&nbsp;
## Known issues
None

---
&nbsp;
## Links and further information
Support this device on [Kickstarter](https://www.kickstarter.com/projects/thebug/led-dimming-done-right-pwm-with-i2c/description).  
Datasheet for the LED-Warrior18 is coming soon.  
[Company site](https://www.codemercs.com) for information on more devices.