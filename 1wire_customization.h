/* 
 * File:   1wire_customization.h
 * Author: rene
 *
 * Created on 15. September 2013, 09:54
 */

#ifndef ONE_WIRE_CUSTOMIZATION_H
#define	ONE_WIRE_CUSTOMIZATION_H
#include <xc.h>
#define FCY 8000000UL
#include <libpic30.h>

//ONE WIRE PORT PIN DEFINITION
///****************************************************
// This Configuration is required to make any PIC MicroController
// I/O pin as Open drain to drive 1-wire.
///****************************************************
#define OW_PIN_DIRECTION 	LATBbits.LATB5
#define OW_WRITE_PIN  		TRISBbits.TRISB5
#define OW_READ_PIN		PORTBbits.RB5

#define OW_TEMP_SIG LATBbits.LATB5
#define OW_TEMP_TRIS TRISBbits.TRISB5
#define OW_TEMP_SIG_IN PORTBbits.RB5



#endif	/* 1WIRE_CUSTOMIZATION_H */

