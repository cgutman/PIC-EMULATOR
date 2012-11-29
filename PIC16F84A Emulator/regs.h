//
//  regs.h
//  PIC16F84A Emulator
//
//  Licensed under GPLv3
//
//  Cameron Gutman (cameron.gutman@case.edu)
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
#define REG_INDF          0x00
    unsigned char INDF;

    //TMR0 is undefined at init
#define REG_TMR0          0x01
#define RESET_TMR0        UNDEFINED_VAL
#define WRITE_MASK_TMR0   0xFF
#define READ_MASK_TMR0    0xFF
    unsigned char TMR0;      //8-bit RTC counter
    
    //PCL is set to 0 at init
#define REG_PCL           0x02
#define RESET_PCL         0x00
#define WRITE_MASK_PCL    0xFF
#define READ_MASK_PCL     0xFF
    unsigned char PCL;       //Low-order 8-bits of the PC
    
    //STATUS at 0001 1xxx - TO and PD set at init
#define REG_STATUS        0x03
#define RESET_STATUS      0x18
#define WRITE_MASK_STATUS 0x3F
#define READ_MASK_STATUS  0x3F
    unsigned char STATUS;
    
    //FSR is undefined at init
#define REG_FSR           0x04
#define RESET_FSR         UNDEFINED_VAL
#define WRITE_MASK_FSR    0xFF
#define READ_MASK_FSR     0xFF
    unsigned char FSR;       //Indirect addressing pointer

    //PORTA is partially undefined, partially zero at init
#define REG_PORTA         0x05
#define RESET_PORTA       (UNDEFINED_VAL & 0x1F)
#define WRITE_MASK_PORTA  0x1F
#define READ_MASK_PORTA   0x1F
    unsigned char PORTA;

    //PORTB is undefined at init
#define REG_PORTB         0x06
#define RESET_PORTB       UNDEFINED_VAL
#define WRITE_MASK_PORTB  0xFF
#define READ_MASK_PORTB   0xFF
    unsigned char PORTB;

    //Unused register is unused ;)
    unsigned char Unused1;   //Unused register (read as 0)
    
    //EEDATA is undefined at init
#define REG_EEDATA        0x08
#define RESET_EEDATA      UNDEFINED_VAL
#define WRITE_MASK_EEDATA 0xFF
#define READ_MASK_EEDATA  0xFF
    unsigned char EEDATA;    //EEPROM Data Register

    //EEADR is undefined at init
#define REG_EEADR         0x09
#define RESET_EEADR       UNDEFINED_VAL
#define WRITE_MASK_EEADR  0xFF
#define READ_MASK_EEADR   0xFF
    unsigned char EEADR;     //EEPROM Address Register

    //PCLATH is 0 at init
#define REG_PCLATH        0x0A
#define RESET_PCLATH       0x00
#define WRITE_MASK_PCLATH  0x1E
#define READ_MASK_PCLATH   0x00
    unsigned char PCLATH;    //Write buffer for upper 5-bits of PC

    //INTCON is partially 0, partially undefined at init
#define REG_INTCON        0x0B
#define RESET_INTCON      (UNDEFINED_VAL & 0x1)
#define WRITE_MASK_INTCON 0xFF
#define READ_MASK_INTCON  0xFF
    unsigned char INTCON;    //See notes below (INTCON)

    //SRAM is undefined at init
    unsigned char SRAM[GPR_COUNT];

    //Unused registers between bank 0 and bank 1
    unsigned char Unused2[48];

    // ------ Bank 1 - 0x80 ------

    //Indirect addressing placeholder
    unsigned char Unused3;

    //Options register is 0xFF at init
#define REG_OPTION_REG         0x81
#define RESET_OPTION_REG       0xFF
#define WRITE_MASK_OPTION_REG  0xFF
#define READ_MASK_OPTION_REG   0xFF
    unsigned char OPTION_REG;

    //Bank 0 mappings for PCL, STATUS, and FSR
    unsigned char Unused4[3];

    //TRISA is partially undefined, partially 1-filled at init
#define REG_TRISA         0x85
#define RESET_TRISA       0x1F
#define WRITE_MASK_TRISA  0x1F
#define READ_MASK_TRISA   0x1F
    unsigned char TRISA;

    //PORTB is 1-filled at init
#define REG_TRISB         0x86
#define RESET_TRISB       0xFF
#define WRITE_MASK_TRISB  0xFF
#define READ_MASK_TRISB   0xFF
    unsigned char TRISB;
    
    //Unused register location 0x87 (corresponds to 0x07)
    unsigned char Unused5;

    //EECON1 is partially undefined, partially 0 at init
#define REG_EECON1         0x88
#define RESET_EECON1       (UNDEFINED_VAL & 0x8)
#define WRITE_MASK_EECON1  0x1F
#define READ_MASK_EECON1   0x1F
    unsigned char EECON1;

    //EECON2 is not a physical register TODO
    unsigned char EECON2;
} REGISTER_FILE;

/* ------- STATUS Register Bit Definitions (03h) ------- */

//Reserved bits (maintain as 0)
#define STATUS_IRP_RSRV_BIT 7
#define STATUS_IRP_RSRV     (1 << STATUS_IRP_RSRV_BIT)
#define STATUS_RP1_RSRV_BIT 6
#define STATUS_RP1_RSRV     (1 << STATUS_RP1_RSRV_BIT)

//Bank select bit
// 01 = Bank 1 (80h-FFh)
// 00 = Bank 0 (00h-7Fh)
#define STATUS_RP0_BIT     5
#define STATUS_RP0         (1 << STATUS_RP0_BIT)

//Timeout bit
// 01 = After power-up, CLRWDT, or SLEEP retires
// 00 = A WDT time-out occurred
#define STATUS_TO_BIT      4
#define STATUS_TO          (1 << STATUS_TO_BIT)

//Power-down bit
// 01 = After powerup or CLRWDT
// 00 = After SLEEP
#define STATUS_PD_BIT      3
#define STATUS_PD          (1 << STATUS_PD_BIT)

//Zero bit
// 01 = The result of an operation is 0
// 00 = The result of an operation not 0
#define STATUS_Z_BIT       2
#define STATUS_Z           (1 << STATUS_Z_BIT)

//Digit carry/borrow bit
// 01 = A carry-out from the 4th low order bit of the result occurred
// 00 = No carry-out of the 4th low order bit occurred
#define STATUS_DC_BIT      1
#define STATUS_DC          (1 << STATUS_DC_BIT)

//Carry/borrow bit
// 01 = A carry-out from the MSB occurred
// 00 - No carry-out from the MSB occurred
#define STATUS_C_BIT       0
#define STATUS_C           (1 << STATUS_C_BIT)


/* --------- Function definitions for regs.c -------- */
void RegsInitializeRegisterFile(REGISTER_FILE *Regs);

void RegsSetValue(REGISTER_FILE *Regs, unsigned char RegFileAddr, unsigned char Value);

char RegsGetValue(REGISTER_FILE *Regs, unsigned char RegFileAddr);

void RegsPrintStatusRegister(unsigned char StatusVal);

#endif
