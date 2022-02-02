## Code for using LED-Warrior18 with GPIO I2C on Raspberry PI


In this directory you will find some sample applications for the Raspberry PI I²C interface.  
<br>
For using the I²C interface on Raspberry PI you have to enable it by using:


<span style="color: lime">*sudo raspi-config*</span>  

In the configuation menu navigate into '*Advanced Options - I2C*' and enable the I²C interface. After this you have to reboot the Raspberry PI.  

To check for a propper connection to the LED-Warrior18 on the I²C, you can use the 'i2c-tools'. You can install them from the raspberry repository with

*sudo apt update*  
*sudo apt install i2c-tools*  

After installing the tools you can run the command

*i2cdetect -y 1*  

to search for connected I²C devices. The output shows the 7bit I²C address from all connected devices. The output should looks like the following


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;0&nbsp;&nbsp;1&nbsp;&nbsp;2&nbsp;&nbsp;3&nbsp;&nbsp;4&nbsp;&nbsp;5&nbsp;6&nbsp;&nbsp;7&nbsp;&nbsp;8&nbsp;&nbsp;9&nbsp;&nbsp;a&nbsp;&nbsp;b&nbsp;c&nbsp;&nbsp;d&nbsp;&nbsp;e&nbsp;&nbsp;&nbsp;f  <br>
00:          -- -- -- -- -- -- -- -- -- -- -- -- --  <br>
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  <br>
20: 20 -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  <br>
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  <br>
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  <br>
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  <br>
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --  <br>
70: -- -- -- -- -- -- -- --  <br>
<br>

Now your Raspberry PI is ready for the LED-Warrior18 to work with.
