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

int RunEmu(unsigned char *Bytecode, int BytecodeLength)
{
    int err;
    EMU_STATE state;

    //Initialize the CPU core state
    err = CpuInitializeCore(&state.Cpu);
    if (err < 0)
    {
        printf("Failed to initialize the CPU's core state\n");
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
