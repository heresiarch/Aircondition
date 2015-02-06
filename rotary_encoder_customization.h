/* 
 * File:   rotary_encoder_customization.h
 * Author: rene
 *
 * Created on 24. Mai 2014, 09:57
 */

#ifndef ROTARY_ENCODER_CUSTOMIZATION_H
#define	ROTARY_ENCODER_CUSTOMIZATION_H
#include <xc.h>
#define FCY 8000000UL
#include <libpic30.h>
//Rotary Encoder PIN Definitions
#define PHASE_B  PORTAbits.RA2
#define PHASE_A  PORTAbits.RA3
#define KNOB_PIN PORTBbits.RB4

#endif	/* ROTARY_ENCODER_CUSTOMIZATION_H */

