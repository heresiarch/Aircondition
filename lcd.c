
#include "lcd.h"
#include <xc.h>
#include "lcd_customization.h"
#include <libpic30.h>
#include "stringconversion.h"


// User specific definitions
// Definitions for LC display
#define LCD_POWER_UP_DELAY_MS 50 // Power up delay in ms
#define LCD_INIT_DELAY_MS 10 // Initialization steps delay in ms
#define LCD_COLS 16 // Display columns
#define LCD_ROWS 2 // Display rows
// Definitions for display modes and functions
#define LCD_CMD_MODE  0
#define LCD_DATA_MODE 1
#define LCD_INIT_MODE 2
#define LCD_CLR 1
#define LCD_HOME 2
#define LCD_DISP_ON 0x0C
#define LCD_FUNCTION_4BIT_1LINE 0x20
#define LCD_FUNCTION_4BIT_2LINES 0x28
#define LCD_FUNCTION_8BIT_1LINE 0x30
#define LCD_FUNCTION_8BIT_2LINES 0x38



void lcd_delay(void) // Do clock depending delay
{
  __delay_us(100);
}
//-----------------------------------------------------------------------------

char LCD_ROW_TABLE[4] = // Display row addresses
{
  0x80,
  0xC0,
  0x80 + LCD_COLS,
  0xC0 + LCD_COLS
};

void lcd_init_shift_reg(uint8_t mode) // Clear shift-register, set E and RS
{
  uint8_t n;

  LCD_CLR_DATA(); // Clear data signal
  lcd_delay();

  // Clear all outputs of shift register
  for (n = 7; n; n--)
  {
    LCD_SET_CLK();
    lcd_delay();
    LCD_CLR_CLK();
  }

  // Set high level for E at Q7
  LCD_SET_DATA();
  lcd_delay();
  LCD_SET_CLK();
  lcd_delay();
  LCD_CLR_CLK();

  // Set level for RS at Q6
  if (mode == LCD_DATA_MODE)
  {
    LCD_SET_DATA();
  }
  else
  {
    LCD_CLR_DATA();
  }

  lcd_delay();
  LCD_SET_CLK();
  lcd_delay();
  LCD_CLR_CLK();
}
//-----------------------------------------------------------------------------

void lcd_shift_nibble(uint8_t data) // Clock in 4 bits of data
{
  uint8_t n, mask = 0x08;

  for (n = 4; n; n--) // Do this for 4 databits
  {
    if (data & mask) // Set or reset data signal
    {
      LCD_SET_DATA();
    }
    else
    {
      LCD_CLR_DATA();
    }

    LCD_SET_CLK();
    lcd_delay();
    LCD_CLR_CLK();

    mask >>= 1; // Shift right compare mask
  }

  // Clear data line, then do the last shift, Q7=1
  LCD_CLR_DATA(); // Always clear data line before the last shift

  LCD_SET_CLK();
  lcd_delay();
  LCD_CLR_CLK();
}
//-----------------------------------------------------------------------------

void lcd_shift_byte(uint8_t data,uint8_t mode) // Write command or data byte to display
{
  lcd_init_shift_reg(mode); // Clear shift register, set RS and E
  lcd_shift_nibble(data >> 4); // Shift high nibble first

  // Now 6 bits are shifted, Q7=1 waiting for the data line to go high
  // for a logical AND, thus a high at E to latch the data at Q5..Q2
  LCD_SET_DATA();
  lcd_delay();

  // Do not write the low nibble if in LCD_INIT_MODE because
  // the interface is in 8-bit mode at startup
  if (mode != LCD_INIT_MODE)
  {
    lcd_init_shift_reg(mode); // Clear shift register, set RS and E
    lcd_shift_nibble(data); // Shift low nibble last

    // Now 6 bits are shifted, Q7=1 waiting for the data line to go high
    // for a logical AND, thus a high at E to latch the data at Q5..Q2
    LCD_SET_DATA();
    lcd_delay();
  }

  LCD_CLR_DATA(); // Clear E always
  lcd_delay();
}
//-----------------------------------------------------------------------------

void lcd_write_command(uint8_t command) // Write command byte to display
{
  lcd_shift_byte(command,LCD_CMD_MODE); // Do serial byte transfer
}
//-----------------------------------------------------------------------------

void lcd_write_data(uint8_t data) // Write data byte to display
{
  lcd_shift_byte(data,LCD_DATA_MODE); // Do serial byte transfer
}
//-----------------------------------------------------------------------------

// Function flags - Bit0 0: 5x7 Dots, 1: 5x10 Dots
//                  Bit1 0: 1 Row, 1: 2 Rows
//                  Bit2 0: 4-Bit interface, 1: 8-Bit interface

