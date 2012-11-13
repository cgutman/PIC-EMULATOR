//
//  cpu.h
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/13/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#ifndef PIC16F84A_Emulator_cpu_h
#define PIC16F84A_Emulator_cpu_h

#include "regs.h"

//PIC's program memory size is 1024 words
#define PROGRAM_MEM_SIZE (1024 * 2)

//This struct represents the CPU state
typedef struct _PIC_CPU {
    REGISTER_FILE Regs;
    WORKING_REGISTER W;
    unsigned char *ProgMem[PROGRAM_MEM_SIZE];
} PIC_CPU;

int CpuInitializeProgramMemory(PIC_CPU *Cpu, unsigned char *buffer, int size);

int CpuInitializeCore(PIC_CPU *Cpu);

int CpuExec(PIC_CPU *Cpu);

#endif
