//
//  cpu.c
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/13/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#include <stdio.h>

#include "cpu.h"

int CpuInitializeCore(PIC_CPU *Cpu)
{
    //Initialize register file
    RegsInitializeRegisterFile(&Cpu->Regs);

    //W and SRAM state is left undefined

    //Success
    return 0;
}

int CpuInitializeProgramMemory(PIC_CPU *Cpu, unsigned char *buffer, int size)
{
    //Make sure the bytecode fits in the PIC's memory
    if (size > PROGRAM_MEM_SIZE)
    {
        printf("Program is too large for the PIC\n");
        return -1;
    }

    //Copy the bytecode into our private memory
    memcpy(Cpu->ProgMem, buffer, size);

    //Success
    return 0;
}

//Executes one instruction
int CpuExec(PIC_CPU *Cpu)
{
    return 0;
}