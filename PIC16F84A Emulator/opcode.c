//
//  opcode.c
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/13/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#include <stdio.h>

#include "opcode.h"

unsigned short OpGenerateOpcode(char *opname, char operand1, char operand2)
{
    unsigned short opcode = 0xFFFF;

    if (!strcmp(opname, "ADDWF"))
    {
        opcode = OP_ADDWF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "ANDWF"))
    {
        opcode = OP_ANDWF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "CLRF"))
    {
        opcode = OP_CLRF;
        opcode |= (operand1 & 0x7F);
    }
    else if (!strcmp(opname, "CLRW"))
    {
        opcode = OP_CLRW;
    }
    else if (!strcmp(opname, "COMF"))
    {
        opcode = OP_COMF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "DECF"))
    {
        opcode = OP_DECF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "DECFSZ"))
    {
        opcode = OP_DECFSZ;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "INCF"))
    {
        opcode = OP_INCF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "INCFSZ"))
    {
        opcode = OP_INCFSZ;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "IORWF"))
    {
        opcode = OP_IORWF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "MOVF"))
    {
        opcode = OP_MOVF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "MOVWF"))
    {
        opcode = OP_MOVWF;
        opcode |= (operand1 & 0x7F);
    }
    else if (!strcmp(opname, "NOP"))
    {
        opcode = 0;
    }
    else if (!strcmp(opname, "RLF"))
    {
        opcode = OP_RLF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "RRF"))
    {
        opcode = OP_RRF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "SUBWF"))
    {
        opcode = OP_SUBWF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "SWAPF"))
    {
        opcode = OP_SWAPF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "XORWF"))
    {
        opcode = OP_COMF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x80;
    }
    else if (!strcmp(opname, "BCF"))
    {
        opcode = OP_BCF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x380;
    }
    else if (!strcmp(opname, "BSF"))
    {
        opcode = OP_BSF;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x380;
    }
    else if (!strcmp(opname, "BTFSC"))
    {
        opcode = OP_BTFSC;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x380;
    }
    else if (!strcmp(opname, "BTFSS"))
    {
        opcode = OP_BTFSS;
        opcode |= (operand1 & 0x7F);
        opcode |= (operand2 << 7) & 0x380;
    }
    else if (!strcmp(opname, "ADDLW"))
    {
        opcode = OP_ADDLW;
        opcode |= (operand1 & 0xFF);
    }
    else if (!strcmp(opname, "ANDLW"))
    {
        opcode = OP_ANDLW;
        opcode |= (operand1 & 0xFF);
    }
    else if (!strcmp(opname, "CALL"))
    {
        opcode = OP_CALL;
        opcode |= (operand1 & 0x7FF);
    }
    else if (!strcmp(opname, "CLRWDT"))
    {
        opcode = OP_CLRWDT;
    }
    else if (!strcmp(opname, "GOTO"))
    {
        opcode = OP_GOTO;
        opcode |= (operand1 & 0x7FF);
    }
    else if (!strcmp(opname, "IORLW"))
    {
        opcode = OP_IORLW;
        opcode |= (operand1 & 0xFF);
    }
    else if (!strcmp(opname, "MOVLW"))
    {
        opcode = OP_MOVLW;
        opcode |= (operand1 & 0xFF);
    }
    else if (!strcmp(opname, "RETFIE"))
    {
        opcode = OP_RETFIE;
    }
    else if (!strcmp(opname, "RETLW"))
    {
        opcode = OP_RETLW;
        opcode |= (operand1 & 0xFF);
    }
    else if (!strcmp(opname, "RETURN"))
    {
        opcode = OP_RETURN;
    }
    else if (!strcmp(opname, "SLEEP"))
    {
        opcode = OP_SLEEP;
    }
    else if (!strcmp(opname, "SUBLW"))
    {
        opcode = OP_SUBLW;
        opcode |= (operand1 & 0xFF);
    }
    else if (!strcmp(opname, "XORLW"))
    {
        opcode = OP_XORLW;
        opcode |= (operand1 & 0xFF);
    }

    return opcode;
}

