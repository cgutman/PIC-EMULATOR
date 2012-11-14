//
//  cpu.c
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/13/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#include <stdio.h>

#include "cpu.h"
#include "opcode.h"

int CpuInitializeCore(PIC_CPU *Cpu)
{
    //Initialize register file
    RegsInitializeRegisterFile(&Cpu->Regs);

    //W and SRAM state is left undefined

    //Success
    return 0;
}

int CpuInitializeProgramMemory(PIC_CPU *Cpu, unsigned char *buffer, int size)
{
    //Make sure the bytecode fits in the PIC's memory
    if (size > PROGRAM_MEM_SIZE)
    {
        printf("Program is too large for the PIC\n");
        return -1;
    }

    //Make sure the bytecode size is a multiple of the PIC instruction length
    if (((size * 0x08) % PIC_OPCODE_BITS) != 0)
    {
        printf("Program is not valid PIC bytecode\n");
        return -1;
    }

    //Copy the bytecode into our private memory
    memcpy(Cpu->ProgMem, buffer, size);

    //Success
    return 0;
}

unsigned short CpuGetPC(PIC_CPU *Cpu)
{
    //Construct the full PC and do wrap-around handling
    return ((Cpu->Regs.PCLATH << 8) | Cpu->Regs.PCL) & (PROGRAM_MEM_INSTRUCTIONS - 1);
}

void CpuSetPC(PIC_CPU *Cpu, unsigned short PC)
{
    //Write the new PC back
    Cpu->Regs.PCLATH = (PC << 8) & 0x1F;
    Cpu->Regs.PCL = (PC & 0xFF);
}

unsigned short CpuGetOpcode(PIC_CPU *Cpu, unsigned short PC)
{
    return (Cpu->ProgMem[PC].Opcode) & PIC_OPCODE_MASK;
}

void CpuSetOpcode(PIC_CPU *Cpu, unsigned short PC, unsigned short Opcode)
{
    Cpu->ProgMem[PC].Opcode = Opcode & PIC_OPCODE_MASK;
}

