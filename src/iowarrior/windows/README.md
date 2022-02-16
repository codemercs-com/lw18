## Using IO-Warrior on Windows
For using the IO-Warrior on Windows you can use out *iowkit.dll*. This DLL will include all basic functions for communication and can be found in the SDK on our [website](https://codemercs.com/downloads/iowarrior/IO-Warrior_SDK_win.zip)

## Installation
Windows use the standard system drivers for HID devices. You don't have to install 3rd party drivers.  
There are two different ways to 'install' the iowkit.dll.  
1. The *iowkit.dll* is in the same place like the application
2. The *iowkit.dll* is located in the 'System32' or 'SysWOW64' directory of Windows, depends on your binary (x86 or x64).
   
**We** recommend the first method.

## Usage
There is a documentation *IowKit_V15_API.pdf* in the SDK for using the methods of the *iowkit*.

## Important Note
The *iowkit.dll* can only be used by one application at the same time to prevent access errors. 
