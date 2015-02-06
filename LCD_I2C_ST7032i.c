#include <xc.h>
#include "LCD_I2C_ST7032i.h"
#include "LCD_I2C_ST7032i_customization.h"
#include <libpic30.h>
#include "i2c.h"
#include "stringconversion.h"


#define LCD_CLEAR       0x01		// Clear Display
#define LCD_HOME	0x02		// LCD Return home
#define LCD_ENTRY 	0x06		// Set LCD Entry Mode


#define LCD_ON		0x0E		// Turn on LCD and Cursor
#define LCD_OFF		0x08		// Turn off LCD

#define LCD_CGRAM_ADDR	0x40	// Start address of LCD CGRAM
#define LCD_CGMAX 	64			// Max CGRAM bytes


#define iDat	1
#define iCmd	0

// flags for display on/off control
#define LCD_DISPLAYCONTROL			0x08		// Command (HD44780)			Page 22
#define LCD_DISPLAYON 				0x04		// Setting (HD44780)*
#define LCD_DISPLAYOFF 				0x00		// Setting (HD44780)
#define LCD_CURSORON 				0x02		// Setting (HD44780)
#define LCD_CURSOROFF 				0x00		// Setting (HD44780)*
#define LCD_BLINKON 				0x01		// Setting (HD44780)
#define LCD_BLINKOFF 				0x00		// Setting (HD44780)*


uint8_t _displaycontrol = LCD_DISPLAYON;


uint8_t charRAM[LCD_CGMAX] ={
    // Make eight patterns of 8*5 font
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 1.Dsiplay All
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 2.White Board
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, // 3.Virtical 1
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, // 4.Virtical 2
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, // 5.Horizon 1
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, // 6.Horizon 2
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // 7.Stars
    0xFF, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0xFF, // 8.Rectangle
};



// Functions for ST7032I

// Write a string to LCD

void SendByte(uint8_t DatCmd, uint8_t dByte)
{
    i2c_start();
    i2c_send(LCD_I2C_ADDR);
    if (DatCmd == iDat)
    {
        i2c_send(0x40);
    } else
    {
        i2c_send(0x00);
    }
    i2c_send(dByte);
    i2c_stop();
    //DelayUs(100); // Special Delay for Character LCD
}

// Fill CGRAM with array CGRAM[]

void lcd2_fill_CGRAM(void)
{
    uint8_t i;
    SendByte(iCmd, 0x38);
    __delay_us(30);
    SendByte(iCmd, LCD_CGRAM_ADDR);
    for (i = 0; i < LCD_CGMAX; i++)
    {
        SendByte(iDat, charRAM[i]);
    }
}

// Show All patterns in CGRAM

void lcd2_show_CGRAM(void)
{
    uint8_t i, k;

    for (i = 0; i < 8; i++)
    {
        SendByte(iCmd, LCD_OFF);
        SendByte(iCmd, LCD_L1);
        for (k = 0; k < LCD_CHAR; k++)
        {
            switch (k) {
#if defined(LCD_L2)
                case LCD_COL:
                    SendByte(iCmd, LCD_L2);
                    break;
#endif
#if defined(LCD_L3)
                case LCD_COL * 2:
                    SendByt//
    e(iCmd, LCD_L3);
                    break;
#endif
#if defined(LCD_L4)
                case LCD_COL * 3:
                    SendByte(iCmd, LCD_L4);
                    break;
#endif
                default:
                    break;
            }
            SendByte(iDat, i);
        }
        SendByte(iCmd, LCD_ON);
        lcd2_wait_and_clear(1500);
    }
}
// Call built-in Characters

void lcd2_call_builtin_char(void)
{
    uint8_t i, k;
    for (i = 0; i < LCD_COL; i += LCD_ROW)
    {
        SendByte(iCmd, LCD_L1);
        for (k = 0; k < LCD_CHAR; k++)
        {
            #if defined(LCD_L2)
            switch (k) {
                case LCD_COL:
                    SendByte(iCmd, LCD_L2);
                    break;
            #if defined(LCD_L3)
                case LCD_COL * 2:
                    SendByte(iCmd, LCD_L3);
                    break;
            #endif
            #if defined(LCD_L4)
                case LCD_COL * 3:
                    SendByte(iCmd, LCD_L4);
                    break;
            #endif
                default:
                    break;
            }
            #endif
            SendByte(iDat, k + LCD_COL * i);
        }
        lcd2_wait_and_clear(1500);
    }
}

