/*
 * LED-Warrior18: Write PWM with broadcast
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

//
int write_i2c(int fd, unsigned char adr, unsigned char* report, int n)
{
	int ret = 0;

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

int main(int argc, char **argv)
{
	int device = 0;
	unsigned long funcs = 0;

	device = open("/dev/i2c-1", O_RDWR);

	if (device < 0)
	{
		std::cout << "ERROR: open()" << std::endl;
		return -1;
	}

	if (ioctl(device, I2C_FUNCS, &funcs))
	{
		std::cout << "ERROR: ioctl()" << std::endl;
		close(device);
		return -1;
	}

	int pwm1 = 1000; // Input value for pwm1
	int pwm2 = 5000; // Input value for pwm2

	//Use two LED-Warrior18 moduls for broadcast test
	unsigned char numdev = 2;
	unsigned char i2c[2] = {0x20, 0x22};

	//Report buffer for Write/Read
	unsigned char buffer[5] = {0};

	//Enable Sync mode for all devices
	buffer[0] = LW18_REG_COMMAND;
	buffer[1] = LW18_CMD_SYNC;

	//For every single device
	for(int i=0; i<numdev; i++)
		write_i2c(device, i2c[i], buffer, 2);

	//Send PWM value to all devices
	memset(&buffer, 0x00, 5);
	buffer[0] = LW18_REG_PWM16;
	buffer[1] = (pwm1 & 0x00FF);
	buffer[2] = (pwm1 & 0xFF00) >> 8;
	buffer[3] = (pwm2 & 0x00FF);
	buffer[4] = (pwm2 & 0xFF00) >> 8;

	//Send PWM to every single device
	for(int i=0; i<numdev; i++)
	{
		write_i2c(device, i2c[i], buffer, 5);
		usleep(1000*2000); //TEST: delay for demo
	}

	//Send broadcast command to load PWM value from background
	memset(&buffer, 0x00, 5);
	buffer[0] = LW18_BROADCAST;
	write_i2c(device, LW18_I2C_BROADCAST, buffer, 1);

	//Disable sync mode for all device to have access for direct
	//write PWM values.
	memset(&buffer, 0x00, 5);
	buffer[0] = LW18_REG_COMMAND;
	buffer[1] = LW18_CMD_NONE;

	//Send command to every single device
	for(int i=0; i<numdev; i++)
		write_i2c(device, i2c[i], buffer, 2);

	close(device);
	return 0;
}