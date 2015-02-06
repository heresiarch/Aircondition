/******* G E N E R I C   D E F I N I T I O N S ************************************************/

#include "1wire_customization.h"
#include "1wire.h"

#define	HIGH	1
#define	LOW	0
#define	OUTPUT	0
#define	INPUT 	1
#define	SET	1
#define	CLEAR	0


#define DIR_OUT 0
#define DIR_IN 1


/**********************************************************************
* Function:        void drive_OW_low (void)
* PreCondition:    None
* Input:		   None
* Output:		   None
* Overview:		   Configure the OW_PIN as Output and drive the OW_PIN LOW.
***********************************************************************/
void drive_OW_low (void)
{
	OW_PIN_DIRECTION = OUTPUT;
	OW_WRITE_PIN=LOW;
}

/**********************************************************************
* Function:        void drive_OW_high (void)
* PreCondition:    None
* Input:		   None
* Output:		   None
* Overview:		   Configure the OW_PIN as Output and drive the OW_PIN HIGH.
***********************************************************************/
void drive_OW_high (void)
{
	OW_PIN_DIRECTION = OUTPUT;
	OW_WRITE_PIN = HIGH;
}

/**********************************************************************
* Function:        unsigned char read_OW (void)
* PreCondition:    None
* Input:		   None
* Output:		   Return the status of OW pin.
* Overview:		   Configure as Input pin and Read the status of OW_PIN
***********************************************************************/
unsigned char read_OW (void)
{
	uint8_t read_data=0;

	OW_WRITE_PIN = INPUT;

	 if (HIGH == OW_READ_PIN)
	 	read_data = SET;
	 else
		read_data = CLEAR;

	return read_data;
}

/**********************************************************************
* Function:        unsigned char OW_reset_pulse(void)
* PreCondition:    None
* Input:		   None
* Output:		   Return the Presense Pulse from the slave.
* Overview:		   Initialization sequence start with reset pulse.
*				   This code generates reset sequence as per the protocol
***********************************************************************/
unsigned char OW_reset_pulse(void)

{
	uint8_t presence_detect;

  	drive_OW_low(); 				// Drive the bus low

 	__delay_us(240);	  			// delay 480 microsecond (us)
	__delay_us(240);

 	drive_OW_high ();  				// Release the bus

	__delay_us(70);				// delay 70 microsecond (us)

	presence_detect = read_OW();	//Sample for presence pulse from slave

 	__delay_us(205);	  			// delay 410 microsecond (us)
	__delay_us(205);

	drive_OW_high ();		    	// Release the bus

	return presence_detect;
}

/**********************************************************************
* Function:        void OW_write_bit (unsigned char write_data)
* PreCondition:    None
* Input:		   Write a bit to 1-wire slave device.
* Output:		   None
* Overview:		   This function used to transmit a single bit to slave device.
*
***********************************************************************/

void OW_write_bit (uint8_t write_bit)
{
	if (write_bit)
	{
		//writing a bit '1'
		drive_OW_low(); 				// Drive the bus low
		__delay_us(6);				// delay 6 microsecond (us)
		drive_OW_high ();  				// Release the bus
		__delay_us(64);				// delay 64 microsecond (us)
	}
	else
	{
		//writing a bit '0'
		drive_OW_low(); 				// Drive the bus low
		__delay_us(60);				// delay 60 microsecond (us)
		drive_OW_high ();  				// Release the bus
		__delay_us(10);				// delay 10 microsecond for recovery (us)
	}
}


/**********************************************************************
* Function:        unsigned char OW_read_bit (void)
* PreCondition:    None
* Input:		   None
* Output:		   Return the status of the OW PIN
* Overview:		   This function used to read a single bit from the slave device.
*
***********************************************************************/

unsigned char OW_read_bit (void)
{
	uint8_t read_data;
	//reading a bit
	drive_OW_low(); 						// Drive the bus low
	__delay_us(6);						// delay 6 microsecond (us)
	drive_OW_high ();  						// Release the bus
	__delay_us(9);						// delay 9 microsecond (us)

	read_data = read_OW();					//Read the status of OW_PIN

	__delay_us(55);						// delay 55 microsecond (us)
	return read_data;
}

