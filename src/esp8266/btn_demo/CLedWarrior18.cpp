#include <Wire.h>
#include "CLedWarrior18.h"

CLedWarrior18::CLedWarrior18()
{
	m_i2c = LW18_I2C_DEFAULT;
	m_Pwm1 = 0;
	m_Pwm2 = 0;
}

CLedWarrior18::~CLedWarrior18()
{
}

void CLedWarrior18::Init()
{
	Wire.begin();
}

void CLedWarrior18::SetI2cAddress(uint8_t value)
{
	m_i2c = value;
}

uint8_t CLedWarrior18::GetI2cAddress()
{
	return m_i2c;
}

int CLedWarrior18::GetFrequency(uint16_t periode)
{
	return LW18_FREQ_DEFAULT / (periode + 1);
}

uint8_t CLedWarrior18::WriteCommand(uint8_t value)
{
	Wire.beginTransmission(m_i2c);
	Wire.write(LW18_REG_COMMAND);
	Wire.write((uint8_t)value);
	return Wire.endTransmission();
}

uint8_t CLedWarrior18::WritePwm16(uint16_t p1, uint16_t p2)
{
	Wire.beginTransmission(m_i2c);
	Wire.write(LW18_REG_PWM16);

	Wire.write((uint8_t)(p1 & 0x00FF));		   // LSB
	Wire.write((uint8_t)((p1 & 0xFF00) >> 8)); // MSB

	Wire.write((uint8_t)(p2 & 0x00FF));		   // LSB
	Wire.write((uint8_t)((p2 & 0xFF00) >> 8)); // MSB

	return Wire.endTransmission();
}

void CLedWarrior18::ReadPwm16()
{
	// Set start register
	Wire.beginTransmission(m_i2c);
	Wire.write(LW18_REG_PWM16);
	Wire.endTransmission();

	// Read from register n-Bytes
	uint8_t buffer[4]{0};
	Wire.requestFrom(m_i2c, 4);
	Wire.readBytes(buffer, 4);

	m_Pwm1 = buffer[0] | (buffer[1] << 8);
	m_Pwm2 = buffer[2] | (buffer[3] << 8);
}

uint16_t CLedWarrior18::GetPwm1()
{
	return m_Pwm1;
}

uint16_t CLedWarrior18::GetPwm2()
{
	return m_Pwm2;
}

uint8_t CLedWarrior18::WritePwm8(uint8_t p1, uint8_t p2)
{
	Wire.beginTransmission(m_i2c);
	Wire.write(LW18_REG_PWM8);
	Wire.write((uint8_t)p1);
	Wire.write((uint8_t)p2);
	return Wire.endTransmission();
}

uint8_t CLedWarrior18::WritePeriode(uint16_t p1, uint16_t p2)
{
	// Store periode in RAM
	m_Periode1 = p1;
	m_Periode2 = p2;

	Wire.beginTransmission(m_i2c);
	Wire.write(LW18_REG_PERIODE);

	Wire.write((uint8_t)(p1 & 0x00FF));		   // LSB
	Wire.write((uint8_t)((p1 & 0xFF00) >> 8)); // MSB

	Wire.write((uint8_t)(p2 & 0x00FF));		   // LSB
	Wire.write((uint8_t)((p2 & 0xFF00) >> 8)); // MSB

	return Wire.endTransmission();
}

uint16_t CLedWarrior18::GetPeriodePwm1()
{
	return m_Periode1;
}

uint16_t CLedWarrior18::GetPeriodePwm2()
{
	return m_Periode2;
}

uint8_t CLedWarrior18::SetNewI2CAddress(uint8_t value)
{
	Wire.beginTransmission(m_i2c);
	Wire.write(LW18_REG_SETADDR);

	Wire.write((uint8_t)value);
	Wire.write((uint8_t)~value);

	return Wire.endTransmission();
}

void CLedWarrior18::GetSignature()
{
	Wire.beginTransmission(m_i2c);
	Wire.write(LW18_REG_SIGNATURE);
	Wire.endTransmission();

	// Read from register n-Bytes
	uint8_t buffer[6]{0};
	Wire.requestFrom(m_i2c, 6);
	Wire.readBytes(buffer, 6);

	m_Vendor = (buffer[0] << 8) | buffer[1];
	m_Product = (buffer[2] << 8) | buffer[3];
	m_Version = (buffer[4] << 8) | buffer[5];
}

uint16_t CLedWarrior18::GetVendor()
{
	return m_Vendor;
}

uint16_t CLedWarrior18::GetVersion()
{
	return m_Version;
}

uint16_t CLedWarrior18::GetProduct()
{
	return m_Product;
}

uint8_t CLedWarrior18::StorePwm16()
{
	return WriteCommand(LW18_CMD_FLASH);
}

uint8_t CLedWarrior18::EnableSync()
{
	return WriteCommand(LW18_CMD_SYNC);
}

uint8_t CLedWarrior18::DisableSync()
{
	return WriteCommand(LW18_CMD_NONE);
}

uint8_t CLedWarrior18::SetLinLog(uint8_t value)
{
	return WriteCommand(value);
}

uint8_t CLedWarrior18::Broadcast()
{
	Wire.beginTransmission(0x00);
	Wire.write(LW18_BROADCAST);
	return Wire.endTransmission();
}
