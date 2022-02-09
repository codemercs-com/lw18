/*
   Sample code for LED-Warrior18 controlled by an ESP8266
*/

#define FLAG_NONE 0x0000
#define FLAG_BTN_1 0x0001
#define FLAG_BTN_2 0x0002

#include "CLedWarrior18.h"

// Button struct
struct BUTTON_STRUCT
{
	uint8_t pin;
	uint8_t now_state;
	uint8_t last_state;
};

BUTTON_STRUCT btn[2]; // the number of the pushbutton pin
CLedWarrior18 lw18;

// Dimming
int dimm_pos = 0;													   // Position for dimming
const uint8_t dimm_size = 11;										   // Max size of array dimm[]
uint8_t dimm[11] = {0, 26, 52, 78, 104, 130, 156, 182, 208, 234, 254}; // Table for 10% dimming steps + off

// Flag
uint8_t flag = FLAG_NONE;

// Debounce based on Arduino dokumentation
long lastDebounceTime = 0;
long debounceDelay = 50;

void setup()
{
	// Setup buttons
	btn[0].pin = 12;
	btn[0].now_state = LOW;
	btn[0].last_state = LOW;
	pinMode(btn[0].pin, INPUT);

	btn[1].pin = 14;
	btn[1].now_state = LOW;
	btn[1].last_state = LOW;
	pinMode(btn[1].pin, INPUT);

	// init LED-Warrior18
	lw18.Init();
	lw18.SetI2cAddress(LW18_I2C_DEFAULT);
}

void loop()
{
	// Get button states
	btn[0].now_state = digitalRead(btn[0].pin);
	btn[1].now_state = digitalRead(btn[1].pin);

	// Debounce
	if ((millis() - lastDebounceTime) > debounceDelay)
	{
		// Only if button 2 not pressed
		if (btn[1].now_state == LOW)
		{
			// Button 1
			if ((btn[0].now_state == HIGH) && (btn[0].last_state == LOW))
			{
				dimm_pos++;

				if (dimm_pos > (dimm_size - 1))
					dimm_pos = 10;

				flag |= FLAG_BTN_1;
				btn[0].last_state = HIGH;
				lastDebounceTime = millis();
			}
			else if ((btn[0].now_state == LOW) && (btn[0].last_state == HIGH))
			{
				btn[0].last_state = LOW;
				lastDebounceTime = millis();
			}
		}

		// Only if button 1 not pressed
		if (btn[0].now_state == LOW)
		{
			// Button 2
			if ((btn[1].now_state == HIGH) && (btn[1].last_state == LOW))
			{
				dimm_pos--;

				if (dimm_pos < 0)
					dimm_pos = 0;

				flag |= FLAG_BTN_2;
				btn[1].last_state = HIGH;
				lastDebounceTime = millis();
			}
			else if ((btn[1].now_state == LOW) && (btn[1].last_state == HIGH))
			{
				btn[1].last_state = LOW;
				lastDebounceTime = millis();
			}
		}
	}

	// Send data if any button bressed (flag check)
	if (((flag & (FLAG_BTN_1 | FLAG_BTN_2)) == FLAG_BTN_1) ||
		((flag & (FLAG_BTN_1 | FLAG_BTN_2)) == FLAG_BTN_2))
	{
		lw18.WritePwm8(dimm[dimm_pos], 0);
		flag = FLAG_NONE;
	}
}
