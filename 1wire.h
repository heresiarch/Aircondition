/* 
 * File:   1wire.h
 * Author: rene
 *
 * Created on 15. September 2013, 09:51
 */

#ifndef ONE_WIRE_H
#define	ONE_WIRE_H
#include <stdint.h>

/** P R O T O T Y P E S ******************************************************/
void drive_one_wire_low (void);
void drive_one_wire_high (void);
uint8_t read__one_wire (void);
void OW_write_bit (uint8_t write_data);
uint8_t OW_read_bit (void);
uint8_t OW_reset_pulse(void);
void OW_write_byte (uint8_t write_data);
uint8_t OW_read_byte (void);

// write num_vals in array buff beginning at address adr
void write_ds2430(uint8_t adr, uint8_t *buff, uint8_t num_vals);
// reads num_vals bytes into array buff beginning at address adr
void read_ds2430(uint8_t adr, uint8_t *buff, uint8_t num_vals);

#endif	/* ONE_WIRE_H */

