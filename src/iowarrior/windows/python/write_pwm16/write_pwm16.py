import time, os
import ctypes

#Header for LED-Warrior18
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

#Try to open iowkit
try:
	iowkit = ctypes.WinDLL(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'iowkit'))
except:
	try:
		iowkit = ctypes.WinDLL(os.path.join(os.curdir, 'iowkit'))
	except:
		iowkit = ctypes.WinDLL('iowkit')

#Header for IO-Warrior
# IOWKIT_HANDLE IOWKIT_API IowKitOpenDevice(void);
iowkit.IowKitOpenDevice.argtypes        = None
iowkit.IowKitOpenDevice.restype         = ctypes.c_voidp

# void IOWKIT_API IowKitCloseDevice(IOWKIT_HANDLE devHandle);
iowkit.IowKitCloseDevice.argtypes       = [ctypes.c_voidp]
iowkit.IowKitCloseDevice.restype        = None

# IOWKIT_HANDLE IOWKIT_API IowKitGetDeviceHandle(ULONG numDevice);
iowkit.IowKitGetDeviceHandle.argtypes   = [ctypes.c_ulong]  # numDev = 1 ... 16
iowkit.IowKitGetDeviceHandle.restype    = ctypes.c_voidp

# ULONG IOWKIT_API IowKitGetNumDevs(void);
iowkit.IowKitGetNumDevs.argtypes        = None
iowkit.IowKitGetNumDevs.restype         = ctypes.c_ulong    # Py default

# PCHAR IOWKIT_API IowKitVersion(void);
# res like: IO-Warrior Kit V1.5
iowkit.IowKitVersion.argtypes           = None
iowkit.IowKitVersion.restype            = ctypes.c_char_p

# ULONG IOWKIT_API IowKitProductId(IOWKIT_HANDLE iowHandle);
# res like: 0x1501
iowkit.IowKitGetProductId.argtypes      = [ctypes.c_voidp]
iowkit.IowKitGetProductId.restype       = ctypes.c_ulong    # Py default

# ULONG IOWKIT_API IowKitGetRevision(IOWKIT_HANDLE iowHandle);
# res like: 0x1030
iowkit.IowKitGetRevision.argtypes       = [ctypes.c_voidp]
iowkit.IowKitGetRevision.restype        = ctypes.c_ulong    # Py default

# BOOL IOWKIT_API IowKitSetTimeout(IOWKIT_HANDLE devHandle, ULONG timeout);
iowkit.IowKitSetTimeout.argtypes        = [ctypes.c_voidp, ctypes.c_ulong]
iowkit.IowKitSetTimeout.restype         = ctypes.c_bool

# BOOL IOWKIT_API IowKitSetWriteTimeout(IOWKIT_HANDLE devHandle, ULONG timeout);
iowkit.IowKitSetWriteTimeout.argtypes   = [ctypes.c_voidp, ctypes.c_ulong]
iowkit.IowKitSetWriteTimeout.restype    = ctypes.c_bool

# ULONG IOWKIT_API IowKitWrite(IOWKIT_HANDLE devHandle, ULONG numPipe, PCHAR buffer, ULONG length);
iowkit.IowKitWrite.argtypes             = [ctypes.c_voidp, ctypes.c_ulong, ctypes.c_voidp, ctypes.c_ulong]
iowkit.IowKitWrite.restype              = ctypes.c_ulong    # Py default

# ULONG IOWKIT_API IowKitRead(IOWKIT_HANDLE devHandle, ULONG numPipe, PCHAR buffer, ULONG length);
iowkit.IowKitRead.argtypes              = [ctypes.c_voidp, ctypes.c_ulong, ctypes.c_voidp, ctypes.c_ulong]
iowkit.IowKitRead.restype               = ctypes.c_ulong    # Py default

# BOOL IOWKIT_API IowKitReadImmediate(IOWKIT_HANDLE devHandle, PDWORD value);
# Return current value directly read from the IO-Warrior I/O pins.
# not relevant for I2C
iowkit.IowKitReadImmediate.argtypes     = [ctypes.c_voidp, ctypes.POINTER(ctypes.c_ulong)]
iowkit.IowKitReadImmediate.restype      = ctypes.c_bool

# BOOL IOWKIT_API IowKitGetSerialNumber(IOWKIT_HANDLE iowHandle, PWCHAR serialNumber);
# from iowkit.h:
# typedef unsigned short *       PWCHAR;
# from library docu:
# "The serial number is represented as an Unicode string. The buffer pointed to
# by serialNumber must be big enough to hold 9 Unicode characters (18 bytes),
# because the string is terminated in the usual C way with a 0 character."
#
# Originally suggested code fails:
# iowkit.IowKitGetSerialNumber.argtypes  = [ctypes.c_voidp, ctypes.c_wchar_p]
# It results in Python crashing on conversion of c_wchar_p, probably because
# the implicit conversion of Python3 expects 4 bytes per unicode char, while
# the iowlibrary uses only 2.
# Workaround is a string buffer of length 18 and the conversion is per
# buffer.raw.decode("utf-8")
iowkit.IowKitGetSerialNumber.argtypes   = [ctypes.c_voidp, ctypes.c_voidp] # ok
iowkit.IowKitGetSerialNumber.restype    = ctypes.c_bool

