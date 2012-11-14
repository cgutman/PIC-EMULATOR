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

//Naive parsing function
int DecodeStringInput(char *opstr, char *opname, int *op1, int *op2)
{
    int i, matches;
    char hasMore;
    int op;
    char isHex[2];

    //Initialize output
    *op1 = *op2 = 0;

    //Convert to upper case
    for (i = 0; opstr[i]; i++)
    {
         //Truncate the string on the first control char
         if (iscntrl(opstr[i]))
         {
             opstr[i] = 0;
             break;
         }

         opstr[i] = toupper(opstr[i]);
    }

    //Truncate at the first non-alphabetic character
    hasMore = 0;
    for (i = 0; opstr[i]; i++)
    {
         if (!isalpha(opstr[i]))
         {
             opstr[i] = 0;
             hasMore = 1;
             break;
         }
    }

    //Copy the opname
    strcpy(opname, opstr);

    //Done if there's nothing left
    if (!hasMore)
    {
        return 0;
    }

    //Skip the opname and trailing NUL now
    opstr += strlen(opname) + 1;

    //Search for the 0x hex prefix
    op = 0;
    isHex[0] = 0;
    isHex[1] = 0;
    for (i = 0; opstr[i]; i++)
    {
         //Skip to next operand on comma
         if (opstr[i] == ',')
         {
             op++;
             continue;
         }

         //Check for the hex prefix
         if (opstr[i] == '0' && opstr[i+1] == 'X')
         {
             //Check for multiple prefixes
             if (isHex[op])
                 return -1;

             isHex[op] = 1;

             //Skip the X
             i++;
             continue;
         }

         //Check for a valid number
         if (!isHex[op] && !isdigit(opstr[i]) && !isspace(opstr[i]))
             return -1;
         else if (isHex[op] && !isdigit(opstr[i]) && !isspace(opstr[i]) &&
                  !(opstr[i] >= 'A' && opstr[i] <= 'F'))
             return -1;
    }

    if (op == 0)
    {
        if (isHex[0])
        {
            matches = sscanf(opstr, "%x", op1);
        }
        else
        {
            matches = sscanf(opstr, "%d", op1);
        }
    }
    else
    {
        if (isHex[0])
        {
            if (isHex[1])
            {
                matches = sscanf(opstr, "%x, %x", op1, op2);
            }
            else
            {
                matches = sscanf(opstr, "%x, %d", op1, op2);
            }
        }
        else
        {
            if (isHex[1])
            {
                matches = sscanf(opstr, "%d, %x", op1, op2);
            }
            else
            {
                matches = sscanf(opstr, "%d, %d", op1, op2);
            }
        }
    }

    //Make sure the string was well-formatted
    if (matches != op + 1)
        return -1;

    return 0;
}

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
                        opcode = 0xFFFF;
                    }
                    else
                    {
                        //Decode the string
                        err = DecodeStringInput(opstr, opname, &op1, &op2);
                        if (err < 0)
                        {
                            //Invalid input
                            opcode = 0xFFFF;
                        }
                        else
                        {
                            //Generate an opcode
                            opcode = OpGenerateOpcode(opname, (char)op1, (char)op2);
                        }
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
