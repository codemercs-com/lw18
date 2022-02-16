/*
* Sample code for LED-Warrior18 on Windows.
*/
#include <Windows.h>
#include <iostream>
#include <string.h>
#include <iomanip>

#include "iowkit.h"
#include "lw18.h"


int main(int argc, char** argv)
{
	IOWKIT_HANDLE handle; 				// Main HANDLE, 1st handle found
	IOWKIT28_SPECIAL_REPORT report;		//report struct for iowkit

	unsigned int numDev = 0; // Device count
	wchar_t sn[9] = {};	 // Serialnumber
	unsigned long pid = 0;	 // ProductID
	unsigned char i2c = (LW18_I2C_DEFAULT << 1); //IO-Warrior use 8Bit I2C address

	int ch1 = 0; // Input value for write
	int ch2 = 0; // Input value for write

	//Check for argument count
	if (argc != 3)
	{
		std::cout << "Arguments not correct. You must insert two (!) arguments as INT to set PWM output." << std::endl;
		std::cout << "Examle: ./basic_pwm16 5123 33123" << std::endl;
		return 1;
	}

	handle = IowKitOpenDevice(); // Open API and get first IO-Warrior
	if (handle != NULL)
	{
		numDev = IowKitGetNumDevs();	//Get number of connected IO-Warriors
		if (numDev > 1)
		{
			std::cout << "ERROR: Multiple IO-Warrior found. Please connect only one IO-Warrior28. Exit" << std::endl;
			IowKitCloseDevice(handle);
			return 1;
		}


		pid = IowKitGetProductId(handle); // Get ProductID of IO-Warrior
		IowKitSetTimeout(handle, 1000);	  // Set timeout for cancel write/read actions. By default: INFINITE

		// Check for IO-Warrior28 device. This demo works only with IO-Warrior28
		if (pid != IOWKIT_PID_IOW28)
		{
			std::cout << "ERROR: Unsupported IO-Warrior found. Exit" << std::endl;
			IowKitCloseDevice(handle);
			return 1;
		}

		//Get serial number for output
		IowKitGetSerialNumber(handle, sn);
		std::cout << "Device found | PID: " << std::hex << (pid & 0xFFFF) << std::dec << " | Serial: " << sn << std::endl;

		// Convert argument into INT
		ch1 = atoi(argv[1]);
		ch2 = atoi(argv[2]);

		// Enable I2C mode of IO-Warrior28
		memset(&report, 0, IOWKIT28_SPECIAL_REPORT_SIZE);
		report.ReportID = 0x01;	// Special mode: I2C
		report.Bytes[0] = 0x01;	//Enable I2C
		report.Bytes[1] = 0x00;	//Flags
		IowKitWrite(handle, IOW_PIPE_I2C_MODE, (char*)&report, IOWKIT28_SPECIAL_REPORT_SIZE);
		// No IowKitRead() required, nothing come back on enable special mode

		// Write PWM16
		memset(&report, 0, IOWKIT28_SPECIAL_REPORT_SIZE);
		report.ReportID = 0x02;					//Write I2C
		report.Bytes[0] = 0xC6;					//Start + Stop + 6 bytes of Data
		report.Bytes[1] = i2c;					//I2C address
		report.Bytes[2] = LW18_REG_PWM16;		//Register
		report.Bytes[3] = (ch1 & 0x00FF);	   // LSB
		report.Bytes[4] = (ch1 & 0xFF00) >> 8; // MSB
		report.Bytes[5] = (ch2 & 0x00FF);	   // LSB
		report.Bytes[6] = (ch2 & 0xFF00) >> 8; // MSB

		// Write report to IO-Warrior
		if (IowKitWrite(handle, IOW_PIPE_I2C_MODE, (char*)&report, IOWKIT28_SPECIAL_REPORT_SIZE) == IOWKIT28_SPECIAL_REPORT_SIZE)
		{
			// Read I2C answere with error flags
			if (IowKitRead(handle, IOW_PIPE_I2C_MODE, (char*)&report, IOWKIT28_SPECIAL_REPORT_SIZE) != IOWKIT28_SPECIAL_REPORT_SIZE)
			{
				std::cout << "Error during read" << std::endl;
				//Error handling here...
			}
		}
		else
			std::cout << "Error during write" << std::endl;

		// Disable I2C mode of IO-Warrior28
		memset(&report, 0, IOWKIT28_SPECIAL_REPORT_SIZE);
		report.ReportID = 0x01; // Special mode: I2C
		report.Bytes[0] = 0x00;	//Disable I2C
		report.Bytes[1] = 0x00;	//FLags
		IowKitWrite(handle, IOW_PIPE_I2C_MODE, (char*)&report, IOWKIT28_SPECIAL_REPORT_SIZE);
		// No IowKitRead() required, nothing come back on enable special mode
	}
	else
		std::cout << "ERROR: No IO-Warrior28 (USB to I2C interface) found. Exit" << std::endl;

	IowKitCloseDevice(handle); // You have to unload the iowkit API!   


	return 0;
}
