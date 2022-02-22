// LED-Warrior18 I2C address
#define LW18_I2C_DEFAULT 0x20 // 7Bit
#define LW18_I2C_BROADCAST 0x00

// LED-Warrior18 address register
#define LW18_REG_COMMAND 0x00	// len: 1Byte R/W;
#define LW18_REG_PWM16 0x01		// len: 4Byte R/W;
#define LW18_REG_PWM8 0x02		// len: 2Byte W;
#define LW18_REG_PERIODE 0x03	// len: 4Byte W;
#define LW18_REG_SIGNATURE 0xF0 // len: 6Byte R
#define LW18_REG_SETADDR 0xFE	// len: 2Byte W

// Broadcast command
#define LW18_BROADCAST 0x0A // Send to i2c address 0x00 a 1byte msg with 0x0A

// Bits for command register
#define LW18_CMD_NONE 0x00	 // 0000 0000
#define LW18_CMD_SYNC 0x01	 // 0000 0001
#define LW18_CMD_LINMAP 0x02 // 0000 0010
#define LW18_CMD_FLASH 0x80	 // 1000 0000

// Period defines
#define LW18_FREQ_DEFAULT 48000000 // 48kHz
#define LW18_FREQ_1 730			   // Hz
#define LW18_MAX_PERIODE 0xFFFF

// Some defines
#define LW18_MAX_PWM16 0xFFFF // 16 bit value max (65535)
#define LW18_MAX_PWM8 0xFF	  // 8 bit value max (255)

class CLedWarrior18
{
private:
	uint8_t m_i2c;
	uint16_t m_Pwm1;
	uint16_t m_Pwm2;

	uint16_t m_Vendor;
	uint16_t m_Product;
	uint16_t m_Version;

	uint16_t m_Periode1;
	uint16_t m_Periode2;

public:
	CLedWarrior18();
	virtual ~CLedWarrior18();

	void Init();					   // Init wire.h
	void SetI2cAddress(uint8_t value); // Set slave I2C address
	uint8_t GetI2cAddress();		   // Get slave I2C address

	int GetFrequency(uint16_t periode); // Calculate frequency from periode

	uint8_t WriteCommand(uint8_t value);			// Write COMMAND register
	uint8_t WritePwm16(uint16_t p1, uint16_t p2);	// Write 16 bit PWM
	void ReadPwm16();								// Read 16 bit PWM
	uint16_t GetPwm1();								// Get 16 bit PWM1
	uint16_t GetPwm2();								// Get 16 bit PWM2
	uint8_t WritePwm8(uint8_t p1, uint8_t p2);		// Write 8 bit PWM
	uint8_t WritePeriode(uint16_t p1, uint16_t p2); // Write 16 bit periode values
	uint16_t GetPeriodePwm1();						// Get written periode PWM1
	uint16_t GetPeriodePwm2();						// Get written periode PWM2
	uint8_t SetNewI2CAddress(uint8_t value);		// Set new I2C slave address into flash ROM
	void GetSignature();							// Get device revision
	uint16_t GetVendor();
	uint16_t GetVersion();
	uint16_t GetProduct();

	// Command functions
	uint8_t StorePwm16();			  // Write actual PWM into flash
	uint8_t EnableSync();			  // Enable sync mode
	uint8_t DisableSync();			  // Disable sync mode
	uint8_t SetLinLog(uint8_t value); // Set LIN or LOG for 8 bit PWM
	uint8_t Broadcast();			  // Send broadcast command
};
