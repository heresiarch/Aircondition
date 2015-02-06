#include "i2c.h"
#include <xc.h>
#include "i2c_customization.h"
#include <libpic30.h>


#define Slave 0x7C //3E for CiZ (0x7C shifted over 1 bit)  //3C for BiZ (0x78 shifted over 1 bit)
#define Comsend  0x00
#define Datasend  0x40
#define Line2  0xC0



//internal for ic2_poll
char bit_in(void);

void i2c_start()
{
	if (SDA_dir == 1) SDA_dir = 0;
	if (SCL_dir == 1) SCL_dir = 0;
	SDA = 1;	//nach Reset ist der Bus Low, er muss aber High sein!!!
	SCL = 1;	//nach Reset ist der Bus Low, er muss aber High sein!!!
	SDA = 0;
	__delay_us(5);
	SCL = 0;
	__delay_us(5);
}

void i2c_stop()
{
	SDA = 0;
	__delay_us(5);
	SCL = 1;
	__delay_us(5);
	SDA = 1;
	__delay_us(5);
}

void i2c_send(uint8_t b)
{
	uint8_t mask;
	mask = 0x80;
	do
	{
		if(b & mask)
                    SDA = 1;
		else
                    SDA = 0;
		SCL = 1;
		__delay_us(5);
		SCL = 0;
		__delay_us(5);
		mask = mask/2;
	}
	while(mask > 0);
	SDA = 1;
	SCL = 1;
	__delay_us(5);
	SCL = 0;
	__delay_us(5);
}

unsigned char i2c_read(uint8_t ack)
{
	uint8_t mask,daten;
	daten = 0;
	mask = 0x80;
	SDA = 1;
	do
	{
		SDA_dir = 0;		// Port wird von Ausgang auf Eingang umgeschalten
		SCL = 1;
		__delay_us(5);
		if(SDA_Read ==1)daten = daten | mask;
		SCL = 0;
		__delay_us(5);
		mask = mask/2;
		SDA_dir = 1;		// Port wird wieder Ausgang
	}
	while(mask > 0);

	__delay_us(5);
	SDA = ack;
	__delay_us(5);
	SCL = 1;
	__delay_us(5);
	SCL = 0;
	__delay_us(5);
	return daten;
}

uint8_t i2c_poll(char addr)
{
   uint8_t ack = 0;
   uint8_t mask;
   i2c_start();
   mask = 0x80;
    do
    {
	if(addr & mask) SDA = 1;
	else SDA = 0;
	SCL = 1;
	__delay_us(5);
	SCL = 0;
	__delay_us(5);
	mask = mask/2;
    }
   while(mask > 0);
   ack = bit_in();
   i2c_stop();
   return ack;
}

char bit_in(void)
{
    char ret = 0;
    SCL = 0;                        // ensure SCL is low
    SDA_dir = 1;                   // configure SDA as an input
    SCL = 1;                        // bring SCL high to begin transfer
    int i = 0;
    while(i<10000 && ret==0)
    {
        ret = SDA_Read;
        i++;
        __delay_us(1);        
    }
    SCL = 0;                        // bring SCL low again.
    SDA_dir = 0;                    // configure SDA as output again
    return ret;
}


void Show(char *text)
{
	int n, d;
	d=0x00;
	i2c_start();
	i2c_send(Slave);
	i2c_send(Datasend);
	for(n=0;n<16;n++){
		i2c_send(*text);
		++text;
		}
	i2c_stop();
}

void nextline(void)
{
	i2c_start();
	i2c_send(Slave);
	i2c_send(Comsend);
	i2c_send(Line2);
	i2c_stop();
}
void CGRAM (void)
{
 	i2c_start();
	i2c_send(Slave);
	i2c_send(Comsend);
	i2c_send(0x38);		//go to instructino table 0
	i2c_send(0x40);		//Set CGRAM address to 0x00
	i2c_stop();
	__delay_ms(10);

	i2c_start();
	i2c_send(Slave);
	i2c_send(Datasend);
	i2c_send(0x00);		//write to first CGRAM address
	i2c_send(0x1E);
	i2c_send(0x18);
	i2c_send(0x14);
	i2c_send(0x12);
	i2c_send(0x01);
	i2c_send(0x00);
	i2c_send(0x00);		//8 bytes per character
	//continue writing to remaining CGRAM if desired
	i2c_stop();
}
/****************************************************
*           Initialization For ST7032i              *
*****************************************************/
void init_LCD()
{
    //i2c_poll(Slave);
      i2c_start();
      i2c_send(Slave);
      i2c_send(Comsend);
      i2c_send(0x38);
      __delay_ms(1);
      i2c_send(0x39);
      __delay_ms(1);
      i2c_send(0x14);   // Internalosc
      i2c_send(0x70);   
      i2c_send(0x5E);
      i2c_send(0x6D);
      i2c_send(0x0C);   // Displayon
      i2c_send(0x01);   // Clear
      i2c_send(0x06);   // Entrymode

      __delay_ms(1);
      i2c_stop();



 /*
 i2c_start();
i2c_send(Slave);
i2c_send(Comsend);
i2c_send(0x38);
__delay_ms(10);
i2c_send(0x39);
__delay_ms(10);
i2c_send(0x14);
i2c_send(0x78);
i2c_send(0x5E);
i2c_send(0x6D);
i2c_send(0x0C);
i2c_send(0x01);
i2c_send(0x06);
__delay_ms(10);
i2c_stop();

CGRAM();			//define CGRAM

i2c_start();
i2c_send(Slave);
i2c_send(Comsend);
i2c_send(0x39);
i2c_send(0x01);		//go back Home
i2c_stop();
__delay_ms(10);*/
}