void lcd_function_mode(uint8_t function_mode) // Set display funktion mode
{
  function_mode = ((function_mode & 7) << 2) + 0x20;
  lcd_write_command(function_mode);
}
//-----------------------------------------------------------------------------

// Display flags - Bit0 0: blink cursor off, 1: blink cursor on
//                 Bit1 0: line cursor off, 1: line cursor on
//                 Bit2 0: display off, 1: display on

void lcd_display_mode(uint8_t display_mode) // Set display mode
{
  display_mode = (display_mode & 7) + 0x08;
  lcd_write_command(display_mode);
}
//-----------------------------------------------------------------------------

// Entry flags - Bit0 0: display shift off, 1: display shift on
//               Bit1 0: decrement, 1: increment

void lcd_entry_mode(uint8_t entry_mode) // Set entry mode
{
  entry_mode = (entry_mode & 3) + 0x04;
  lcd_write_command(entry_mode);
}
//-----------------------------------------------------------------------------

void lcd_clear(void) // Clear display
{
  lcd_write_command(LCD_CLR);
  __delay_ms(4);
}
//-----------------------------------------------------------------------------

void lcd_home(void) // Set cursor to home position
{
  lcd_write_command(LCD_HOME);
}
//-----------------------------------------------------------------------------

void lcd_goto_xy(uint8_t xpos,uint8_t ypos) // Set display cursor position
{
  if (xpos < LCD_COLS && ypos < LCD_ROWS)
  {
    lcd_write_command(LCD_ROW_TABLE[ypos] + xpos);
  }
}
//-----------------------------------------------------------------------------

void lcd_write_char(char chr) // Write character
{
  lcd_write_data(chr);
}
//-----------------------------------------------------------------------------

void lcd_write_char_xy(uint8_t x,uint8_t y,char chr) // Write character at position
{
  lcd_goto_xy(x,y);
  lcd_write_data(chr);
}
//-----------------------------------------------------------------------------

void lcd_write_str(char* sp) // Write string
{
  while (*sp)
  {
    lcd_write_data(*sp++);
  }
}
//-----------------------------------------------------------------------------

void lcd_write_str_xy(uint8_t x,uint8_t y,char* sp) // Write string at position
{
  lcd_goto_xy(x,y);
  lcd_write_str(sp);
}
//-----------------------------------------------------------------------------

void lcd_init(void) // Initialize display
{
  // Set clock- and data pin to low level
  LCD_CLR_CLK();
  LCD_CLR_DATA();

  __delay_ms(LCD_POWER_UP_DELAY_MS); // Delay for power up time

  // Start Initialization
  // Only the high nibble ist transfered in LCD_INIT_MODE
  lcd_shift_byte(LCD_FUNCTION_8BIT_1LINE,LCD_INIT_MODE);
  __delay_ms(LCD_INIT_DELAY_MS);
  lcd_shift_byte(LCD_FUNCTION_8BIT_1LINE,LCD_INIT_MODE);
  __delay_ms(LCD_INIT_DELAY_MS);
  lcd_shift_byte(LCD_FUNCTION_8BIT_1LINE,LCD_INIT_MODE);
  __delay_ms(LCD_INIT_DELAY_MS);
  lcd_shift_byte(LCD_FUNCTION_4BIT_1LINE,LCD_INIT_MODE);
  __delay_ms(LCD_INIT_DELAY_MS);

  // Display is in 4 bit I/O mode now
#if (LCD_ROWS > 1)
  lcd_write_command(LCD_FUNCTION_4BIT_2LINES);
#else
  lcd_write_command(LCD_FUNCTION_4BIT_1LINE);
#endif
  __delay_ms(LCD_INIT_DELAY_MS);
  lcd_write_command(LCD_DISP_ON);
  __delay_ms(LCD_INIT_DELAY_MS);
  lcd_write_command(LCD_CLR);
  lcd_write_command(LCD_HOME);
  __delay_ms(LCD_INIT_DELAY_MS);
}


void lcd_write_U8(char value,uint8_t digits) // Write unsigned 8-bit number
{
  lcd_write_str(sc_S08_to_str(value,digits));
}
//-----------------------------------------------------------------------------

void lcd_write_U8_hex(char value) // Write hexdecimal 8-bit number
{
  lcd_write_str(sc_U8_to_hex(value));
}
//-----------------------------------------------------------------------------

void lcd_write_U16(uint16_t value,uint8_t digits) // Write unsigned 16-bit number
{
  lcd_write_str(sc_U16_to_str(value,digits));
}
//-----------------------------------------------------------------------------

void lcd_write_int_hex(uint16_t value) // Write hexdecimal 16-bit number
{
  lcd_write_str(sc_U16_to_hex(value));
}
//-----------------------------------------------------------------------------
