//
//  assembler.h
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/27/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#ifndef PIC16F84A_Emulator_assembler_h
#define PIC16F84A_Emulator_assembler_h

#define MAX_TUPLES 0xFF

typedef struct _ASM_PROGRAM
{
    int OpcodeCount;
    PIC_OPCODE Opcodes[1];
} ASM_PROGRAM;

typedef struct _ASM_STRINT_TUPLE
{
    int Value;
    char *String;
} ASM_STRINT_TUPLE;

typedef struct _ASM_CONTEXT
{
    ASM_STRINT_TUPLE TupleTable[MAX_TUPLES];
    int TupleCount;
} ASM_CONTEXT;

int AsmGetOperandVal(ASM_CONTEXT *context, char *str);
int AsmInitializeContext(ASM_CONTEXT *context);
ASM_PROGRAM *AsmAssembleAscii(ASM_CONTEXT *context, char *buff, int size);

//FIXME: Shouldn't be exposed
int DecodeStringInput(ASM_CONTEXT *context, char *opstr, char *opname, int *op1, int *op2);

#endif
