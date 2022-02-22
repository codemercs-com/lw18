/*
 * Read signature and PWM16 register from LED-Warrior18 with a class
 */

#include "CLedWarrior18.h"

CLedWarrior18 lw18;
bool m_RunOnce;

void setup() 
{
    //Init Serial
    Serial.begin(9600);

    //Generate random seed by unconnected GPIO pin
    randomSeed(analogRead(0));
        
    // init LED-Warrior18 Class
    lw18.Init();
    lw18.SetI2cAddress(LW18_I2C_DEFAULT);
    
    //Run this programm only on start/restart (once)
    m_RunOnce = false;
}

void loop() 
{
    if(m_RunOnce == false)
    {
        //Get Signrature 
        lw18.GetSignature();
        Serial.println("*** Signature ***");  
        Serial.print("Vendor: ");
        Serial.println(lw18.GetVendor());
        Serial.print("Product: ");
        Serial.println(lw18.GetProduct());
        Serial.print("Version: ");
        Serial.println(lw18.GetVersion());
        Serial.println("");

        //For demo: Write random PWM16 values
        int r1 = random(0, 5000);
        int r2 = random(0, 5000);
        
        lw18.WritePwm16(r1, r2);
        Serial.print("Write random PWM values for demo: ");
        Serial.print(r1);
        Serial.print(", ");
        Serial.println(r2);
        Serial.println("");

        //Get actual PWM16 values
        lw18.ReadPwm16();
        Serial.println("*** Actual PWM value ***");      
        Serial.print("Channel 1: ");
        Serial.println(lw18.GetPwm1());      
        Serial.print("Channel 2: ");
        Serial.println(lw18.GetPwm2());
        Serial.println("* * * * * * * * * *");
       
        m_RunOnce = true;
    }
}
