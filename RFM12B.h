/* 
 * File:   RFM12B.h
 * Author: rene
 *
 * Created on 24. August 2013, 23:18
 */

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
/// Original Code was written by Severin Birrer       ///
/// More informations at www.blucorazon.ch            ///
///                                                   ///
/////////////////////////////////////////////////////////

#ifndef RFM12B_H
#define	RFM12B_H
#include <stdint.h>

void RFM12B_PORT_INIT(void);
/// RFM12B_CMD                                        ///
/// Function: Sends command and receives data         ///
uint8_t RFM12B_CMD(uint16_t cmd);
/// RFM12B_WAIT_READY                                 ///
/// Function: Wait for RFM12B to be ready (FFIT)      ///
void RFM12B_WAIT_READY(void);
/// RFM12B_READ                                       ///
/// Function: Wait and read a byte from FIFO          ///
uint8_t RFM12B_READ(void);
/// RFM12B_WRITE                                      ///
/// Function: Write a byte                            ///
void RFM12B_WRITE(uint8_t abyte);
/// RFM12B_INIT                                       ///
/// Function: Initalise as a sender or a receiver     ///
void RFM12B_INIT(uint8_t type);
/// RFM12B_SEND                                       ///
/// Function: Send multiple bytes                     ///
void RFM12B_SEND(uint8_t *ptr, uint16_t size);
/// RFM12B_RECEIVE                                    ///
/// Function: Receive multiple bytes                  ///
void RFM12B_RECEIVE(uint8_t *ptr, uint16_t size);

#endif	/* RFM12B_H */

