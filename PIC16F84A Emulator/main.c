//
//  main.c
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/13/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#include <stdio.h>

#include "emu.h"
#include "opcode.h"

int main(int argc, const char * argv[])
{
    char opname[16];
    int op1, op2;
    unsigned short opcode;
    int err;
    EMU_STATE state;

    if (argc <= 1)
    {
        printf("PIC Emulator - Interpreter Mode\n");
        
        err = EmuInitialize(&state);
        if (err < 0)
        {
            printf("Failed to initialize emulator\n");
            return err;
        }
        
        for (;;)
        {
            printf("Opcode: ");
            scanf("%s %d, %d", opname, &op1, &op2);

            //Generate an opcode
            opcode = OpGenerateOpcode(opname, (char)op1, (char)op2);

#ifdef DBG
            printf("Encoded opcode: 0x%x\n", opcode);

            //Print the opcode back
            printf("Decoded opcode: ");
            OpPrintOpcode(opcode);
            printf("\n");
#endif

            //Execute the opcode
            err = EmuExecuteOpcode(&state, opcode);
            if (err < 0)
                return err;
        }
    }
    else
    {
        //Not supported
    }
}
