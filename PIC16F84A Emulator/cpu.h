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

//PIC's program memory size is 1024 instructions
#define PROGRAM_MEM_SIZE 0x400

//PIC's opcodes are 14-bit
#define PIC_OPCODE_MASK 0x3FFF

//PIC's opcodes are 14 bits
#define PIC_OPCODE_SIZE 0xE

//This struct represents the CPU state
typedef struct _PIC_CPU {
    REGISTER_FILE Regs;
    WORKING_REGISTER W;
    unsigned char *ProgMem[PROGRAM_MEM_SIZE];
} PIC_CPU;

int CpuInitializeProgramMemory(PIC_CPU *Cpu, unsigned char *buffer, int size);

int CpuInitializeCore(PIC_CPU *Cpu);

int CpuExec(PIC_CPU *Cpu);

int CpuExecuteOpcode(PIC_CPU *Cpu, short opcode, unsigned short PC);

#endif
