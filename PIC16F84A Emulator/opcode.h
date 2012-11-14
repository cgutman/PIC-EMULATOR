//
//  opcode.h
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/13/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#ifndef PIC16F84A_Emulator_opcode_h
#define PIC16F84A_Emulator_opcode_h

typedef struct _PIC_OPCODE {
    unsigned short Opcode:14;
} PIC_OPCODE;

#define OP_ADDWF    0x700
#define OP_ANDWF    0x500
#define OP_CLRF     0x180
#define OP_CLRW     0x100
#define OP_COMF     0x900
#define OP_DECF     0x300
#define OP_DECFSZ   0xB00
#define OP_INCF     0xA00
#define OP_INCFSZ   0xF00
#define OP_IORWF    0x400
#define OP_MOVF     0x800
#define OP_MOVWF    0x080
#define OP_RLF      0xD00
#define OP_RRF      0xC00
#define OP_SUBWF    0x200
#define OP_SWAPF    0xE00
#define OP_XORWF    0x600

#define OP_BCF      0x1000
#define OP_BSF      0x1400
#define OP_BTFSC    0x1800
#define OP_BTFSS    0x1C00

#define OP_GOTO     0x2800
#define OP_CALL     0x2000

#define OP_CLRWDT   0x64
#define OP_RETFIE   0x09
#define OP_RETURN   0x08
#define OP_SLEEP    0x63

#define OP_ADDLW    0x3E00
#define OP_ANDLW    0x3900
#define OP_IORLW    0x3800
#define OP_MOVLW    0x3000
#define OP_RETLW    0x3400
#define OP_SUBLW    0x3C00
#define OP_XORLW    0x3A00

void OpPrintOpcode(unsigned short opcode);
unsigned short OpGenerateOpcode(char *opname, char operand1, char operand2);

#endif