#Defines for IOW28
IOWKIT28_SPECIAL_REPORT = ctypes.c_ubyte * 64
IOWKIT28_SPECIAL_REPORT_SIZE = ctypes.c_ulong(64)
SERIAL_NUMBER_BUFFER = ctypes.c_ubyte * 9

#Interface number
IOW_OPEN_SIMPLE = ctypes.c_ulong(0)
IOW_OPEN_COMPLEX = ctypes.c_ulong(1)
IOW_OPEN_I2C = ctypes.c_ulong(2)
IOW_OPEN_ADC = ctypes.c_ulong(3)

# IO-Warrior vendor & product IDs
IOWKIT_VENDOR_ID                        = 0x07c0
# IO-Warrior 40
IOWKIT_PRODUCT_ID_IOW40                 = 0x1500
# IO-Warrior 24
IOWKIT_PRODUCT_ID_IOW24                 = 0x1501
IOWKIT_PRODUCT_ID_IOW24_SENSI           = 0x158A
# IO-Warrior 56
IOWKIT_PRODUCT_ID_IOW56                 = 0x1503
# IO-Warrior28
IOWKIT_PRODUCT_ID_IOW28                 = 0x1504


# PROGRAM

#Write 16Bit values to LW18
def WritePwm16(handle, v1, v2):
	v1 = (v1 & 0xFFFF) #0...65535 allowed
	v2 = (v2 & 0xFFFF) #0...65535 allowed

	report = IOWKIT28_SPECIAL_REPORT(
	0x02, # I2C-Write
	0xC6, # Start, Stopp, 6 Bytes
	(LW18_I2C_DEFAULT << 1), # Address
	LW18_REG_PWM16, # I2C register
	(v1 & 0x00FF), # data
	(v1 & 0xFF00) >> 8, # data
	(v2 & 0x00FF), 
	(v2 & 0xFF00) >> 8
	)

	iowkit.IowKitWrite(handle, IOW_OPEN_I2C, ctypes.byref(report), IOWKIT28_SPECIAL_REPORT_SIZE)
	iowkit.IowKitRead(handle, IOW_OPEN_I2C, ctypes.byref(report), IOWKIT28_SPECIAL_REPORT_SIZE)
	return

def EnableI2C(handle, state):
	report = IOWKIT28_SPECIAL_REPORT(
	0x01, # I2C-Mode
	state, # Enable
	0x00, # flags
	0x00, 0x00, 0x00, 0x00, 0x00
	)

	iowkit.IowKitWrite(handle, IOW_OPEN_I2C, ctypes.byref(report), IOWKIT28_SPECIAL_REPORT_SIZE)
	return

def getDeviceName (pid):
	if pid == IOWKIT_PRODUCT_ID_IOW24:
		itype = "24"
	elif pid == IOWKIT_PRODUCT_ID_IOW40:
		itype = "40"
	elif pid == IOWKIT_PRODUCT_ID_IOW56:
		itype = "56"
	elif pid == IOWKIT_PRODUCT_ID_IOW28:
		itype = "28"
	else:
		itype = " (unknown)"

	return "IO-Warrior{}".format(itype)


def dprint (text, ret, *args):
	sarg = ""
	for arg in args: sarg += "{:16s} ".format(str(arg))
	print("{:30s}: {:5s} {:15s}".format(text, str(ret), sarg))


ioHandle = iowkit.IowKitOpenDevice()
if ioHandle == 0:
	raise IOError("No device detected, ioHandle: " + str(ioHandle))

# Get info of IO-Warrior
# Get number of IOWs
numdevs = iowkit.IowKitGetNumDevs()
dprint("IowKitGetNumDevs", numdevs)

# Get Kit Version
ikv = iowkit.IowKitVersion()
dprint("IowKitVersion", "----", "", ikv.decode("UTF-8"))

# Get Revision
rev = iowkit.IowKitGetRevision(ioHandle)
dprint("IowKitGetRevision", rev, hex(rev), "Firmware Version")

# Get ProductID and Name
pid = iowkit.IowKitGetProductId(ioHandle)
dprint("IowKitGetProductId", pid, hex(pid), getDeviceName(pid))

# Get Serial number
bsno  = ctypes.create_string_buffer(18)
isn   = iowkit.IowKitGetSerialNumber(ioHandle, ctypes.byref(bsno))
dprint("IowKitGetSerialNumber", isn, bsno.raw.decode("utf-8"))

#Prepare for Write

# set read timeout to 1000 msecs
status = iowkit.IowKitSetTimeout(ioHandle, ctypes.c_ulong(1000));

# set write timeout to 1000 msecs
status = iowkit.IowKitSetWriteTimeout(ioHandle, ctypes.c_ulong(1000));

# Enable I2C
EnableI2C(ioHandle, 1)

# Write PWM16 values to LED-Warrior18
WritePwm16(ioHandle, 2048, 31522)

# Disable I2C
EnableI2C(ioHandle, 0)

# DON'T FORGETT TO CLOSE AT THE END OF THE SCRIPT!!!!!
iowkit.IowKitCloseDevice(ioHandle)