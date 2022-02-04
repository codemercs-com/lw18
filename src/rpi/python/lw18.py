#Sample code for using LED-Warrior18 with Python 3 based on GPIO I2C interface

#!/usr/bin/env python3
import sys, getopt
from smbus import SMBus

#DEFINES
LW18_I2C_DEFAULT    = 0x20
LW18_I2C_BROADCAST  = 0x00

#I2C register of LW18
LW18_REG_COMMAND    = 0x00  #R/W : 1Byte ; Command
LW18_REG_PWM16      = 0x01  #R/W : 4Byte ; 2x 16 bit PWM
LW18_REG_PWM8       = 0x02  #W : 2Byte ; 2x 8 bit PWM (log)
LW18_REG_FREQUENCY  = 0x03  #W : 4Byte ; 2x 16 bit frequency
LW18_REG_SIGNATURE  = 0xF0  #R : 6Byte ; Signature
LW18_REG_SETADDR    = 0xFE  #W : 2Byte ; change I2C address

#Bits for COMMAND register
LW18_MODE_SYNC_ENABLE   = 0x01
LW18_MODE_SYNC_RESET    = 0x00
LW18_PWM_TO_FLASH       = 0x80
LW18_SET_BROADCAST      = 0x0A

#Frequency values
LW18_FREQ_DEFAULT   = 48000000 #Hz -> 48 MHz
LW18_FREQ_1         = 730      #Hz
LW18_MAX_PERIODE    = 0xFFFF

#Flags
FLAG_NONE       = 0x0000
FLAG_WRITE      = 0x0001
FLAG_WRITE_C1   = 0x0002
FLAG_WRITE_C2   = 0x0004
FLAG_FREQ       = 0x0008
FLAG_FREQ_C1    = 0x0010
FLAG_FREQ_C2    = 0x0020
FLAG_READ_PWM   = 0x0040
FLAG_READ_FREQ  = 0x0080
FLAG_REV        = 0x0100


#Enable I2C Bus of Raspi
i2cbus = SMBus(1)  # Create a new I2C bus

#Write command bits
def WriteCommand(_i2c, value):
    #Write 1 byte to device
    buffer = [0] #init with defaults
    buffer[0] = (value & 0xFF) #0...255 allowed
    i2cbus.write_i2c_block_data(_i2c, LW18_REG_COMMAND, buffer)
    return 0


#Write 16Bit values to LW18
def WritePwm16(_i2c, v1, v2):
    v1 = (v1 & 0xFFFF) #0...65535 allowed
    v2 = (v2 & 0xFFFF) #0...65535 allowed

    #Write 4 byte to device
    buffer = [0,0,0,0] #init with defaults
    buffer[0] = (v1 & 0x00FF)
    buffer[1] = (v1 & 0xFF00) >> 8
    buffer[2] = (v2 & 0x00FF)
    buffer[3] = (v2 & 0xFF00) >> 8

    i2cbus.write_i2c_block_data(_i2c, LW18_REG_PWM16, buffer)
    return 0


#Read 16Bit PWM from LW18
def ReadPwm16(_i2c):
    #Read 4 byte from device
    buffer = [0,0,0,0]
    buffer = i2cbus.read_i2c_block_data(_i2c, LW18_REG_PWM16, 4)

    #Calc INT values for return
    pwm = [0,0]
    pwm[0] = buffer[0] | (buffer[1] << 8)
    pwm[1] = buffer[2] | (buffer[3] << 8)
    return pwm


#Write 8Bit PWM value to LW18
def WritePwm8(_i2c, v1, v2):
    #Write 2 byte to device
    buffer = [0,0] #init with defaults
    buffer[0] = (v1 & 0xFF) #0...255 allowed
    buffer[1] = (v2 & 0xFF) #0...255 allowed
    i2cbus.write_i2c_block_data(_i2c, LW18_REG_PWM8, buffer)
    return 0


#Write frequency
def WriteFrequency(_i2c, v1, v2):
    v1 = (v1 & 0xFFFF) #0...65535 allowed
    v2 = (v2 & 0xFFFF) #0...65535 allowed

    #Write 4 byte to device
    buffer = [0,0,0,0] #init with defaults
    buffer[0] = (v1 & 0x00FF)       #LSB
    buffer[1] = (v1 & 0xFF00) >> 8  #MSB
    buffer[2] = (v2 & 0x00FF)       #LSB
    buffer[3] = (v2 & 0xFF00) >> 8  #MSB

    i2cbus.write_i2c_block_data(_i2c, LW18_REG_FREQUENCY, buffer)
    return 0


#Return frequency as readable value for output
def GetFrequency(period):
    return (LW18_FREQ_DEFAULT / (period +1))


#Read signatur register
def ReadSignature(_i2c):
    #Read 6 byte from device
    buffer = [0,0,0,0,0,0]
    buffer = i2cbus.read_i2c_block_data(_i2c, LW18_REG_SIGNATURE, 6)

    #Calc INT values for return
    signatur = [0,0,0]
    signatur[0] = buffer[1] | (buffer[0] << 8)
    signatur[1] = buffer[3] | (buffer[2] << 8)
    signatur[2] = buffer[5] | (buffer[4] << 8)
    return signatur


