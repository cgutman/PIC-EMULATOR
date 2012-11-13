//
//  regs.c
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/13/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#include <stdio.h>

#include "regs.h"

void RegsInitializeRegisterFile(REGISTER_FILE *Regs)
{
    //Bank 0 and shared registers
    Regs->TMR0 = RESET_TMR0;
    Regs->PCL = RESET_PCL;
    Regs->STATUS = RESET_STATUS;
    Regs->FSR = RESET_FSR;
    Regs->PORTA = RESET_PORTA;
    Regs->PORTB = RESET_PORTB;
    Regs->EEDATA = RESET_EEDATA;
    Regs->EEADR = RESET_EEADR;
    Regs->PCLATH = RESET_PCLATH;
    Regs->INTCON = RESET_INTCON;
    
    //Bank 1 registers
    Regs->OPTION_REG = RESET_OPTION_REG;
    Regs->EECON1 = RESET_EECON1;
}

void RegsSetValue(REGISTER_FILE *Regs, unsigned char RegFileAddr, unsigned char Value)
{
    //Check if we're doing an indirect address resolution
    if ((RegFileAddr & 0x7F) == 0)
    {
        //Resolve it using the address in the FSR register
        RegFileAddr = Regs->FSR;
    }
    
    //We handle registers with multiple addresses here (except GPR)
    switch (RegFileAddr)
    {
            //PCL
        case 0x02:
        case 0x82:
            Regs->PCL = Value;
            return;
            
            //STATUS
        case 0x03:
        case 0x83:
            Regs->STATUS = Value;
            return;
            
            //FSR
        case 0x04:
        case 0x84:
            Regs->FSR = Value;
            return;
            
            //PCLATH
        case 0x0A:
        case 0x8A:
            Regs->PCLATH = Value;
            return;
            
            //INTCON
        case 0x0B:
        case 0x8B:
            Regs->INTCON = Value;
            return;
    }
    
    //Check if this is a write on unimplemented locations
    if ((RegFileAddr & 0x7F) > 0x50)
    {
        //No-op
        return;
    }
    //Check if it's a GPR write
    else if ((RegFileAddr & 0x7F) > 0x0C)
    {
        //Mask the MSB before write
        RegFileAddr &= 0x7F;
    }

    //Do a simple memory write
    ((unsigned char *)Regs)[RegFileAddr] = Value;
}

char RegsGetValue(REGISTER_FILE *Regs, unsigned char RegFileAddr)
{
    //Check if we're doing an indirect address resolution
    if ((RegFileAddr & 0x7F) == 0)
    {
        //Resolve it using the address in the FSR register
        RegFileAddr = Regs->FSR;
    }
    
    //We handle registers with multiple addresses here (except GPR)
    switch (RegFileAddr)
    {
            //PCL
        case 0x02:
        case 0x82:
            return Regs->PCL;
            
            //STATUS
        case 0x03:
        case 0x83:
            return Regs->STATUS;
            
            //FSR
        case 0x04:
        case 0x84:
            return Regs->FSR;
            
            //PCLATH
        case 0x0A:
        case 0x8A:
            return Regs->PCLATH;
            
            //INTCON
        case 0x0B:
        case 0x8B:
            return Regs->INTCON;
    }
    
    //Check if this is a read on unimplemented locations
    if ((RegFileAddr & 0x7F) > 0x50)
    {
        //It's always zero
        return 0;
    }
    //Check if it's a GPR read
    else if ((RegFileAddr & 0x7F) > 0x0C)
    {
        //Mask the MSB before reading
        RegFileAddr &= 0x7F;
    }
    
    //Do a simple memory read
    return ((unsigned char *)Regs)[RegFileAddr];
}
