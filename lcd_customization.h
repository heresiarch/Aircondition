/* 
 * File:   lcd_customization.h
 * Author: rene
 *
 * Created on 18. August 2013, 14:55
 */

#ifndef LCD_CUSTOMIZATION_H
#define	LCD_CUSTOMIZATION_H
#define FCY 8000000UL
// LCD clock pin definitions
#define LCD_CLOCK_BIT LATBbits.LATB14 // Bit
// LCD data pin definitions
#define LCD_DATA_BIT LATBbits.LATB15 // Bit
// Macros for clock- and data signals
#define LCD_SET_CLK() {LCD_CLOCK_BIT = 1;}
#define LCD_CLR_CLK() {LCD_CLOCK_BIT = 0;}
#define LCD_SET_DATA() {LCD_DATA_BIT = 1;}
#define LCD_CLR_DATA() {LCD_DATA_BIT = 0;}
#endif	/* LCD_CUSTOMIZATION_H */

