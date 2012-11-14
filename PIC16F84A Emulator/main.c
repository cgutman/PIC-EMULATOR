//
//  main.c
//  PIC16F84A Emulator
//
//  Licensed under GPLv3
//
//  Cameron Gutman (cameron.gutman@case.edu)
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "emu.h"
#include "opcode.h"

int main(int argc, const char * argv[])
{
#define MAX_INPUT_LEN 32
#define MAX_OPNAME_LEN 16
    char opname[MAX_OPNAME_LEN], opstr[MAX_INPUT_LEN];
    int op1, op2;
    int err;
    int i;
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
                    if (!fgets(opstr, MAX_INPUT_LEN, stdin))
                    {
                        //Invalid input
                        opcode = 0xFFFF;
                    }
                    else
                    {
                        //Convert to upper case
                        for (i = 0; i < MAX_INPUT_LEN; i++)
                        {
                             if (opstr[i] == 0)
                                 break;

                             opstr[i] = toupper(opstr[i]);
                        }

                        //Decode the input
                        sscanf(opstr, "%s %d, %d", opname, &op1, &op2);
                        
                        //Generate an opcode
                        opcode = OpGenerateOpcode(opname, (char)op1, (char)op2);
                    }
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

    return 0;
}
