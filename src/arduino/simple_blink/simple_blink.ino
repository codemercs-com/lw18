/*
 * Simple Arduino programm to blink channel 1 and channel 2
 */

#include "Wire.h"

// LED-Warrior18 I2C address
#define LW18_I2C_DEFAULT 0x20 // 7Bit
#define LW18_I2C_BROADCAST 0x00

// LED-Warrior18 address register
#define LW18_REG_COMMAND 0x00    // len: 1Byte R/W;
#define LW18_REG_PWM16 0x01     // len: 4Byte R/W;
#define LW18_REG_PWM8 0x02      // len: 2Byte W;
#define LW18_REG_PERIODE 0x03   // len: 4Byte W;
#define LW18_REG_SIGNATURE 0xF0 // len: 6Byte R
#define LW18_REG_SETADDR 0xFE   // len: 2Byte W

// Broadcast command
#define LW18_BROADCAST 0x0A // Send to i2c address 0x00 a 1byte msg with 0x0A

// Bits for command register
#define LW18_CMD_NONE 0x00   // 0000 0000
#define LW18_CMD_SYNC 0x01   // 0000 0001
#define LW18_CMD_LINMAP 0x02 // 0000 0010
#define LW18_CMD_FLASH 0x80  // 1000 0000

// Period defines
#define LW18_FREQ_DEFAULT 48000000 // 48kHz
#define LW18_FREQ_1 730            // Hz
#define LW18_MAX_PERIODE 0xFFFF

// Some defines
#define LW18_MAX_PWM16 0xFFFF // 16 bit value max (65535)
#define LW18_MAX_PWM8 0xFF    // 8 bit value max (255)


void setup() 
{
    Wire.begin();     
}

//Write 4 data bytes to the LED-Warrior28 (default I2C address)
void WritePwm16(uint16_t ch1, uint16_t ch2)
{
    Wire.beginTransmission(LW18_I2C_DEFAULT);
    Wire.write(LW18_REG_PWM16);

    Wire.write((uint8_t)(ch1 & 0x00FF));        // LSB
    Wire.write((uint8_t)((ch1 & 0xFF00) >> 8)); // MSB

    Wire.write((uint8_t)(ch2 & 0x00FF));        // LSB
    Wire.write((uint8_t)((ch2 & 0xFF00) >> 8)); // MSB

    Wire.endTransmission();
}

//Write 4 data bytes to the LED-Warrior28 (default I2C address)
void WritePwm8(uint8_t ch1, uint8_t ch2)
{
    Wire.beginTransmission(LW18_I2C_DEFAULT);
    Wire.write(LW18_REG_PWM8);

    Wire.write((uint8_t)ch1);
    Wire.write((uint8_t)ch2);

    Wire.endTransmission();
}

void loop() 
{  
    WritePwm16(0, LW18_MAX_PWM16);
    delay(1000);

    WritePwm16(LW18_MAX_PWM16, 0);
    delay(1000);
}
