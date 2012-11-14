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
#include "opcode.h"

//PIC's program memory size is 1024 instructions
#define PROGRAM_MEM_INSTRUCTIONS 0x400
#define PROGRAM_MEM_SIZE ((PROGRAM_MEM_INSTRUCTIONS * PIC_OPCODE_BITS) / 0x08)

//PIC's opcodes are 14-bit
#define PIC_OPCODE_MASK 0x3FFF

//PIC's opcodes are 14 bits
#define PIC_OPCODE_BITS 0xE

//This struct represents the CPU state
typedef struct _PIC_CPU {
    REGISTER_FILE Regs;
    WORKING_REGISTER W;
    PIC_OPCODE ProgMem[PROGRAM_MEM_INSTRUCTIONS];
} PIC_CPU;

int CpuInitializeProgramMemory(PIC_CPU *Cpu, unsigned char *buffer, int size);

int CpuInitializeCore(PIC_CPU *Cpu);

int CpuExec(PIC_CPU *Cpu);

unsigned short CpuExecuteOpcode(PIC_CPU *Cpu, short opcode, unsigned short PC);

void CpuSetOpcode(PIC_CPU *Cpu, unsigned short PC, unsigned short Opcode);
unsigned short CpuGetOpcode(PIC_CPU *Cpu, unsigned short PC);

void CpuSetPC(PIC_CPU *Cpu, unsigned short PC);
unsigned short CpuGetPC(PIC_CPU *Cpu);

#endif
