## Using IO-Warrior on Linux

**INFO: This instruction is created for an Raspberry PI, but can be used on other Linux distributions as well.**

The IO-Warrior family is part of the Linux kernel version 4.14, 4.19, 5.4, and higher. You can check this with the command 

```
sudo modinfo iowarrior
```

The output should be look like this:
```
filename:       /lib/modules/5.10.63-v7+/kernel/drivers/usb/misc/iowarrior.ko
license:        GPL
description:    USB IO-Warrior driver
author:         Christian Lucht <lucht@codemercs.com>
srcversion:     92DD515200ED650FB612735
alias:          usb:v07C0p1506d*dc*dsc*dp*ic*isc*ip*in*
alias:          usb:v07C0p1505d*dc*dsc*dp*ic*isc*ip*in*
alias:          usb:v07C0p1504d*dc*dsc*dp*ic*isc*ip*in*
alias:          usb:v07C0p158Bd*dc*dsc*dp*ic*isc*ip*in*
alias:          usb:v07C0p158Ad*dc*dsc*dp*ic*isc*ip*in*
alias:          usb:v07C0p1503d*dc*dsc*dp*ic*isc*ip*in*
alias:          usb:v07C0p1512d*dc*dsc*dp*ic*isc*ip*in*
alias:          usb:v07C0p1511d*dc*dsc*dp*ic*isc*ip*in*
alias:          usb:v07C0p1501d*dc*dsc*dp*ic*isc*ip*in*
alias:          usb:v07C0p1500d*dc*dsc*dp*ic*isc*ip*in*
depends:
intree:         Y
name:           iowarrior
vermagic:       5.10.63-v7+ SMP mod_unload modversions ARMv7 p2v8
```
All supported IO-Warrior devices listed as **'usb:v< VendorID >p< ProductID >d....'**  
&nbsp;

## Install iowkit API

To install the *'libiowkit'* on Linux you have to download the latest API from our [website]([http://](https://codemercs.com/downloads/iowarrior/IO-Warrior_SDK_linux.zip) and compile the iowkit API on your system.  
Unzip the file and navigate into the *'libiowkit-1.7.0'* directory and run to following commands:  
```
./configure  
make  
sudo make install  
```


## Add rules for access without root permissions (sudo)
After the installation a file must be created to get access to the IO-Warior without any root permissions.  
&nbsp;

Add a new rules into '/etc/udev/rules.d/':  
```
sudo nano /etc/udev/rules.d/10-iowarrior.rules
```
and insert the following line:

```
KERNEL=="iowarrior*", NAME="usb/iowarrior%n", OWNER="pi", GROUP="pi", MODE="0666"
```

Reboot the system.  
Now your chosen user (here 'pi') have write/read access to the IO-Warrior family.


&nbsp;
## Important Note
Witout add a rules file you have only access to the IO-Warrior with 'sudo' or root permissions.  

