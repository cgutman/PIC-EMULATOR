//
//  stack.c
//  PIC16F84A Emulator
//
//  Licensed under GPLv3
//
//  Cameron Gutman (cameron.gutman@case.edu)
//

#include <stdio.h>

#include "stack.h"

void StkInitialize(PIC_STACK *Stack)
{
    Stack->NextTop = 0;
}

void StkPush(PIC_STACK *Stack, unsigned char Data)
{
    //Store the data in the next location
    Stack->Entries[Stack->NextTop] = Data;

    //Update the next top
    Stack->NextTop = Stack->NextTop + 1;
    Stack->NextTop %= PIC_STACK_ENTRIES;
}

unsigned char StkPop(PIC_STACK *Stack)
{
    //Update the next top
    Stack->NextTop = Stack->NextTop - 1;
    Stack->NextTop %= PIC_STACK_ENTRIES;

    //Return the value that was there
    return Stack->Entries[Stack->NextTop];
}
