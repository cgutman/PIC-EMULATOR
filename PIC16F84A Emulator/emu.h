//
//  emu.h
//  PIC16F84A Emulator
//
//  Licensed under GPLv3
//
//  Cameron Gutman (cameron.gutman@case.edu)
//

#ifndef PIC16F84A_Emulator_emu_h
#define PIC16F84A_Emulator_emu_h

#include "regs.h"
#include "cpu.h"

//This struct represents the emulator's state
typedef struct _EMU_STATE {
    PIC_CPU Cpu;
} EMU_STATE;

int EmuInitialize(EMU_STATE *State);
int EmuExecuteOpcode(EMU_STATE *State);
int EmuExecuteBytecode(unsigned char *Bytecode, int BytecodeLength);

#endif
