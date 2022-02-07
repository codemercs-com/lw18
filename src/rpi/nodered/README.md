## LED-Warrior18 with NodeRed

&nbsp;
## Requirements
For using this nodes you have to install the 'i2c-bus' npm package.  
For more information about this package please visit: https://github.com/fivdi/i2c-bus#readme

&nbsp;
## Install NODE
Copy the folder 'node-cm-lw18' into ~./node-red/node_modules and start/restart your Node-Red server.

&nbsp;
## Usage

&nbsp;
### Node: set cmd
Send 1 byte command value to the LED-Warrior18.

&nbsp;
### Node: get cmd
Read 1 byte value of the command register.

&nbsp;
### Node: set pwm16
Write 4 byte data to set the PWM output for both channel.

&nbsp;
### Node: get pwm16
Read the 16bit PWM values from LED-Warrior18 as object: {pwm1: [INT], pwm2: [INT]} 

&nbsp;
### Node: set pwm8
Write 2 byte data to set the PWM output for to both channel.  
Based on linear or logaritmic setup.

&nbsp;
### Node: set freq
Write 4 byte data to set the periode of both PWM channel.

&nbsp;
## Important Note
This node is created to use with a Raspberry PI only.

&nbsp;
## Known issues
None