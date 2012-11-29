//
//  assembler.c
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/20/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "opcode.h"
#include "str.h"

#include "assembler.h"
#include "regs.h"

ASM_STRINT_TUPLE StaticTuples[] =
{
    //Registers
    {REG_INDF, "INDF"},
    {REG_TMR0, "TMR0"},
    {REG_PCL, "PCL"},
    {REG_STATUS, "STATUS"},
    {REG_FSR, "FSR"},
    {REG_PORTA, "PORTA"},
    {REG_PORTB, "PORTB"},
    {REG_EEDATA, "EEDATA"},
    {REG_EEADR, "EEADR"},
    {REG_PCLATH, "PCLATH"},
    {REG_INTCON, "INTCON"},
    {REG_OPTION_REG, "OPTION_REG"},
    {REG_TRISA, "TRISA"},
    {REG_TRISB, "TRISB"},
    {REG_EECON1, "EECON1"},
    
    //Destinations
    {DST_W, "W"},
    {DST_F, "F"},
    
    //Register bits
    {STATUS_RP0_BIT, "RP0"},
    {STATUS_PD_BIT, "PD"},
    {STATUS_Z_BIT, "Z"},
    {STATUS_DC_BIT, "DC"},
    {STATUS_C_BIT, "C"},
    
    //Terminator
    {0, NULL}
};

//Naive parsing function
int DecodeStringInput(ASM_CONTEXT *context, char *opstr, char *opname, int *op1, int *op2)
{
    char eater[255];
    int i, matches;
    char hasMore;
    int op;
    char isHex[2];
    int tempOp[2];
    
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
        int replacedOper;
        
        //Skip to next operand on comma
        if (opstr[i] == ',')
        {
            op++;
            continue;
        }
        
        //Check if we should skip
        if (*op1 != 0 && op == 0)
        {
            continue;
        }
        else if (*op2 != 0 && op == 1)
        {
            break;
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
        {
            //Make sure we're checking the start of the operand
            if (i == 0 || opstr[i - 1] == ',')
            {
                replacedOper = AsmGetOperandVal(context, &opstr[i]);
                if (replacedOper < 0)
                {
                    //Replacement failed
                    return -1;
                }
                else
                {
                    if (op == 0)
                    {
                        *op1 = -1;
                        tempOp[0] = replacedOper;
                    }
                    else
                    {
                        *op2 = -1;
                        tempOp[1] = replacedOper;
                    }
                    continue;
                }
                
            }
            
            return -1;
        }
        else if (isHex[op] && !isdigit(opstr[i]) && !isspace(opstr[i]) &&
                 !(opstr[i] >= 'A' && opstr[i] <= 'F'))
            return -1;
    }
    
    if (op == 0)
    {
        //Only write if no replacement was done
        if (*op1 == 0)
        {
            if (isHex[0])
            {
                matches = sscanf(opstr, "%x", &tempOp[0]);
            }
            else
            {
                matches = sscanf(opstr, "%d", &tempOp[0]);
            }
        }
        else
        {
            //Replaced all
            matches = 1;
        }
    }
    else
    {
        //No replacement of either operand
        if (*op1 == 0 && *op2 == 0)
        {
            if (isHex[0])
            {
                if (isHex[1])
                {
                    matches = sscanf(opstr, "%x , %x", &tempOp[0], &tempOp[1]);
                }
                else
                {
                    matches = sscanf(opstr, "%x , %d", &tempOp[0], &tempOp[1]);
                }
            }
            else
            {
                if (isHex[1])
                {
                    matches = sscanf(opstr, "%d , %x", &tempOp[0], &tempOp[1]);
                }
                else
                {
                    matches = sscanf(opstr, "%d , %d", &tempOp[0], &tempOp[1]);
                }
            }
        }
        //Replacement of both is nop
        else if (*op1 != 0 && *op2 != 0)
        {
            matches = 2;
        }
        //Replacement of op1
        else if (*op1 != 0)
        {
            if (isHex[0])
            {
                if (isHex[1])
                {
                    matches = sscanf(opstr, "%s , %x", eater, &tempOp[1]);
                }
                else
                {
                    matches = sscanf(opstr, "%s , %d", eater, &tempOp[1]);
                }
            }
            else
            {
                if (isHex[1])
                {
                    matches = sscanf(opstr, "%s , %x", eater, &tempOp[1]);
                }
                else
                {
                    matches = sscanf(opstr, "%s , %d", eater, &tempOp[1]);
                }
            }
            matches++;
        }
        //Replacement of op2
        else
        {
            if (isHex[0])
            {
                if (isHex[1])
                {
                    matches = sscanf(opstr, "%x ,", &tempOp[0]);
                }
                else
                {
                    matches = sscanf(opstr, "%x ,", &tempOp[0]);
                }
            }
            else
            {
                if (isHex[1])
                {
                    matches = sscanf(opstr, "%d ,", &tempOp[0]);
                }
                else
                {
                    matches = sscanf(opstr, "%d ,", &tempOp[0]);
                }
            }
            matches++;
        }
    }
    
    //Make sure the string was well-formatted
    if (matches != op + 1)
        return -1;
    
    //Store the variables back
    if (op >= 0)
        *op1 = tempOp[0];
    if (op >= 1)
        *op2 = tempOp[1];
    
    return 0;
}

