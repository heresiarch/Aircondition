/* 
 * File:   LCD_I2C_ST7032i_customization.h
 * Author: rene
 *
 * Created on 24. Juli 2014, 09:02
 */

#ifndef LCD_I2C_ST7032I_CUSTOMIZATION_H
#define	LCD_I2C_ST7032I_CUSTOMIZATION_H
#define FCY 8000000UL
#define LCD_COL 	16
#define LCD_ROW 	2
#define LCD_CHAR	LCD_COL*LCD_ROW

#define LCD_L1		0x80
#define LCD_L2		0xC0
//#define LCD_L3		0x90
//#define LCD_L4		0xD0

#define LCD_I2C_ADDR	0x7C	// Slave Address of the LCM: 0111 110(R/W)

#endif	/* LCD_I2C_ST7032I_CUSTOMIZATION_H */

