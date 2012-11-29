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
#include <stdlib.h>

#include "emu.h"
#include "opcode.h"

int main(int argc, const char * argv[])
{
#define MAX_INPUT_LEN 32
#define MAX_OPNAME_LEN 16
    char opname[MAX_OPNAME_LEN], opstr[MAX_INPUT_LEN];
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
                    if (!fgets(opstr, MAX_INPUT_LEN, stdin))
                    {
                        //Invalid input
                        opcode = OP_INVALID;
                    }
                    else
                    {
                        //Decode the string
                        err = DecodeStringInput(&state.AsmContext, opstr, opname, &op1, &op2);
                        if (err < 0)
                        {
                            //Invalid input
                            opcode = OP_INVALID;
                        }
                        else
                        {
                            //Generate an opcode
                            opcode = OpGenerateOpcode(opname, (char)op1, (char)op2);
                        }
                    }
                }
                while (opcode == OP_INVALID);
                
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
        FILE *f;
        int size;
        unsigned char *fbuffer;
        
        printf("PIC Emulator - %s\n", argv[2]);
        
        //Open the input file
        f = fopen(argv[2], "r");
        if (f == NULL)
        {
            printf("Failed to open the input file\n");
            return -1;
        }
        
        //Grab file size
        fseek(f, 0, SEEK_END);
        size = (int)ftell(f);
        rewind(f);
        
        //Allocate a buffer for the file contents
        fbuffer = malloc(size);
        if (!fbuffer)
            return -1;
        
        //Read in the file
        fread(fbuffer, 1, size, f);

        //Rewind the file
        rewind(f);
        
        //Initialize the emulator
        err = EmuInitialize(&state);
        if (err < 0)
        {
            printf("Failed to initialize emulator\n");
            return err;
        }
        
        //Binary mode
        if (toupper(*argv[1]) == 'B')
        {
            printf("Binary Mode\n");

            //Execute the bytecode
            err = EmuExecuteBytecode(fbuffer, size);
            if (err < 0)
            {
                printf("Failed to execute bytecode\n");
                return err;
            }
        }
        //ASCII mode
        else if (toupper(*argv[1]) == 'A')
        {
            printf("ASCII Mode\n");

            //Assemble and execute the ASCII
            err = EmuAssembleAndExecute((char*)fbuffer, size);
            if (err < 0)
            {
                printf("Failed to execute bytecode\n");
                return err;
            }
        }
    }

    return 0;
}