int AsmInitializeContext(ASM_CONTEXT *context)
{
    int i;

    //Copy the static tuple table in
    for (i = 0; StaticTuples[i].String; i++)
    {
        context->TupleTable[i] = StaticTuples[i];
    }
    
    context->TupleCount = i;
    
    return 0;
}

int AsmGetOperandVal(ASM_CONTEXT *context, char *str)
{
    char truncatedChar = 0;
    int i;
    int ret = -1;

    //Truncate the string at the first space or comma
    for (i = 0; str[i]; i++)
    {
        if (isspace(str[i]) || str[i] == ',')
        {
            truncatedChar = str[i];
            str[i] = 0;
            break;
        }
    }
    
    //Search the string tuple table
    for (i = 0; i < context->TupleCount; i++)
    {
        if (!strcmp(str, context->TupleTable[i].String))
        {
            ret = context->TupleTable[i].Value;
            break;
        }
    }
    
    //Replace the character we removed if we removed one
    if (truncatedChar != 0)
    {
        //Go to the null
        for (i = 0; str[i]; i++);
        
        str[i] = truncatedChar;
    }
    
    return ret;
}

int GetNextLine(char *out, int outlen, char *in, int *inIdx)
{
    int i;

    if (in[*inIdx] == 0)
        return 0;
    
    for (i = 0; i < outlen - 1 && in[*inIdx] != '\n' && in[*inIdx] != 0; i++)
    {
        out[i] = in[(*inIdx)++];
        
    }
    
    //Skip the new line
    if (in[*inIdx] == '\n')
    {
        (*inIdx)++;
    }

    out[i] = 0;

    return 1;
}

ASM_PROGRAM *AsmAssembleAscii(ASM_CONTEXT *context, char *fbuffer, int size)
{
#define MAX_INPUT_LEN 32
#define MAX_OPNAME_LEN 16
    char opname[MAX_OPNAME_LEN], opstr[MAX_INPUT_LEN];
    int op1, op2;
    ASM_PROGRAM *program;
    int opcodes = 0;
    int i;
    int j;
    int err;
    
    //Stupid way of counting opcodes
    for (i = 0; i < size; i++)
    {
        if (fbuffer[i] == '\n')
        {
            opcodes++;
        }
    }
    
    //Allocate bytecode buffer
    program = malloc(opcodes * sizeof(PIC_OPCODE) + sizeof(int));
    if (!program)
        return NULL;
    
    //Decode opcodes
    i = 0;
    for (j = 0; j < opcodes; j++)
    {
        unsigned short opcode;

        if (!GetNextLine(opstr, MAX_INPUT_LEN, fbuffer, &i))
            break;

        //Decode the string
        err = DecodeStringInput(context, opstr, opname, &op1, &op2);
        if (err < 0)
        {
            printf("Illegal opcode: %s\n", opstr);
            return NULL;
        }
        
        //Generate an opcode
        opcode = OpGenerateOpcode(opname, (char)op1, (char)op2);
        if (opcode == OP_INVALID)
        {
            printf("Illegal opcode: %s\n", opstr);
            return NULL;
        }
        
        program->Opcodes[j].Opcode = opcode;
    }
    
    program->OpcodeCount = opcodes;
    
    return program;
}