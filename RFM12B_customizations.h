/* 
 * File:   RFM12B_customizations.h
 * Author: rene
 *
 * Created on 24. August 2013, 23:26
 */

#ifndef RFM12B_CUSTOMIZATIONS_H
#define	RFM12B_CUSTOMIZATIONS_H

#define FCY 8000000UL
// RFM12B pin definitions and Macros
#define MOSI      LATBbits.LATB9
#define MOSI_DIR  TRISBbits.TRISB9
#define MISO      LATBbits.LATB8
#define MISO_DIR  TRISBbits.TRISB8
#define in_MISO   PORTBbits.RB8
#define nSEL      LATBbits.LATB7
#define nSEL_DIR  TRISBbits.TRISB7
#define SCK       LATBbits.LATB10
#define SCK_DIR   TRISBbits.TRISB10

//#define in_IRQ()  input(IRQ)

#endif	/* RFM12B_CUSTOMIZATIONS_H */