void OpPrintOpcode(unsigned short opcode)
{
    //Make sure this is 14-bit
    if ((opcode & 0xC000) != 0)
    {
        printf("Invalid high bits in opcode");
        return;
    }

    //Classify the opcode by the highest byte
    if ((opcode & 0x3000) == 0x0000)
    {
        //Byte-oriented file register operations
        switch (opcode & 0xF00)
        {
            case OP_ADDWF:
                printf("ADDWF %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case OP_ANDWF:
                printf("ANDWF %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case 0x100:
                if ((opcode & 0x80) != 0)
                {
                    printf("CLRF %d", (opcode & 0x7F));
                }
                else
                {
                    printf("CLRW");
                }
                break;
            case OP_COMF:
                printf("COMF %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case OP_DECF:
                printf("DECF %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case OP_DECFSZ:
                printf("DECFSZ %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case OP_INCF:
                printf("INCF %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case OP_INCFSZ:
                printf("INCFSZ %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case OP_IORWF:
                printf("IORWF %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case OP_MOVF:
                printf("MOVF %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case 0x000:
                if ((opcode & 0x80) != 0)
                {
                    printf("MOVWF %d", (opcode & 0x7F));
                }
                else if ((opcode & 0xF) == 0)
                {
                    printf("NOP");
                }
                else
                {
                    //Control ops
                    switch (opcode & 0xFF)
                    {
                        case OP_CLRWDT:
                            printf("CLRWDT");
                            break;
                        case OP_RETFIE:
                            printf("RETFIE");
                            break;
                        case OP_RETURN:
                            printf("RETURN");
                            break;
                        case OP_SLEEP:
                            printf("SLEEP");
                            break;
                        default:
                            printf("Invalid 00 0000 opcode!");
                            break;
                    }
                }
                break;
            case OP_RLF:
                printf("RLF %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case OP_RRF:
                printf("RRF %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case OP_SUBWF:
                printf("SUBWF %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case OP_SWAPF:
                printf("SWAPF %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            case OP_XORWF:
                printf("XORWF %d, %d", (opcode & 0x7F), (opcode & 0x80) >> 7);
                break;
            default:
                printf("Invalid 00 opcode!");
                break;
        }
    }
    else if ((opcode & 0x3000) == 0x1000)
    {
        //Bit-oriented file register operations
        switch (opcode & 0x3C00)
        {
            case OP_BCF:
                printf("BCF %d, %d", (opcode & 0x7F), (opcode & 0x380) >> 7);
                break;
            case OP_BSF:
                printf("BSF %d, %d", (opcode & 0x7F), (opcode & 0x380) >> 7);
                break;
            case OP_BTFSC:
                printf("BTFSC %d, %d", (opcode & 0x7F), (opcode & 0x380) >> 7);
                break;
            case OP_BTFSS:
                printf("BTFSS %d, %d", (opcode & 0x7F), (opcode & 0x380) >> 7);
                break;
            default:
                printf("Invalid 01 opcode!");
                break;
        }
    }
    else if ((opcode & 0x3000) == 0x2000)
    {
        //GOTO / CALL
        if ((opcode & 0x800) != 0)
        {
            printf("GOTO %d", (opcode & 0x7FF));
        }
        else
        {
            printf("CALL %d", (opcode & 0x7FF));
        }
    }
    else //0x3000
    {
        if ((opcode & 0xE00) == 0xE00)
        {
            printf("ADDLW %d", (opcode & 0xFF));
        }
        else if ((opcode & 0xF00) == 0x900)
        {
            printf("ANDLW %d", (opcode & 0xFF));
        }
        else if ((opcode & 0xF00) == 0x800)
        {
            printf("IORLW %d", (opcode & 0xFF));
        }
        else if ((opcode & 0xF00) == 0xA00)
        {
            printf("XORLW %d", (opcode & 0xFF));
        }
        else if ((opcode & 0xC00) == 0x000)
        {
            printf("MOVLW %d", (opcode & 0xFF));
        }
        else if ((opcode & 0xC00) == 0x400)
        {
            printf("RETLW %d", (opcode & 0xFF));
        }
        else if ((opcode & 0xC00) == 0xC00)
        {
            printf("SUBLW %d", (opcode & 0xFF));
        }
        else
        {
            printf("Invalid 11 opcode!");
        }
    }
}