/**********************************************************************
* Function:        void OW_write_byte (unsigned char write_data)
* PreCondition:    None
* Input:		   Send byte to 1-wire slave device
* Output:		   None
* Overview:		   This function used to transmit a complete byte to slave device.
*
***********************************************************************/
void OW_write_byte (uint8_t write_data)
{
	uint8_t loop;

	for (loop = 0; loop < 8; loop++)
	{
		OW_write_bit(write_data & 0x01); 	//Sending LS-bit first
		write_data >>= 1;					// shift the data byte for the next bit to send
	}
}

/**********************************************************************
* Function:        unsigned char OW_read_byte (void)
* PreCondition:    None
* Input:		   None
* Output:		   Return the read byte from slave device
* Overview:		   This function used to read a complete byte from the slave device.
*
***********************************************************************/

unsigned char OW_read_byte (void)
{
	uint8_t loop, result=0;

	for (loop = 0; loop < 8; loop++)
	{

		result >>= 1; 				// shift the result to get it ready for the next bit to receive
		if (OW_read_bit())
		result |= 0x80;				// if result is one, then set MS-bit
	}
	return result;
}

void reset_ow(void)
{
   OW_TEMP_TRIS=DIR_OUT;
   OW_TEMP_SIG=0;
   __delay_us(250);
   __delay_us(250);
   OW_TEMP_TRIS=DIR_IN;
   OW_TEMP_SIG=1;
   __delay_us(250);
   __delay_us(250);
}

void write_ow(uint8_t b)
{
   unsigned char i;

   OW_TEMP_SIG=1;
   OW_TEMP_TRIS=DIR_OUT;
   for ( i=0;i<8;i++)
   {
      OW_TEMP_SIG=0;
      if ( b & 0x01 )
      {
         __delay_us(10);
         OW_TEMP_SIG=1;
      }
      __delay_us(70);
      OW_TEMP_SIG=1;
      __delay_us(10);
      b >>= 1;
   }
   OW_TEMP_TRIS=DIR_IN;
   OW_TEMP_SIG=1;
}

uint8_t read_ow(void)
{
   uint8_t b=0;
   uint8_t m;
   uint8_t i;

   m=1;
   for ( i=0;i<8;i++)
   {
      OW_TEMP_SIG=1;
      OW_TEMP_TRIS=DIR_OUT;
      OW_TEMP_SIG=0;
      __delay_us(8/2);
      OW_TEMP_TRIS=DIR_IN;
      OW_TEMP_SIG=1;
      __delay_us(15/2);

      if ( 1 == OW_TEMP_SIG_IN )
      {
         b |= m;
      }
      m <<=1;
      __delay_us(60);
   }
   OW_TEMP_TRIS=DIR_IN;
   OW_TEMP_SIG=1;
   return b;
}


void write_ds2430(uint8_t adr, uint8_t *buff, uint8_t num_vals)
// write num_vals in array buff beginning at address adr
{
   uint8_t n;
   reset_ow();
   write_ow(0xcc);	// skip ROM
   write_ow(0xf0);	// read memory into scratch pad

   reset_ow();
   write_ow(0xcc);	// skip ROM
   write_ow(0x0f);	// write scratch pad

   write_ow(adr);	// starting address
   for(n=0; n<num_vals; n++)
   {
      write_ow(buff[n]);
   }

   reset_ow();
   write_ow(0xcc);	// skip ROM
   write_ow(0x55);	// copy scratch pad
   write_ow(0xa5); 	// validation key

   drive_OW_high();	// while programming
   __delay_ms(250);
   read_ow();
}

void read_ds2430(uint8_t adr, uint8_t *buff, uint8_t num_vals)
// reads num_vals bytes into array buff beginning at address adr
{
   uint8_t n;
   reset_ow();
   write_ow(0xcc);	// skip ROM
   write_ow(0xf0);	// read memory into scratch pad

   write_ow(adr);

   for(n=0; n<num_vals; n++)
   {
      buff[n]=read_ow();
   }
}

/*
 char molch[] =  "DEADBEAF";
            char molch2[]=  "        ";
            //write_ds2430(6, molch, 8);
            // sensor 0, adr 6, array d, 8 bytes
            read_ds2430(6, molch2, 8);
            // sensor 0, adr 6, array buff, 8 bytes
            lcd_clear();
            lcd_home();
            lcd_write_str_xy(0,0,molch2);
            __delay_ms(1000);
*/


/********************************************************************************************
                  E N D     O F     1 W I R E . C
*********************************************************************************************/