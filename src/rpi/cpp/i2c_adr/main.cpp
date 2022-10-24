/*
 * LED-Warrior18: Change I2C-Address by i2c interface of raspberry pi
 */

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

#include "lw18.h"

int32_t write_i2c(int32_t fd, uint8_t adr, uint8_t* report, int32_t n)
{
	int32_t ret = 0;

	if (ioctl(fd, I2C_SLAVE, adr) >= 0)
		{
			ret = write(fd, report, n);
			if (ret != n)
				std::cout << "ERROR: " << __FUNCTION__ << std::endl;
		}
		else
			std::cout << "ERROR: " << __FUNCTION__ << std::endl;
	
	return ret;
}

int32_t check_hex(std::string str, uint32_t len)
{
	int32_t ret = 0;
	std::string pool = "0123456789abcdef";
	std::string sub = "";

	if(str.length() != len)
		return -2; //Error: string length not equal 'len'

	//Check for valid hex digits
	for(uint32_t i=0; i<len; i++)
	{
		sub = str.at(i);
		ret += sub.find_first_of(pool, 0);
	}

	return ret;
}


int32_t main(int32_t argc, char **argv)
{
	int32_t device = 0;
	uint64_t funcs = 0;
	uint8_t i2c_old = 0x00;
	uint8_t i2c_new = 0x00;

	//Exit if argument len not correct
	if((argc <3) || (argc >3))
	{
		std::cout << "Usage: ./i2c_adr <old> <new>" << std::endl;
		return 1;
	}

	//Some input checks
	if(check_hex(argv[1], 2) != 0)
	{
		std::cout << "Input error for 'old' I2C address. Please use only hex digits, max length: 2." << std::endl;
		return 1;
	}

	if(check_hex(argv[2], 2) != 0)
	{
		std::cout << "Input error for 'new' I2C address. Please use only hex digits, max length: 2." << std::endl;
		return 1;
	}

	//convert string to long (hex)
	i2c_old = std::stoul(argv[1], nullptr, 16);
	i2c_new = std::stoul(argv[2], nullptr, 16);

	//Open i2c interface from raspberry pi
	device = open("/dev/i2c-1", O_RDWR);

	if (device < 0)
	{
		std::cout << "ERROR: open()" << std::endl;
		return 1;
	}

	if (ioctl(device, I2C_FUNCS, &funcs))
	{
		std::cout << "ERROR: ioctl()" << std::endl;
		close(device);
		return 1;
	}

	//Init buffer and write to lw18
	uint8_t buffer[3] = {LW18_REG_SETADDR, i2c_new, (uint8_t)(~i2c_new)};
	int32_t ret = write_i2c(device, i2c_old, buffer, 3);

	if(ret != 3)
	{
		std::cout << "ERROR: write_i2c() failed" << std::endl;
		close(device);
		return 1;
	}

	close(device);
	return 0;
}