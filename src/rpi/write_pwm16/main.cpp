/*
 * LED-Warrior18: Write 16Bit PWM values over GPIO I2C interface on Raspberry PI
 */

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

//LED-Warrior18 I2C address
#define LW18_I2C_DEFAULT 	0x20 	//7Bit
#define LW18_I2C_BROADCAST 	0x00

//LED-Warrior18 address register
#define LW18_REG_COMMAND 	0x00 	//len: 1Byte R/W;
#define LW18_REG_PWM16 		0x01 	//len: 4Byte R/W;
#define LW18_REG_PWM8 		0x02 	//len: 2Byte W;
#define LW18_REG_SIGNATURE 	0xF0 	//len: 6Byte R
#define LW18_REG_SETADDR 	0xFE 	//len: 2Byte W

//Some flags
#define FLAG_NONE 			0x0000
#define FLAG_WRITE_PWM 		0x0001 	//Flag for write
#define FLAG_WRITE_C1 		0x0002 	//Flag data for PWM1
#define FLAG_WRITE_C2 		0x0004 	//Flag data for PWM2

//Some defines
#define LW18_MAX_PWM        0xFFFF	//16 bit value max (65535)


//Check input argument, only digits allowed
int is_digits(const std::string &str)
{
	return str.find_first_not_of("0123456789") == std::string::npos;
}

int main(int argc, char **argv)
{
	unsigned short flags = FLAG_NONE;	//Flags for arguments
	int pwm1 = 0;	//Input value for pwm1
	int pwm2 = 0;	//Input value for pwm2

	//Chatch the arguments <filename> -a <value> -b <value>
	if (argc > 1)
	{
		for (int i = 0; i < argc; i++)
		{
			//Value for pwm 1
			if (strcmp(argv[i], "-a") == 0)
			{
				//Check for digits only
				if (is_digits(argv[i + 1]) != 0)
				{
					pwm1 = atoi(argv[i + 1]);	//String to int
					flags |= FLAG_WRITE_C1;		//Set flag
				}
				else
					std::cout << "Wrong value for PWM1. Only digits are allowed" << std::endl;
			}

			//Value for pwm 2
			if (strcmp(argv[i], "-b") == 0)
			{
				//Check for digits only
				if (is_digits(argv[i + 1]) != 0)
				{
					pwm2 = atoi(argv[i + 1]);	//String to int
					flags |= FLAG_WRITE_C2;		//set flag
				}
				else
					std::cout << "Wrong value for PWM2. Only digits are allowed" << std::endl;
			}
		}
	}
	else
	{
		std::cout << "To few arguments. <filename> -a <DEC> -b <DEC>" << std::endl;
		return -1;
	}

	//I2C setup
	int device = 0;
	unsigned long funcs = 0;
	int address = LW18_I2C_DEFAULT;

	if( (flags & (FLAG_WRITE_C1 | FLAG_WRITE_C2)) != (FLAG_WRITE_C1 | FLAG_WRITE_C2))
	{
		std::cout << "Something wrong with the arguments. Please input both PWM channel. Only digits allowed for PWM values." << std::endl;
		return -1;
	}

	device = open("/dev/i2c-1", O_RDWR);

	if(device < 0)
	{
		std::cout << "ERROR: open()" << std::endl;
		return -1;
	}

	if(ioctl(device, I2C_FUNCS, &funcs))
	{
		std::cout << "ERROR: ioctl()" << std::endl;
		close(device);
		return -1;
	}

	//Cut off if value > 65535
	if(pwm1 > LW18_MAX_PWM)
		pwm1 = LW18_MAX_PWM;
	
	if(pwm2 > LW18_MAX_PWM)
		pwm2 = LW18_MAX_PWM;

	int ret = 0;
	unsigned char buffer[5] = {0};

	//Create report for send 16bit PWM, LSB first
	buffer[0] = LW18_REG_PWM16;
	buffer[1] = (pwm1 & 0x00FF);
	buffer[2] = (pwm1 & 0xFF00) >> 8;
	buffer[3] = (pwm2 & 0x00FF);
	buffer[4] = (pwm2 & 0xFF00) >> 8;

	if (ioctl(device, I2C_SLAVE, address) >= 0)
	{
		ret = write(device, buffer, 5);
		if (ret != 5)
		{
			std::cout << "ERROR: " << __FUNCTION__ << std::endl;
			return -1;
		}
	}
	else
	{
		std::cout << "ERROR: " << __FUNCTION__ << std::endl;
		return -1;
	}

	close(device);
	return 0;
}