unsigned short CpuExecuteOpcode(PIC_CPU *Cpu, short opcode, unsigned short PC)
{
    unsigned char result;
    unsigned char op1, op2;
    unsigned char status = Cpu->Regs.STATUS;
    unsigned char oldW = Cpu->W;
    unsigned char oldStatus = status;

    //Make sure this is 14-bit
    if ((opcode & 0xC000) != 0)
    {
        printf("Invalid high bits in opcode\n");
        return 0xFFFF;
    }

    //Skip to the next instruction
    PC ++;
    
    //Classify the opcode by the highest byte
    if ((opcode & 0x3000) == 0x0000)
    {
        //Byte-oriented file register operations
        switch (opcode & 0xF00)
        {
            case OP_ADDWF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;

                //If the low 4-bit add overflowed, set the DC bit
                result = (Cpu->W & 0x0F) + (RegsGetValue(&Cpu->Regs, op1) & 0x0F);
                if ((result & 0x10) != 0)
                    status |= STATUS_DC;
                else
                    status &= ~STATUS_DC;
                
                //If the high 4-bit add overflowed, set the C bit
                result = (Cpu->W >> 4) + (RegsGetValue(&Cpu->Regs, op1) >> 4);
                result += (status & STATUS_DC) ? 1 : 0;
                if ((result & 0x10) != 0)
                    status |= STATUS_C;
                else
                    status &= ~STATUS_C;
                
                //Do the operation
                result = Cpu->W + RegsGetValue(&Cpu->Regs, op1);
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);
                
                //Set status flags
                if (result == 0)
                    status |= STATUS_Z;
                else
                    status &= ~STATUS_Z;
                break;
                
            case OP_ANDWF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //Execute the operation
                result = Cpu->W & RegsGetValue(&Cpu->Regs, op1);
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);
                
                //Set status flags
                if (result == 0)
                    status |= STATUS_Z;
                else
                    status &= ~STATUS_Z;

                break;
        
            case 0x100:
                if ((opcode & 0x80) != 0)
                {
                    //CLRF

                    //Get the operand
                    op1 = (opcode & 0x7F);
                    
                    //Execute the operation
                    RegsSetValue(&Cpu->Regs, op1, 0);
                    
                    //Set status flags
                    status |= STATUS_Z;
                }
                else
                {
                    //CLRW
                    
                    //Execute the operation
                    Cpu->W = 0;

                    //Set status flags
                    status |= STATUS_Z;
                }
                break;

            case OP_COMF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //Execute the operation
                result = ~ RegsGetValue(&Cpu->Regs, op1);
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);
                
                //Set status flags
                if (result == 0)
                    status |= STATUS_Z;
                else
                    status &= ~STATUS_Z;
                
                break;
                
            case OP_DECF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //Execute the operation
                result = RegsGetValue(&Cpu->Regs, op1) - 1;
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);
                
                //Set status flags
                if (result == 0)
                    status |= STATUS_Z;
                else
                    status &= ~STATUS_Z;
                
                break;
            case OP_DECFSZ:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //Execute the operation
                result = RegsGetValue(&Cpu->Regs, op1) - 1;
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);
                
                //Skip next instruction if 0
                if (result == 0)
                    PC ++;
                
                break;

            case OP_INCF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //Execute the operation
                result = RegsGetValue(&Cpu->Regs, op1) + 1;
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);
                
                //Set status flags
                if (result == 0)
                    status |= STATUS_Z;
                else
                    status &= ~STATUS_Z;
                
                break;
                
            case OP_INCFSZ:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //Execute the operation
                result = RegsGetValue(&Cpu->Regs, op1) + 1;
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);
                
                //Skip next instruction if 0
                if (result == 0)
                    PC ++;
                break;
                
            case OP_IORWF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //Execute the operation
                result = Cpu->W | RegsGetValue(&Cpu->Regs, op1);
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);
                
                //Set status flags
                if (result == 0)
                    status |= STATUS_Z;
                else
                    status &= ~STATUS_Z;
                break;
                
            case OP_MOVF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //Execute the operation
                result = RegsGetValue(&Cpu->Regs, op1);
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);
                
                //Set status flags
                if (result == 0)
                    status |= STATUS_Z;
                else
                    status &= ~STATUS_Z;
                break;

            case 0x000:
                if ((opcode & 0x80) != 0)
                {
                    //MOVWF

                    //Get the operand
                    op1 = (opcode & 0x7F);
                    
                    //Execute the operation
                    RegsSetValue(&Cpu->Regs, op1, Cpu->W);
                }
                else if ((opcode & 0xF) == 0)
                {
                    //Nop
                }
                else
                {
                    //Control ops
                    switch (opcode & 0xFF)
                    {
                        case OP_CLRWDT:
                            printf("CLRWDT\n");
                            break;
                        case OP_RETFIE:
                            printf("RETFIE\n");
                            break;
                        case OP_RETURN:
                            printf("RETURN\n");
                            break;
                        case OP_SLEEP:
                            printf("SLEEP\n");
                            break;
                        default:
                            printf("Invalid 00 0000 opcode!\n");
                            break;
                    }
                    return 0xFFFF;
                }
                break;
                
            case OP_RLF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //Execute the operation
                result = RegsGetValue(&Cpu->Regs, op1);
                
                //Bit 7 -> Carry bit
                if ((result & 0x80) != 0)
                    status |= STATUS_C;
                else
                    status &= ~STATUS_C;

                //Shift left 1
                result = result << 1;
                
                //Carry bit -> Bit 0
                if ((Cpu->Regs.STATUS & STATUS_C) != 0)
                    result |= 0x01;
                else
                    result &= ~0x01;
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);

                break;
                
            case OP_RRF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //Execute the operation
                result = RegsGetValue(&Cpu->Regs, op1);
                
                //Bit 0 -> Carry bit
                if (result & 0x01)
                    status |= STATUS_C;
                else
                    status &= ~STATUS_C;
                
                //Shift right 1
                result = result >> 1;
                
                //Carry bit -> Bit 7
                if ((Cpu->Regs.STATUS & STATUS_C) != 0)
                    result |= 0x80;
                else
                    result &= ~0x80;
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);

                break;
                
            case OP_SUBWF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //If the low 4-bit sub underflowed, set the DC bit
                result = (RegsGetValue(&Cpu->Regs, op1) & 0x0F) - (Cpu->W & 0x0F);
                
                //Polarity is reversed for SUB
                if ((result & 0x10) == 0)
                    status |= STATUS_DC;
                else
                    status &= ~STATUS_DC;
                
                //If the high 4-bit sub underflowed, set the C bit
                result = RegsGetValue(&Cpu->Regs, op1) >> 4;
                result -= (status & STATUS_DC) ? 0 : 1;
                result -= Cpu->W >> 4;
                
                //Polarity is reversed for SUB
                if ((result & 0x10) == 0)
                    status |= STATUS_C;
                else
                    status &= ~STATUS_C;
                
                //Execute the operation
                result = RegsGetValue(&Cpu->Regs, op1) - Cpu->W;
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);
                
                //Set status flags
                if (result == 0)
                    status |= STATUS_Z;
                else
                    status &= ~STATUS_Z;
                
                break;

            case OP_SWAPF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //Execute the operation
                result = (RegsGetValue(&Cpu->Regs, op1) << 4) & 0xF0;
                result |= (RegsGetValue(&Cpu->Regs, op1) >> 4) & 0x0F;
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);
                
                break;
                
            case OP_XORWF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x80) >> 7;
                
                //Execute the operation
                result = Cpu->W ^ RegsGetValue(&Cpu->Regs, op1);
                
                //Store the results
                if (op2 == 0)
                    Cpu->W = result;
                else
                    RegsSetValue(&Cpu->Regs, op1, result);
                
                //Set status flags
                if (result == 0)
                    status |= STATUS_Z;
                else
                    status &= ~STATUS_Z;
                break;
            default:
                printf("Invalid 00 opcode!\n");
                return 0xFFFF;
        }
    }
    else if ((opcode & 0x3000) == 0x1000)
    {
        //Bit-oriented file register operations
        switch (opcode & 0x3C00)
        {
            case OP_BCF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x380) >> 7;
                
                //Execute the operation
                result = RegsGetValue(&Cpu->Regs, op1) & ~(1 << op2);
                
                //Store the results
                RegsSetValue(&Cpu->Regs, op1, result);
                break;
                
            case OP_BSF:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x380) >> 7;
                
                //Execute the operation
                result = RegsGetValue(&Cpu->Regs, op1) | (1 << op2);
                
                //Store the results
                RegsSetValue(&Cpu->Regs, op1, result);
                break;
                
            case OP_BTFSC:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x380) >> 7;
                
                //Execute the operation
                result = RegsGetValue(&Cpu->Regs, op1) & (1 << op2);
                
                //Skip the next instruction
                if (result == 0)
                    PC ++;
                break;
                
            case OP_BTFSS:
                //Get the operands
                op1 = (opcode & 0x7F);
                op2 = (opcode & 0x380) >> 7;
                
                //Execute the operation
                result = RegsGetValue(&Cpu->Regs, op1) & (1 << op2);
                
                //Skip the next instruction
                if (result != 0)
                    PC ++;
                break;
                
            default:
                printf("Invalid 01 opcode!\n");
                return 0xFFFF;
        }
    }
    else if ((opcode & 0x3000) == 0x2000)
    {
        //GOTO / CALL
        if ((opcode & 0x800) != 0)
        {
            //GOTO

            //Update the PC
            PC = (opcode & 0x7FF);
        }
        else
        {
            printf("CALL %d\n", (opcode & 0x7FF));
            return 0xFFFF;
        }
    }
    else //0x3000
    {
        if ((opcode & 0xE00) == 0xE00)
        {
            //ADDLW
            
            //Get the operand
            op1 = (opcode & 0xFF);

            //If the low 4-bit add overflowed, set the DC bit
            result = (Cpu->W & 0x0F) + (op1 & 0x0F);
            if ((result & 0x10) != 0)
                status |= STATUS_DC;
            else
                status &= ~STATUS_DC;
                        
            //If the high 4-bit add overflowed, set the C bit
            result = (Cpu->W >> 4) + (op1 >> 4);
            result += (status & STATUS_DC) ? 1 : 0;
            if ((result & 0x10) != 0)
                status |= STATUS_C;
            else
                status &= ~STATUS_C;

            //Do the operation
            result = Cpu->W + op1;
            
            //Store the results
            Cpu->W = result;
            
            //Set status flags
            if (result == 0)
                status |= STATUS_Z;
            else
                status &= ~STATUS_Z;
        }
        else if ((opcode & 0xF00) == 0x900)
        {
            //ANDLW
            
            //Get the operand
            op1 = (opcode & 0xFF);
            
            //Execute the operation
            result = Cpu->W & op1;
            
            //Store the results
            Cpu->W = result;
            
            //Set status flags
            if (result == 0)
                status |= STATUS_Z;
            else
                status &= ~STATUS_Z;
        }
        else if ((opcode & 0xF00) == 0x800)
        {
            //IORLW
            
            //Get the operand
            op1 = (opcode & 0xFF);
            
            //Execute the operation
            result = Cpu->W | op1;
            
            //Store the results
            Cpu->W = result;
            
            //Set status flags
            if (result == 0)
                status |= STATUS_Z;
            else
                status &= ~STATUS_Z;
        }
        else if ((opcode & 0xF00) == 0xA00)
        {
            //XORLW
            
            //Get the operand
            op1 = (opcode & 0xFF);
            
            //Execute the operation
            result = Cpu->W ^ op1;
            
            //Store the results
            Cpu->W = result;
            
            //Set status flags
            if (result == 0)
                status |= STATUS_Z;
            else
                status &= ~STATUS_Z;
        }
        else if ((opcode & 0xC00) == 0x000)
        {
            //MOVLW
            
            //Get the operand
            op1 = (opcode & 0xFF);
            
            //Execute the operation
            result = op1;
            
            //Store the results
            Cpu->W = result;
        }
        else if ((opcode & 0xC00) == 0x400)
        {
            printf("RETLW %d\n", (opcode & 0xFF));
            return 0xFFFF;
        }
        else if ((opcode & 0xC00) == 0xC00)
        {
            //SUBLW
            
            //Get the operand
            op1 = (opcode & 0xFF);
            
            //If the low 4-bit sub underflowed, set the DC bit
            result = (op1 & 0x0F) - (Cpu->W & 0x0F);
            
            //Polarity is reversed for SUB
            if ((result & 0x10) == 0)
                status |= STATUS_DC;
            else
                status &= ~STATUS_DC;
            
            //If the high 4-bit sub underflowed, set the C bit
            result = op1 >> 4;
            result -= (status & STATUS_DC) ? 0 : 1;
            result -= Cpu->W >> 4;
            
            //Polarity is reversed for SUB
            if ((result & 0x10) == 0)
                status |= STATUS_C;
            else
                status &= ~STATUS_C;
            
            //Execute the operation
            result = op1 - Cpu->W;
            
            //Store the results
            Cpu->W = result;
            
            //Set status flags
            if (result == 0)
                status |= STATUS_Z;
            else
                status &= ~STATUS_Z;
        }
        else
        {
            printf("Invalid 11 opcode!\n");
            return 0xFFFF;
        }
    }
    
    if (status != oldStatus)
    {
        printf("STATUS:[");
        RegsPrintStatusRegister(Cpu->Regs.STATUS);
        printf("] -> [");
        Cpu->Regs.STATUS = status;
        RegsPrintStatusRegister(Cpu->Regs.STATUS);
        printf("]\n");
    }

    if (oldW != Cpu->W)
    {
        printf("W: %d -> %d\n", oldW, Cpu->W);
    }

    return PC;
}

//Executes one instruction
int CpuExec(PIC_CPU *Cpu)
{
    short PC;

    //Get the PC
    PC = CpuGetPC(Cpu);

    //Execute an opcode
    PC = CpuExecuteOpcode(Cpu, CpuGetOpcode(Cpu, PC), PC);
    if (PC == 0xFFFF)
    {
        printf("Opcode execution failed\n");
        return -1;
    }
    
    //Set the new PC
    printf("PC -> 0x%x\n", PC);
    CpuSetPC(Cpu, PC);

    //1 instruction retired :)
    return 0;
}