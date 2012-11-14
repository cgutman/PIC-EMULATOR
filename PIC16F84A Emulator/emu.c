//
//  emu.c
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/13/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#include <stdio.h>

#include "emu.h"
#include "cpu.h"

int EmuInitialize(EMU_STATE *State)
{
    int err;

    //Initialize the CPU core state
    err = CpuInitializeCore(&State->Cpu);
    if (err < 0)
    {
        printf("Failed to initialize the CPU's core state\n");
        return err;
    }

    return 0;
}

int EmuExecuteOpcode(EMU_STATE *State)
{
    return CpuExec(&State->Cpu);
}

int EmuExecuteBytecode(unsigned char *Bytecode, int BytecodeLength)
{
    int err;
    EMU_STATE state;

    //Call common init function
    err = EmuInitialize(&state);
    if (err < 0)
    {
        printf("Failed to initialize emulator\n");
        return err;
    }

    //Initialize the CPU program memory
    err = CpuInitializeProgramMemory(&state.Cpu, Bytecode, BytecodeLength);
    if (err < 0)
    {
        printf("Failed to initialize the CPU's program memory");
        return err;
    }

    //Main emulator loop
    for (;;)
    {
        //Execute the next instruction
        err = CpuExec(&state.Cpu);
        if (err < 0)
            break;
    }
    
    return 0;
}
