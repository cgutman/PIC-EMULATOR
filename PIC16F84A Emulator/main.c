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
    char opname[16], opstr[32];
    int op1, op2;
    int err;
    EMU_STATE state;
    unsigned char badops[PROGRAM_MEM_SIZE];

    
    if (argc <= 1)
    {
        //FIXME: This violates my CPU <-> EMU abstraction

        printf("PIC Emulator - Interpreter Mode\n");
        
        err = EmuInitialize(&state);
        if (err < 0)
        {
            printf("Failed to initialize emulator\n");
            return err;
        }
        
        memset(badops, 0xFF, PROGRAM_MEM_SIZE);
        err = CpuInitializeProgramMemory(&state.Cpu, badops, PROGRAM_MEM_SIZE);
        if (err < 0)
        {
            printf("Failed to initialize program memory\n");
            return err;
        }
        
        for (;;)
        {
            unsigned short PC = CpuGetPC(&state.Cpu);
            unsigned short opcode = CpuGetOpcode(&state.Cpu, PC);

            //Check if there's an opcode already decoded for this
            //FIXME: PIC_OPCODE_MASK is a valid opcode
            if (opcode != PIC_OPCODE_MASK)
            {
                //Print the opcode back
                printf("Opcode 0x%x: ", PC);
                OpPrintOpcode(opcode);
                printf("\n");
            }
            else
            {
                do
                {
                    //Get a new opcode from the console
                    printf("Opcode 0x%x: ", PC);
                    scanf("%s", opstr);
                    sscanf(opstr, "%s %d, %d", opname, &op1, &op2);
                    
                    //Generate an opcode
                    opcode = OpGenerateOpcode(opname, (char)op1, (char)op2);
                } while (opcode == 0xFFFF);
                
                //Write the opcode to program memory
                CpuSetOpcode(&state.Cpu, PC, opcode);
            }

            //Execute the next opcode
            err = EmuExecuteOpcode(&state);
            if (err < 0)
                return err;
        }
    }
    else
    {
        //Not supported
    }
}