void lcd2_init(void)
{
    // fix scrap controller rest problem
    i2c_start();
    i2c_stop();
    __delay_ms(50);


    SendByte(iCmd, 0x38);
    __delay_us(30);
    SendByte(iCmd, 0x39); // Function Set
    __delay_us(30);
    SendByte(iCmd, 0x14); // Internal OSC Frequency
    __delay_us(30);
    SendByte(iCmd, 0x70); // Contrast Adjustment (Lower 4 bits)
    __delay_us(30);
    SendByte(iCmd, 0x5E); // Power/Icon/Contrast Control
    // (Icon off/Booster On/C5C4=11)
    __delay_us(30);
    SendByte(iCmd, 0x6D); // Follower control
    __delay_ms(200);
    SendByte(iCmd, 0x0C);
    __delay_us(50);
    lcd2_clear();
    SendByte(iCmd, LCD_ENTRY); // Entry Mode Set
}

void lcd2_clear(void)
{
    SendByte(iCmd, LCD_CLEAR);
    __delay_ms(5);
    // 2ms delay is Necessary after sending LCD_CLS command !!!
}

void lcd2_wait_and_clear(uint16_t ms)
{
    __delay_ms(ms);
    lcd2_clear();
}

void lcd2_home(void)
{
    SendByte(iCmd, LCD_HOME);
    __delay_ms(5);
}

void lcd2_goto_xy(uint8_t x,uint8_t y)
{
     switch (y) {
        #if defined(LCD_L2)
        case 1:
            SendByte(iCmd, LCD_L2 + x);
            break;
        #endif
        #if defined(LCD_L3)
        case 2:
            SendByte(iCmd, LCD_L3 +x);
            break;
        #endif
        #if defined(LCD_L4)
        case 3:
            SendByte(iCmd, LCD_L4 + x);
            break;
        #endif
        case 0:
        default:
            SendByte(iCmd, LCD_L1 +x);
    }
}

void lcd2_write_char(char chr)
{
    SendByte(iDat, chr);
}

void lcd2_write_char_xy(uint8_t x, uint8_t y, char chr)
{
    lcd2_goto_xy(x,y);
    lcd2_write_char(chr);
}

void lcd2_write_str(char* sp)
{
    while ((*sp) != '\0')
    {
        SendByte(iDat, *sp++);
    }
}

void lcd2_write_str_xy(uint8_t x,uint8_t y, char* sp)
{
    lcd2_goto_xy(x,y);
    lcd2_write_str(sp);
}

void lcd2_write_U8(uint8_t value,uint8_t digits) // Write unsigned 8-bit number
{
  lcd2_write_str(sc_S08_to_str(value,digits));
}
//-----------------------------------------------------------------------------

void lcd2_write_U8_hex(uint8_t value) // Write hexdecimal 8-bit number
{
  lcd2_write_str(sc_U8_to_hex(value));
}
//-----------------------------------------------------------------------------

void lcd2_write_U16(uint16_t value,uint8_t digits) // Write unsigned 16-bit number
{
  lcd2_write_str(sc_U16_to_str(value,digits));
}
//-----------------------------------------------------------------------------

void lcd2_write_int_hex(uint16_t value) // Write hexdecimal 16-bit number
{
  lcd2_write_str(sc_U16_to_hex(value));
}

void lcd2_move(uint8_t dir)
{
      SendByte(iCmd, dir);
}

// Tile LCD Screen with one data
void lcd2_tile(uint8_t tData)
{
    uint8_t k;
    SendByte(iCmd, LCD_L1);
    for (k = 0; k < LCD_CHAR; k++)
    {
        switch (k) {
#if defined(LCD_L2)
            case LCD_COL:
                SendByte(iCmd, LCD_L2);
                break;
#endif
#if defined(LCD_L3)
            case LCD_COL * 2:
                SendByte(iCmd, LCD_L3);
                break;
#endif
#if defined(LCD_L4)
            case LCD_COL * 3:
                SendByte(iCmd, LCD_L4);
                break;
#endif
            default:
                break;
        }
        SendByte(iDat, tData);
    }
}

// Turns the underline cursor on/off
void lcd2_cursor(void) {
  _displaycontrol |= LCD_CURSORON;
  SendByte(iCmd,LCD_DISPLAYCONTROL | _displaycontrol);
  __delay_ms(30);
}

void lcd2_no_cursor(void) {
  _displaycontrol &= ~LCD_CURSORON;
  SendByte(iCmd,LCD_DISPLAYCONTROL | _displaycontrol);
  __delay_ms(30);
}

// Turn cursor blinking on/off
void lcd2_blink(void) {
  _displaycontrol |= LCD_BLINKON;
  SendByte(iCmd,LCD_DISPLAYCONTROL | _displaycontrol);
  __delay_ms(30);
}

void lcd2_no_blink(void) {
  _displaycontrol &= ~LCD_BLINKON;
  SendByte(iCmd,LCD_DISPLAYCONTROL | _displaycontrol);
  __delay_ms(30);
}