#Change I2C address (permanent)
def SetI2cAddress(_i2c, new):
    old = (v1 & 0x7F) #0...127 allowed
    new = (v2 & 0x7F) #0...127 allowed

    #Write 4 byte to device
    buffer = [0,0] #init with defaults
    buffer[0] = new #new i2c address
    buffer[1] = ~new #invert i2c address for check

    i2cbus.write_i2c_block_data(_i2c, LW18_REG_SETADDR, buffer)
    return


#Print help
def PrintHelp():
    print('Call: lw18.py [OPTIONS]')
    print('')
    print('OPTIONS')
    print('-i, --i2c <HEX>      Set I2C address in hex format (0x??), 0x20 by default')
    print('-a, --c1 <INT>       16Bit PWM value for channel 1 as <INT> (0...65535)')
    print('-b, --c2 <INT>       16Bit PWM value for channel 2 as <INT> (0...65535)')
    print('-f, --f1 <INT>       16Bit FREQUENCY value for channel 2 as <INT> (0...65535)')
    print('-g, --f2 <INT>       16Bit FREQUENCY value for channel 2 as <INT> (0...65535)')
    print('-r, --pwm            Read 16Bit PWM values')
    print('-v, --rev            Get device information')
    return 0


#Main
def main(argv):
    #Some vars
    flag = FLAG_NONE
    pwm1 = 0
    pwm2 = 0
    frq1 = 0
    frq2 = 0
    i2c = LW18_I2C_DEFAULT

    try:
        opts, args = getopt.getopt(argv,"hrvi:a:b:f:g:",["help","pwm","rev","i2c=","c1=","c2=","f1=","f2="])
    except getopt.GetoptError:
        PrintHelp()
        sys.exit(2)

    #No options, print help
    if len(sys.argv) == 1:
        PrintHelp()
        sys.exit(2)

    #Check options
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            PrintHelp()
            sys.exit()
        elif opt in ("-r", "--pwm"):
            flag |= FLAG_READ_PWM

        #Set user defined I2C address as HEX (0x??)
        elif opt in ("-i", "--i2c"):
            i2c = int(arg, 16)

        elif opt in ("-a", "--c1"):
            flag |= FLAG_WRITE | FLAG_WRITE_C1
            pwm1 = int(arg)
        elif opt in ("-b", "--c2"):
            flag |= FLAG_WRITE | FLAG_WRITE_C2
            pwm2 = int(arg)

        elif opt in ("-f", "--f1"):
            flag |= FLAG_FREQ | FLAG_FREQ_C1
            frq1 = int(arg)
        elif opt in ("-g", "--f2"):
            flag |= FLAG_FREQ | FLAG_FREQ_C2
            frq2 = int(arg)

        elif opt in ("-v", "--rev"):
            flag |= FLAG_REV


    #Read device information
    if (flag & FLAG_REV) == FLAG_REV:
        signatur = ReadSignature(i2c)
        print ('Vendor: ', int(signatur[0]))
        print ('Product: ', int(signatur[1]))
        print ('Version: ', hex(signatur[2]))


    #Write 16Bit PWM values to LW18
    if (flag & FLAG_WRITE) == FLAG_WRITE:
        pwm = ReadPwm16(i2c) #Read actual PWM output

        #Overwrite pwm values if set into arg options
        if (flag & FLAG_WRITE_C1) == FLAG_WRITE_C1:
            pwm[0] = pwm1
        if (flag & FLAG_WRITE_C2) == FLAG_WRITE_C2:
            pwm[1] = pwm2

        #Write new PWM outputs
        WritePwm16(i2c, pwm[0], pwm[1])
    
    #Read 16Bit PWM values from LW18
    if (flag & FLAG_READ_PWM) == FLAG_READ_PWM:
        pwm = ReadPwm16(i2c)
        print ('PWM 1: ', pwm[0])
        print ('PWM 2: ', pwm[1])

    #Write 16Bit FREQUENCY values to LW18, Overwrite only! no read back possible
    if (flag & FLAG_FREQ) == FLAG_FREQ:
        #Default values for frequence (730 Hz)
        f1 = 65535
        f2 = 65535

        #Overwrite pwm values if set into arg options
        if (flag & FLAG_FREQ_C1) == FLAG_FREQ_C1:
            f1 = frq1
        if (flag & FLAG_FREQ_C2) == FLAG_FREQ_C2:
            f2 = frq2

        #Write new periode (Overwrite existing)
        WriteFrequency(i2c, f1, f2)

        #Print readable
        print('Frequency PWM 1: ', GetFrequency(f1), "Hz")
        print('Frequency PWM 2: ', GetFrequency(f2), "Hz")


if __name__ == "__main__":
    main(sys.argv[1:])