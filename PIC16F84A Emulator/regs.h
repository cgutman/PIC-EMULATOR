//
//  regs.h
//  PIC16F84A Emulator
//
//  Created by Cameron Gutman on 11/13/12.
//  Copyright (c) 2012 Cameron Gutman. All rights reserved.
//

#ifndef PIC16F84A_Emulator_regs_h
#define PIC16F84A_Emulator_regs_h

//68 general purpose registers
#define GPR_COUNT  68

//Undefined value constant
#define UNDEFINED_VAL   0xFF

//The PIC has one 8-bit working register
typedef unsigned char WORKING_REGISTER;

//This struct represents PIC register file
typedef struct _REGISTER_FILE {
    // ------ Bank 0 - 0x00 ------

    //Indirect mapping placeholder
    unsigned char Unused0;

    //TMR0 is undefined at init
#define RESET_TMR0        UNDEFINED_VAL
    unsigned char TMR0;      //8-bit RTC counter
    
    //PC is set to 0 at init
#define RESET_PCL         0x00
    unsigned char PCL;       //Low-order 8-bits of the PC
    
    //STATUS at 0001 1xxx - TO and PD set at init
#define RESET_STATUS      0x18
    unsigned char STATUS;
    
    //FSR is undefined at init
#define RESET_FSR         UNDEFINED_VAL
    unsigned char FSR;       //Indirect addressing pointer

    //PORTA is partially undefined, partially zero at init
#define RESET_PORTA       (UNDEFINED_VAL & 0x1F)
    unsigned char PORTA;

    //PORTB is undefined at init
#define RESET_PORTB       UNDEFINED_VAL
    unsigned char PORTB;

    //Unused register is unused ;)
    unsigned char Unused1;   //Unused register (read as 0)
    
    //EEDATA is undefined at init
#define RESET_EEDATA      UNDEFINED_VAL
    unsigned char EEDATA;    //EEPROM Data Register

    //EEADR is undefined at init
#define RESET_EEADR       UNDEFINED_VAL
    unsigned char EEADR;     //EEPROM Address Register

    //PCLATH is 0 at init
#define RESET_PCLATH      UNDEFINED_VAL
    unsigned char PCLATH;    //Write buffer for upper 5-bits of PC

    //INTCON is partially 0, partially undefined at init
#define RESET_INTCON      (UNDEFINED_VAL & 0x1)
    unsigned char INTCON;    //See notes below (INTCON)

    //SRAM is undefined at init
    unsigned char SRAM[GPR_COUNT];

    //Unused registers between bank 0 and bank 1
    unsigned char Unused2[47];

    // ------ Bank 1 - 0x80 ------

    //Indirect addressing placeholder
    unsigned char Unused3;

    //Options register is 0xFF at init
#define RESET_OPTION_REG   0xFF
    unsigned char OPTION_REG;

    //Bank 0 mappings for PCL, STATUS, and FSR
    unsigned char Unused4[3];

    //Unused register location 0x87 (corresponds to 0x07)
    unsigned char Unused5;

    //EECON1 is partially undefined, partially 0 at init
#define RESET_EECON1       (UNDEFINED_VAL & 0x8)
    unsigned char EECON1;

    //EECON2 is not a physical register TODO
    unsigned char EECON2;
} REGISTER_FILE;

/* ------- STATUS Register Bit Definitions (03h) ------- */

//Reserved bits (maintain as 0)
#define STATUS_IRP_RSRV    (1 << 7)
#define STATUS_RP1_RSRV    (1 << 6)

//Bank select bit
// 01 = Bank 1 (80h-FFh)
// 00 = Bank 0 (00h-7Fh)
#define STATUS_RP0         (1 << 5)

//Timeout bit
// 01 = After power-up, CLRWDT, or SLEEP retires
// 00 = A WDT time-out occurred
#define STATUS_TO          (1 << 4)

//Power-down bit
// 01 = After powerup or CLRWDT
// 00 = After SLEEP
#define STATUS_PD          (1 << 3)

//Zero bit
// 01 = The result of an operation is 0
// 00 = The result of an operation not 0
#define STATUS_Z           (1 << 2)

//Digit carry/borrow bit
// 01 = A carry-out from the 4th low order bit of the result occurred
// 00 = No carry-out of the 4th low order bit occurred
#define STATUS_DC          (1 << 1)

//Carry/borrow bit
// 01 = A carry-out from the MSB occurred
// 00 - No carry-out from the MSB occurred
#define STATUS_C           (1 << 0)


/* --------- Function definitions for regs.c -------- */
void RegsInitializeRegisterFile(REGISTER_FILE *Regs);

void RegsSetValue(REGISTER_FILE *Regs, unsigned char RegFileAddr, unsigned char Value);

char RegsGetValue(REGISTER_FILE *Regs, unsigned char RegFileAddr);


#endif
