/////////////////////////////////////////////////////////
///                                                   ///
///      ####    #####  #    #    #   ##   ####       ///
///      #   #   #      ##  ##   ##  #  #  #   #      ///
///      ####    ####   # ## #  # #    #   ####       ///
///      # #     #      #    #    #   #    #   #      ///
///      #  #    #      #    #    #  #     #   #      ///
///      #   #   #      #    #    #  ####  ####       ///
///                                                   ///
///      RFM12B Driver                                ///
///                                                   ///
/////////////////////////////////////////////////////////
///                                                   ///
/// Functions:                                        ///
///                                                   ///
/// RFM12B_INIT(int type)                             ///
/// RFM12B_SEND(int buffer, int size)                 ///
/// RFM12B_RECEIVE(int buffer, int size)              ///
///                                                   ///
/////////////////////////////////////////////////////////
///                                                   ///
/// Updates:                                          ///
///                                                   ///
/// 09.01.2010 New Driver                             ///
///                                                   ///
/////////////////////////////////////////////////////////
///                                                   ///
/// original Code was written by Severin Birrer       ///
/// More informations at www.blucorazon.ch            ///
///                                                   ///
/////////////////////////////////////////////////////////


#include "RFM12B.h"
#include <xc.h>
#include "RFM12B_customizations.h"
#include <libpic30.h>

/////////////////////////////////////////////////////////
///                                                   ///
/// Definitions                                       ///
///                                                   ///
/////////////////////////////////////////////////////////

#define TX        1
#define RX        0

#define MOSI_hi() {MOSI = 1;}
#define MOSI_lo() {MOSI = 0;}
#define nSEL_hi() {nSEL = 1;}
#define nSEL_lo() {nSEL = 0;}
#define SCK_hi()  {SCK = 1;}
#define SCK_lo()  {SCK = 0;}


/////////////////////////////////////////////////////////
///                                                   ///
/// RFM12B_PORT_INIT                                  ///
/// Function: Initalise ports to defined states       ///
///                                                   ///
/////////////////////////////////////////////////////////
void RFM12B_PORT_INIT(void)
{
    MOSI_DIR = 0;
    nSEL_DIR = 0;
    SCK_DIR = 0;
    MISO_DIR = 1;

   nSEL_hi();
   SCK_lo();
   MOSI_lo();
   __delay_ms(100);
}

/////////////////////////////////////////////////////////
///                                                   ///
/// RFM12B_CMD                                        ///
/// Function: Sends command and receives data         ///
///                                                   ///
/////////////////////////////////////////////////////////
uint8_t RFM12B_CMD(uint16_t cmd)
{
   uint8_t i;
   uint16_t temp=0;

   SCK_lo();
   nSEL_lo();
   for(i=0;i<16;i++)
   {
      if(cmd&0x8000)
      {
         MOSI_hi();
      }
      else
      {
         MOSI_lo();
      }
      __delay_us(5);
      SCK_hi();
      __delay_us(5);
      temp<<=1;

      if(in_MISO == 1)
      {
         temp|=0x0001;
      }

      SCK_lo();
      cmd<<=1;
   }
   nSEL_hi();
   return(temp);
}

/////////////////////////////////////////////////////////
///                                                   ///
/// RFM12B_WAIT_READY                                 ///
/// Function: Wait for RFM12B to be ready (FFIT)      ///
///                                                   ///
/////////////////////////////////////////////////////////
void RFM12B_WAIT_READY(void)
{
   nSEL_lo();
   while(!in_MISO); // Wait for Statusbit (FFIT) ready
}

/////////////////////////////////////////////////////////
///                                                   ///
/// RFM12B_READ                                       ///
/// Function: Wait and read a byte from FIFO          ///
///                                                   ///
/////////////////////////////////////////////////////////
uint8_t RFM12B_READ(void)
{
   uint8_t data;

   RFM12B_WAIT_READY();
   data = RFM12B_CMD(0xB000); // Read Data

   return(data&0x00FF);
}

/////////////////////////////////////////////////////////
///                                                   ///
/// RFM12B_WRITE                                      ///
/// Function: Write a byte                            ///
///                                                   ///
/////////////////////////////////////////////////////////
void RFM12B_WRITE(uint8_t abyte)
{
   RFM12B_WAIT_READY();
   RFM12B_CMD(0xB800 + abyte); // Write Data
}

/////////////////////////////////////////////////////////
///                                                   ///
/// RFM12B_INIT                                       ///
/// Function: Initalise as a sender or a receiver     ///
///                                                   ///
/////////////////////////////////////////////////////////
void RFM12B_INIT(uint8_t type)
{
   RFM12B_PORT_INIT();

   if (type)
   {
      // Init Sender
      RFM12B_CMD(0x80A7); // Configuration Command
   }
   else
   {
      // Init Receive
      RFM12B_CMD(0x80E8); // Configuration Command
   }
   // General Init
   RFM12B_CMD(0x8208); // Power Management Command
   RFM12B_CMD(0xA67C); // Frequency Command
   RFM12B_CMD(0xC613); // Datarate Command
   RFM12B_CMD(0x94A0); // Receiver Control Command
   RFM12B_CMD(0xC26A); // Datafilter Command
   //RFM12B_CMD(0xCA12); // FIFO / Reset Mode Command
   RFM12B_CMD(0xCA81);
   RFM12B_CMD(0xCED4); // Synchron Pattern Command
   RFM12B_CMD(0xC4F9); // AFC Command
   RFM12B_CMD(0x9872); // TX Configuration Command
   RFM12B_CMD(0xCC77); // PLL Command
   RFM12B_CMD(0xE000); // Wake-Up Timer Command
   RFM12B_CMD(0xC800); // Low Duty-Cycle Command
   RFM12B_CMD(0xC040); // LB Det./Clock Div. Command
}

/////////////////////////////////////////////////////////
///                                                   ///
/// RFM12B_SEND                                       ///
/// Function: Send multiple bytes                     ///
///                                                   ///
/////////////////////////////////////////////////////////
void RFM12B_SEND(uint8_t *ptr, uint16_t size)
{
   uint16_t i;

   RFM12B_CMD(0x8238); // Enable Sender

   RFM12B_WRITE(0xAA); // Preamble
   RFM12B_WRITE(0xAA); // Preamble
   RFM12B_WRITE(0xAA); // Preamble

   RFM12B_WRITE(0x2D); // Sync HighByte
   RFM12B_WRITE(0xD4); // Sync LowByte

   for (i=0;i<size;i++)
   {
      RFM12B_WRITE(*ptr); // DATA Byte
      ptr++;
   }

   RFM12B_WRITE(0xAA); // Dummy Byte
   RFM12B_WRITE(0xAA); // Dummy Byte
   RFM12B_WRITE(0xAA); // Dummy Byte

   RFM12B_WAIT_READY();
   RFM12B_CMD(0x8208); // Disable Sender
}

/////////////////////////////////////////////////////////
///                                                   ///
/// RFM12B_RECEIVE                                    ///
/// Function: Receive multiple bytes                  ///
///                                                   ///
/////////////////////////////////////////////////////////
void RFM12B_RECEIVE(uint8_t *ptr, uint16_t size)
{
   uint16_t i;

   RFM12B_CMD(0x82C8); // Init Receiver
   RFM12B_CMD(0xCA81); // Enable FIFO
   RFM12B_CMD(0xCA83); // Clear FIFO

   for (i=0;i<size;i++)
   {
      *ptr = RFM12B_READ(); // Send Data
      ptr++;
   }

   RFM12B_CMD(0x8208); // Disable Receiver
}
