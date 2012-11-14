//
//  regs.c
//  PIC16F84A Emulator
//
//  Licensed under GPLv3
//
//  Cameron Gutman (cameron.gutman@case.edu)
//

#include <stdio.h>

#include "regs.h"

void RegsPrintRegisterName(unsigned char RegFileAddr)
{
    switch (RegFileAddr)
    {
        case REG_TMR0:
            printf("TMR0");
            return;
        case REG_OPTION_REG:
            printf("OPTION_REG");
            return;
        case REG_PCL:
        case REG_PCL | 0x80:
            printf("PCL");
            return;
        case REG_STATUS:
        case REG_STATUS | 0x80:
            printf("STATUS");
            return;
        case REG_FSR:
        case REG_FSR | 0x80:
            printf("FSR");
            return;
        case REG_PORTA:
            printf("PORTA");
            return;
        case REG_TRISA:
            printf("TRISA");
            return;
        case REG_PORTB:
            printf("PORTB");
            return;
        case REG_TRISB:
            printf("TRISB");
            return;
        //0x07 and 0x87 are unimplemented on PIC
        case REG_EEDATA:
            printf("EEDATA");
            return;
        case REG_EECON1:
            printf("EECON1");
            return;
        case REG_EEADR:
            printf("EEADR");
            return;
        case 0x89:
            //Not a real register
            return;
        case REG_PCLATH:
        case REG_PCLATH | 0x80:
            printf("PCLATH");
            return;
        case REG_INTCON:
        case REG_INTCON | 0x80:
            printf("INTCON");
            return;
    }
    
    //Check if this is an unimplemented location
    if (((RegFileAddr & 0x7F) >= 0x50) ||
        ((RegFileAddr & 0x7F) == 0x07))
    {
        printf("Unimplemented");
    }
    //Check if it's a GPR
    else if ((RegFileAddr & 0x7F) >= 0x0C)
    {
        //Mask the MSB
        RegFileAddr &= 0x7F;
        
        printf("GPR[0x%x]", (RegFileAddr - 0x0C));
    }
}

void RegsPrintStatusRegister(unsigned char StatusVal)
{
    if (StatusVal & STATUS_RP0)
        printf(" RP0");
    if (StatusVal & STATUS_TO)
        printf(" ~TO");
    if (StatusVal & STATUS_PD)
        printf(" ~PD");
    if (StatusVal & STATUS_Z)
        printf(" Z");
    if (StatusVal & STATUS_DC)
        printf(" DC");
    if (StatusVal & STATUS_C)
        printf(" C");
}

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
    Regs->TRISA = RESET_TRISA;
    Regs->TRISB = RESET_TRISB;
    Regs->EECON1 = RESET_EECON1;
}

void RegsSetValue(REGISTER_FILE *Regs, unsigned char RegFileAddr, unsigned char Value)
{
    unsigned char WriteMask;
    unsigned char *Destination;

    //Check if we're doing an indirect address resolution
    if ((RegFileAddr & 0x7F) == 0)
    {
        //Resolve it using the address in the FSR register
        RegFileAddr = Regs->FSR;
    }

    //Set the MSB if we're on bank 1
    if ((Regs->STATUS & STATUS_RP0) != 0)
    {
        RegFileAddr |= 0x80;
    }
    else
    {
        RegFileAddr &= ~0x80;
    }

    //We handle SPRs here
    Destination = NULL;
    switch (RegFileAddr)
    {
        case REG_TMR0:
            printf("TMR0");
            WriteMask = WRITE_MASK_TMR0;
            Destination = &Regs->TMR0;
            break;
        case REG_OPTION_REG:
            printf("OPTION_REG");
            WriteMask = WRITE_MASK_OPTION_REG;
            Destination = &Regs->OPTION_REG;
            break;
        case REG_PCL:
        case REG_PCL | 0x80:
            printf("PCL");
            WriteMask = WRITE_MASK_PCL;
            Destination = &Regs->PCL;
            break;
        case REG_STATUS:
        case REG_STATUS | 0x80:
            printf("STATUS");
            WriteMask = WRITE_MASK_STATUS;
            Destination = &Regs->STATUS;
            break;
        case REG_FSR:
        case REG_FSR | 0x80:
            printf("FSR");
            WriteMask = WRITE_MASK_FSR;
            Destination = &Regs->FSR;
            break;
        case REG_PORTA:
            printf("PORTA");
            WriteMask = WRITE_MASK_PORTA;
            Destination = &Regs->PORTA;
            break;
        case REG_TRISA:
            printf("TRISA");
            WriteMask = WRITE_MASK_TRISA;
            Destination = &Regs->TRISA;
            break;
        case REG_PORTB:
            printf("PORTB");
            WriteMask = WRITE_MASK_PORTB;
            Destination = &Regs->PORTB;
            break;
        case REG_TRISB:
            printf("TRISB");
            WriteMask = WRITE_MASK_TRISB;
            Destination = &Regs->TRISB;
            break;
        //0x07 and 0x87 are unimplemented on PIC
        case REG_EEDATA:
            printf("EEDATA");
            WriteMask = WRITE_MASK_EEDATA;
            Destination = &Regs->EEDATA;
            break;
        case REG_EECON1:
            printf("EECON1");
            WriteMask = WRITE_MASK_EECON1;
            Destination = &Regs->EECON1;
            break;
        case REG_EEADR:
            printf("EEADR");
            WriteMask = WRITE_MASK_EEADR;
            Destination = &Regs->EEADR;
            break;
        case 0x89:
            //EECON2 - Not a physical register
            return;
        case REG_PCLATH:
        case REG_PCLATH | 0x80:
            printf("PCLATH");
            WriteMask = WRITE_MASK_PCLATH;
            Destination = &Regs->PCLATH;
            break;
        case REG_INTCON:
        case REG_INTCON | 0x80:
            printf("INTCON");
            WriteMask = WRITE_MASK_INTCON;
            Destination = &Regs->INTCON;
            break;
    }
    
    //Check if we'll be writing a SPR
    if (Destination != NULL)
    {
        //Print some debugging stuff
        if ((RegFileAddr & 0x7F) == REG_STATUS)
        {
            printf(": [");
            RegsPrintStatusRegister(*Destination);
            printf("] -> [");
            RegsPrintStatusRegister(Value & WriteMask);
            printf("]\n");
        }
        else
        {
            printf(": 0x%x -> 0x%x\n", *Destination, (Value & WriteMask));
        }

        *Destination &= ~WriteMask;
        *Destination |= (Value & WriteMask);
    }
    else
    {
        //Check if this is a write on unimplemented locations
        if (((RegFileAddr & 0x7F) >= 0x50) ||
            ((RegFileAddr & 0x7F) == 0x07))
        {
            //No-op
            return;
        }
        //Check if it's a GPR write
        else if ((RegFileAddr & 0x7F) >= 0x0C)
        {
            //Mask the MSB before write
            RegFileAddr &= 0x7F;
        }

        //Print some debugging output
        RegsPrintRegisterName(RegFileAddr);
        printf(": %d -> %d\n", ((unsigned char *)Regs)[RegFileAddr], Value);
        
        //Do a simple memory write
        ((unsigned char *)Regs)[RegFileAddr] = Value;
    }
}

