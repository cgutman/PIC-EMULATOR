# PIC-EMULATOR Makefile

CC=gcc
CFLAGS=-c -Wall -Werror

all: PIC-EMU

PIC-EMU: cpu.o emu.o main.o opcode.o regs.o stack.o
	$(CC) cpu.o emu.o main.o opcode.o regs.o stack.o -o PIC-EMU

cpu.o: cpu.c cpu.h opcode.h stack.h
	$(CC) $(CFLAGS) cpu.c

emu.o: emu.c emu.h cpu.h
	$(CC) $(CFLAGS) emu.c

main.o: main.c emu.h opcode.h
	$(CC) $(CFLAGS) main.c

opcode.o: opcode.c opcode.h
	$(CC) $(CFLAGS) opcode.c

regs.o: regs.c regs.h
	$(CC) $(CFLAGS) regs.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) stack.c

clean:
	rm -f *.o PIC-EMU
