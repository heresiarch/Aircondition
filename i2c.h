#ifndef I2C_H
#define	I2C_H
#include <stdint.h>
void i2c_start(void);
void i2c_stop(void);
void i2c_send(uint8_t b);
unsigned char i2c_read(uint8_t ack); // lesen und ack senden oder nicht... 0 oder 1

void init_LCD();
void nextline(void);
void CGRAM (void);
void Show(char* text);


void testme(void);

#endif	/* I2C_H */

