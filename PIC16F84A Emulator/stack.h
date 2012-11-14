//
//  stack.h
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/14/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#ifndef PIC16F84A_Emulator_stack_h
#define PIC16F84A_Emulator_stack_h

#define PIC_STACK_ENTRIES 8

typedef struct _PIC_STACK {
    unsigned char Entries[PIC_STACK_ENTRIES];
    unsigned char NextTop;
} PIC_STACK;

void StkPush(PIC_STACK *Stack, unsigned char Data);
unsigned char StkPop(PIC_STACK *Stack);

void StkInitialize(PIC_STACK *Stack);

#endif
