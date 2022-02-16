package write_pwm16;

import java.util.Arrays;
import com.codemercs.iow.IowKit;

public class WritePwm16 
{
	// Defines IO-Warrior
	public static final char IOW_PIPE_IO_PINS = 0;
	public static final char IOW_PIPE_SPECIAL_MODE = 1;
	public static final char IOW_PIPE_I2C = 2;
	public static final char IOW_PIPE_ADC = 3;

	public static final char IOW28_SPECIAL_REPORT_SIZE = 64;

	public static final long IOWKIT_PID_IOW24 = 0x1501;
	public static final long IOWKIT_PID_IOW40 = 0x1500;
	public static final long IOWKIT_PID_IOW56 = 0x1503;
	public static final long IOWKIT_PID_IOW28 = 0x1504;

	// Defines LED-Warrior18
	// LED-Warrior18 I2C address
	public static final char LW18_I2C_DEFAULT = 0x20; // 7Bit
	public static final char LW18_I2C_BROADCAST = 0x00;

	// LED-Warrior18 address register
	public static final char LW18_REG_COMMAND = 0x00; // len: 1Byte R/W;
	public static final char LW18_REG_PWM16 = 0x01; // len: 4Byte R/W;
	public static final char LW18_REG_PWM8 = 0x02; // len: 2Byte W;
	public static final char LW18_REG_PERIODE = 0x03; // len: 4Byte W;
	public static final char LW18_REG_SIGNATURE = 0xF0; // len: 6Byte R
	public static final char LW18_REG_SETADDR = 0xFE; // len: 2Byte W

	// Broadcast command
	public static final byte LW18_BROADCAST = 0x0A; // Send to i2c address 0x00 a 1byte msg with 0x0A

	// Bits for command register
	public static final char LW18_CMD_NONE = 0x00; // 0000 0000
	public static final char LW18_CMD_SYNC = 0x01; // 0000 0001
	public static final char LW18_CMD_LINMAP = 0x02; // 0000 0010
	public static final char LW18_CMD_FLASH = 0x80; // 1000 0000

	// Period defines
	public static final long LW18_FREQ_DEFAULT = 48000000; // 48kHz
	public static final long LW18_FREQ_1 = 730; // Hz
	public static final long LW18_MAX_PERIODE = 0xFFFF;

	// Some defines
	public static final long LW18_MAX_PWM16 = 0xFFFF; // 16 bit value max (65535)
	public static final char LW18_MAX_PWM8 = 0xFF; // 8 bit value max (255)

	public static void EnableI2C(long handle, int status)
	{
		int[] wBuffer = new int[IOW28_SPECIAL_REPORT_SIZE];	
		Arrays.fill(wBuffer, 0);
		
		wBuffer[0] = 0x01;
		wBuffer[1] = status;
		wBuffer[2] = 0;
		IowKit.write(handle, IOW_PIPE_I2C, wBuffer);
	}

	public static int WritePWM16(long handle, int a, int b)
	{
		int[] wBuffer = new int[IOW28_SPECIAL_REPORT_SIZE];	
		Arrays.fill(wBuffer, 0);
				
		wBuffer[0] = 0x02;
		wBuffer[1] = 0xC6;
		wBuffer[2] = (LW18_I2C_DEFAULT << 1);
		wBuffer[3] = LW18_REG_PWM16;
		wBuffer[4] = (a & 0x00FF);	   // LSB
		wBuffer[5] = (a & 0xFF00) >> 8; // MSB
		wBuffer[6] = (b & 0x00FF);	   // LSB
		wBuffer[7] = (b & 0xFF00) >> 8; // MSB
					
		if(IowKit.write(handle, IOW_PIPE_I2C, wBuffer) == IOW28_SPECIAL_REPORT_SIZE)
		{
			int read[] = IowKit.read(handle, IOW_PIPE_I2C, IOW28_SPECIAL_REPORT_SIZE);	
			
			if (read.length == IOW28_SPECIAL_REPORT_SIZE) 
			{
				if((read[2] & 0x80) == 0x80) //Error flag found
					System.out.println("Read/Write error!");	
			}
			else
			{
				System.out.println("Read error!");
				return 0;
			}
		}
		else
		{
			System.out.println("Write error!");
			return 0;
		}
		
		return 1;
	}

	public static void main(String[] args) 
	{
		String temp = "";
		long pid;
		long handle = IowKit.openDevice();

		// IO-Warrior available
		if (handle != 0L) 
		{
			//Get product ID
			pid = IowKit.getProductId(handle);
			
			if(pid == IOWKIT_PID_IOW24)
				System.out.print("Device Type: IO-Warrior24");
			if(pid == IOWKIT_PID_IOW40)
				System.out.print("Device Type: IO-Warrior40");
			if(pid == IOWKIT_PID_IOW56)
				System.out.print("Device Type: IO-Warrior56");	
			if(pid == IOWKIT_PID_IOW28)
				System.out.print("Device Type: IO-Warrior28");
			
			//Get serial number
			temp = IowKit.getSerialNumber(handle);		
			System.out.println(", Serial: " + temp);
						
			// Set timeout to abort read transactions
			IowKit.setTimeout(handle, 1000);

			// Enable I2C mode of IOW28
			EnableI2C(handle, 0x01);

			// Write 16 Bit PWM to LED-Warrior18
			WritePWM16(handle, 0, 0);

			// Enable I2C mode of IOW28
			EnableI2C(handle, 0x00);
		} 
		else
			System.out.println("IO-Warrior not found!");

		// Don't forget to close !
		IowKit.closeDevice(handle);
	}
}
