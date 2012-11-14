# Redirector to the real makefile in the "PIC16F84A Emulator" subfolder
#
# Dedicated to those who are as lazy as myself

all: PIC-EMU

PIC-EMU:
	cd "PIC16F84A Emulator"; make PIC-EMU; cp PIC-EMU ..

clean:
	cd "PIC16F84A Emulator"; make clean
