//
//  emu.h
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/13/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
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
