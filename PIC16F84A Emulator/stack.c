//
//  stack.c
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/14/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
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
    unsigned char Location;

    //Update the location
    Location = Stack->NextTop - 1;
    Location %= PIC_STACK_ENTRIES;
    Stack->NextTop = Location;

    //Return the value that was there
    return Stack->Entries[Stack->NextTop];
}
