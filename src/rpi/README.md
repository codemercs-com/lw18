## Code for using LED-Warrior18 with GPIO I2C on Raspberry PI


In this directory you will find some sample applications for the Raspberry PI I²C interface.  
<br>
For using the I²C interface on Raspberry PI you have to enable it by using:

```
sudo raspi-config
```

In the configuation menu navigate into '*Advanced Options - I2C*' and enable the I²C interface. After this you have to reboot the Raspberry PI.  

To check for a propper connection to the LED-Warrior18 on the I²C, you can use the 'i2c-tools'. You can install them from the raspberry repository with

```
sudo apt update
sudo apt install i2c-tools
```

After installing the tools you can run the command

```
i2cdetect -y 1
```

to search for connected I²C devices. The output shows the 7bit I²C address from all connected devices. The output should looks like the following


```
     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: 20 -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --
```
Now your Raspberry PI is ready for the LED-Warrior18 to work with.