char RegsGetValue(REGISTER_FILE *Regs, unsigned char RegFileAddr)
{
    unsigned char ReadMask;
    unsigned char *Source;
    
    //Check if we're doing an indirect address resolution
    if ((RegFileAddr & 0x7F) == 0)
    {
        //Resolve it using the address in the FSR register
        RegFileAddr = Regs->FSR;
    }
    
    //Set the MSB if we're on bank 1
    if ((Regs->STATUS & STATUS_RP0) != 0)
    {
        RegFileAddr |= 0x80;
    }
    else
    {
        RegFileAddr &= ~0x80;
    }
    
    //We handle SPRs here
    Source = NULL;
    switch (RegFileAddr)
    {
        case REG_TMR0:
            //TMR0
            ReadMask = READ_MASK_TMR0;
            Source = &Regs->TMR0;
            break;
        case REG_OPTION_REG:
            //OPTION_REG
            ReadMask = READ_MASK_OPTION_REG;
            Source = &Regs->OPTION_REG;
            break;
        case REG_PCL:
        case REG_PCL | 0x80:
            //PCL
            ReadMask = READ_MASK_PCL;
            Source = &Regs->PCL;
            break;
        case REG_STATUS:
        case REG_STATUS | 0x80:
            //STATUS
            ReadMask = READ_MASK_STATUS;
            Source = &Regs->STATUS;
            break;
        case REG_FSR:
        case REG_FSR | 0x80:
            //FSR
            ReadMask = READ_MASK_FSR;
            Source = &Regs->FSR;
            break;
        case REG_PORTA:
            //PORTA
            ReadMask = READ_MASK_PORTA;
            Source = &Regs->PORTA;
            break;
        case REG_TRISA:
            //TRISA
            ReadMask = READ_MASK_TRISA;
            Source = &Regs->TRISA;
            break;
        case REG_PORTB:
            //PORTB
            ReadMask = READ_MASK_PORTB;
            Source = &Regs->PORTB;
            break;
        case REG_TRISB:
            //TRISB
            ReadMask = READ_MASK_TRISB;
            Source = &Regs->TRISB;
            break;
        //0x07 and 0x87 are unimplemented on PIC
        case REG_EEDATA:
            //EEDATA
            ReadMask = READ_MASK_EEDATA;
            Source = &Regs->EEDATA;
            break;
        case REG_EECON1:
            //EECON1
            ReadMask = READ_MASK_EECON1;
            Source = &Regs->EECON1;
            break;
        case REG_EEADR:
            //EEADR
            ReadMask = READ_MASK_EEADR;
            Source = &Regs->EEADR;
            break;
        case 0x89:
            //EECON2 - Not a physical register
            return 0;
        case REG_PCLATH:
        case REG_PCLATH | 0x80:
            //PCLATH
            ReadMask = READ_MASK_PCLATH;
            Source = &Regs->PCLATH;
            break;
        case REG_INTCON:
        case REG_INTCON | 0x80:
            //INTCON
            ReadMask = READ_MASK_INTCON;
            Source = &Regs->INTCON;
            break;
    }
    
    //Check if we'll be reading a SPR
    if (Source != NULL)
    {
        return (*Source & ReadMask);
    }
    else
    {
        //Check if this is a write on unimplemented locations
        if (((RegFileAddr & 0x7F) >= 0x50) ||
            ((RegFileAddr & 0x7F) == 0x07))
        {
            //Unimplemented registers read as 0
            return 0;
        }
        //Check if it's a GPR
        else if ((RegFileAddr & 0x7F) >= 0x0C)
        {
            //Mask the MSB before reading
            RegFileAddr &= 0x7F;
        }
        
        //Do a simple memory read
        return ((unsigned char *)Regs)[RegFileAddr];
    }
}
