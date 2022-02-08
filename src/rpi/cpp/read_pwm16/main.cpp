#include <iostream>
#include <unistd.h>
#include <string.h>
#include <iomanip>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

#include "lw18.h"

//Write report
int write_i2c(int fd, unsigned char adr, unsigned char *report, int n)
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

//Read report
int read_i2c(int fd, unsigned char adr, unsigned char *report, int n)
{
	int ret = 0;
	if (ioctl(fd, I2C_SLAVE, adr) >= 0)
	{
		ret = read(fd, report, n);
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

	unsigned char i2c = LW18_I2C_DEFAULT;
	unsigned char buffer[5] = {0};

	//Set register to read from
	buffer[0] = LW18_REG_PWM16;
	write_i2c(device, i2c, buffer, 1);

	//Read 16 bit PWM values
	memset(&buffer, 0x00, 5);
	read_i2c(device, i2c, buffer, 4);

	//Output
	std::cout << "PWM1: " << (buffer[0] | (buffer[1] << 8)) << std::endl;
	std::cout << "PWM2: " << (buffer[2] | (buffer[3] << 8)) << std::endl;  

	close(device);

	return 0;
}