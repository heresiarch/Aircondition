/* 
 * File:   lcd.h
 * Author: rene
 *
 * Created on 18. August 2013, 14:50
 */

#ifndef LCD_H
#define	LCD_H

#include <stdint.h>
void lcd_init(void); // Initialize display
void lcd_clear(void); // Clear display
void lcd_home(void); // Set cursor to home position
void lcd_goto_xy(uint8_t xpos,uint8_t ypos); // Set display cursor position
void lcd_write_char(char chr); // Write character
void lcd_write_char_xy(uint8_t x, uint8_t y, char chr); // Write character at position
void lcd_write_str(char* sp); // Write string
void lcd_write_str_xy(uint8_t x,uint8_t y, char* sp); // Write string at position
void lcd_write_U8(char value,uint8_t digits); // Write unsigned 8-bit number
void lcd_write_U8_hex(char value); // Write hexdecimal 8-bit number
void lcd_write_U16(uint16_t value,uint8_t digits); // Write unsigned 16-bit number
void lcd_write_U16_hex(uint16_t value); // Write hexdecimal 16-bit number
#endif	/* LCD_H */

