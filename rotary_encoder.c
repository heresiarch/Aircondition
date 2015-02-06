#include "rotary_encoder.h"
#include "rotary_encoder_customization.h"

volatile int8_t enc_delta;          // -128 ... 127
static int8_t last;
uint8_t y_old = 0;
uint8_t flag = 0;
uint16_t  long_counter = 0;
knob_state knob ={0};


void encode_init( void )
{
  int8_t new;
  new = 0;
  if( PHASE_A )
    new = 3;
  if( PHASE_B )
    new ^= 1;                   // convert gray to binary
  last = new;                   // power on state
  enc_delta = 0;
  knob.knob_up = 0;
  knob.long_pressed = 0;
}

void rotarydecode(void)
{
  int8_t new, diff;
  new = 0;
  if( PHASE_A )
    new = 3;
  if( PHASE_B )
    new ^= 1;                   // convert gray to binary
  diff = last - new;                // difference last - new
  if( diff & 1 ){               // bit 0 = value (1)
    last = new;                 // store new as next last
    enc_delta += (diff & 2) - 1;        // bit 1 = direction (+/-)
  }
}


int8_t encode_read1( void )         // read single step encoders
{
  int8_t val;
  val = enc_delta;
  enc_delta = 0;
  return val;                   // counts since last call
}

int8_t encode_read2( void )         // read two step encoders
{
  int8_t val;
  val = enc_delta;
  enc_delta = val & 1;
  return val >> 1;
}

int8_t encode_read4( void )         // read four step encoders
{
  int8_t val;
  val = enc_delta;
  enc_delta = val & 3;
  return val >> 2;
}

void debounce_switch(void)
{
	uint8_t temp = 0;
	temp=(y_old>>2);
	y_old=y_old-temp;
	if(KNOB_PIN == 0)
	{
		y_old=y_old+0x3F;
	}
	if((y_old > 0xF0)&&(flag==0))
	{
		flag=1;
		knob.long_pressed = 0;
		long_counter = 0;
	}
	if((y_old < 0x0F)&&(flag==1))
	{
		if(knob.long_pressed== 0)
		{
			knob.knob_up = 1;
		}
		flag=0;
		knob.long_pressed = 0;
		long_counter = 0;
	}
	if( flag == 1)
	{
		long_counter++;
	}
	else
	{
		long_counter = 0;
	}

	if(long_counter > 1024)
	{
		knob.long_pressed = 1;
	}
}

knob_state* get_knob_state()
{
    return &knob;
}

uint8_t knob_up(void)
{
    uint8_t ret = knob.knob_up;
    //clear state
    knob.knob_up = 0;
    return ret;
}

uint8_t long_pressed(void)
{
    uint8_t ret = 0;
    if(knob.long_pressed > 0)
    {
        knob.long_pressed = 0;
        ret = 1;
    }
    return ret;
}




/*
int main( void )
{
  int32_t val = 0;

  LEDS_DDR = 0xFF;
  encode_init();
  sei();

  for(;;){
    val += encode_read1();          // read a single step encoder
    LEDS = val;
  }
}*/