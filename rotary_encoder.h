/* 
 * File:   rotary_encoder.h
 * Author: rene
 *
 * Created on 24. Mai 2014, 09:27
 */

#ifndef ROTARY_ENCODER_H
#define	ROTARY_ENCODER_H
#include <stdint.h>

// init
void encode_init(void);

// called in ISR every 1 ms
void rotarydecode(void);             // 1ms for manual movement

// read single step encoders
int8_t encode_read1(void);

// read two step encoders
int8_t encode_read2(void);

// read four step encoders
int8_t encode_read4(void);

// called in ISR every 1 ms
void debounce_switch(void);

typedef struct knob_state{
    uint8_t knob_up;
    uint8_t long_pressed;
} knob_state;

knob_state* get_knob_state();
uint8_t knob_up(void);
uint8_t long_pressed(void);

#endif	/* ROTARY_ENCODER_H */

