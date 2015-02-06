/* 
 * File:   LCD_I2C_ST7032i.h
 * Author: rene
 *
 * Created on 23. Juli 2014, 14:17
 */

#ifndef LCD_I2C_ST7032I_H
#define	LCD_I2C_ST7032I_H
#include <stdint.h>

#define LCD_C2L		0x10		// Move Cursor to the left
#define LCD_C2R		0x14		// Move Cursor to the right
#define LCD_D2L		0x18		// Move display to the left
#define LCD_D2R		0x1C		// Move display to the right

// init
void lcd2_init(void); // Initialize display
//navigation
void lcd2_clear(void); // Clear display
void lcd2_wait_and_clear(uint16_t ms); // waits some time in ms and clears display
void lcd2_home(void); // Set cursor to home position
void lcd2_goto_xy(uint8_t xpos,uint8_t ypos); // Set display cursor position
void lcd2_move(uint8_t dir); // Move Cursor or display
// writing
void lcd2_write_char(char chr); // Write character
void lcd2_write_char_xy(uint8_t x, uint8_t y, char chr); // Write character at position
void lcd2_write_str(char* sp); // Write string
void lcd2_write_str_xy(uint8_t x,uint8_t y, char* sp); // Write string at position
void lcd2_write_U8(uint8_t value,uint8_t digits); // Write unsigned 8-bit number
void lcd2_write_U8_hex(uint8_t value); // Write hexdecimal 8-bit number
void lcd2_write_U16(uint16_t value,uint8_t digits); // Write unsigned 16-bit number
void lcd2_write_U16_hex(uint16_t value); // Write hexdecimal 16-bit number
void lcd2_tile(uint8_t tData); // tile LCD Screen with one char
//CGRAM functions
void lcd2_fill_CGRAM(void);// fill CGRAM with array CGRAM[]
void lcd2_show_CGRAM(void);// show all patterns in CGRAM
void lcd2_call_builtin_char(void);// call built-in characters
// blink and cursor control
void lcd2_cursor(void);
void lcd2_no_cursor(void);
void lcd2_blink(void);
void lcd2_no_blink(void);
#endif	/* LCD_I2C_ST7032I_H */

