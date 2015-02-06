/* 
 * File:   main.c
 * Author: rene
 *
 * Created on 18. August 2013, 12:57
 */


#include <string.h>
#include <stdint.h>
#include <xc.h>
//#include <p24FJ32GB002.h>
#define FCY 8000000UL
#include <libpic30.h>
#include "LCD_I2C_ST7032i.h"
#include "RFM12B.h"
#include "TX21IT.h"
#include "LCD_I2C_ST7032i.h"
#include "1wire.h"
#include "rotary_encoder.h"
#include "stringconversion.h"
#include "aircondition.h"

/*
// http://www.microchip.com/forums/tm.aspx?m=491233
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx1 & FWDTEN_OFF & WINDIS_OFF & FWPSA_PR32 & WDTPS_PS8192);
_CONFIG2(IESO_OFF & FNOSC_FRCPLL & OSCIOFNC_ON & POSCMOD_NONE & PLL96MHZ_ON & PLLDIV_DIV2 & FCKSM_CSDCMD & IOL1WAY_OFF);
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_FST & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS3 & DSWDTOSC_LPRC & RTCOSC_LPRC & DSBOREN_OFF & DSWDTEN_OFF)
*/


// CONFIG4
#pragma config DSWDTPS = DSWDTPS3       // DSWDT Postscale Select (1:128 (132 ms))
#pragma config DSWDTOSC = LPRC          // Deep Sleep Watchdog Timer Oscillator Select (DSWDT uses Low Power RC Oscillator (LPRC))
#pragma config RTCOSC = LPRC            // RTCC Reference Oscillator Select (RTCC uses Low Power RC Oscillator (LPRC))
#pragma config DSBOREN = OFF            // Deep Sleep BOR Enable bit (BOR disabled in Deep Sleep)
#pragma config DSWDTEN = OFF            // Deep Sleep Watchdog Timer (DSWDT disabled)

// CONFIG3
#pragma config WPFP = WPFP0             // Write Protection Flash Page Segment Boundary (Page 0 (0x0))
#pragma config SOSCSEL = IO             // Secondary Oscillator Pin Mode Select (SOSC pins have digital I/O functions (RA4, RB4))
#pragma config WUTSEL = FST             // Voltage Regulator Wake-up Time Select (Fast regulator start-up time used)
#pragma config WPDIS = WPDIS            // Segment Write Protection Disable (Segmented code protection disabled)
#pragma config WPCFG = WPCFGDIS         // Write Protect Configuration Page Select (Last page and Flash Configuration words are unprotected)
#pragma config WPEND = WPENDMEM         // Segment Write Protection End Page Select (Write Protect from WPFP to the last page of memory)

// CONFIG2
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary Oscillator disabled)
#pragma config I2C1SEL = PRI            // I2C1 Pin Select bit (Use default SCL1/SDA1 pins for I2C1 )
#pragma config IOL1WAY = OFF            // IOLOCK One-Way Set Enable (The IOLOCK bit can be set and cleared using the unlock sequence)
#pragma config OSCIOFNC = ON            // OSCO Pin Configuration (OSCO pin functions as port I/O (RA3))
#pragma config FCKSM = CSDCMD           // Clock Switching and Fail-Safe Clock Monitor (Sw Disabled, Mon Disabled)
#pragma config FNOSC = FRCPLL           // Initial Oscillator Select (Fast RC Oscillator with Postscaler and PLL module (FRCPLL))
#pragma config PLL96MHZ = ON            // 96MHz PLL Startup Select (96 MHz PLL Startup is enabled automatically on start-up)
#pragma config PLLDIV = DIV2            // USB 96 MHz PLL Prescaler Select (Oscillator input divided by 2 (8 MHz input))
#pragma config IESO = OFF               // Internal External Switchover (IESO mode (Two-Speed Start-up) disabled)

// CONFIG1
#pragma config WDTPS = PS8192           // Watchdog Timer Postscaler (1:8,192)
#pragma config FWPSA = PR32             // WDT Prescaler (Prescaler ratio of 1:32)
#pragma config WINDIS = OFF             // Windowed WDT (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer (Watchdog Timer is disabled)
#pragma config ICS = PGx1               // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC1/PGED1)
#pragma config GWRP = OFF               // General Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)


#define OUTPUTBUFSIZE 16
char outbuf1[OUTPUTBUFSIZE];
char outbuf2[OUTPUTBUFSIZE];
#define RECEIVEBUFSIZE 5
char receiveBuff[RECEIVEBUFSIZE];

volatile uint8_t byteCount = 0;

#define RELAIS_OUT_PIN LATBbits.LATB13
#define RELAIS_OUT_DIR TRISBbits.TRISB13

#define GLOBAL_BUFFER_ELEMENTS 30
TX21SensorData global_buffer[GLOBAL_BUFFER_ELEMENTS];
volatile uint16_t global_counter = 0;


enum Status
{
	CHECK_CONFIG = 1, SET_CONFIG, RUN, ERROR
};

typedef enum Status StatusEnum;
StatusEnum s = CHECK_CONFIG;
StatusEnum old = ERROR;
volatile char dirty = 0;

AirConfigData configData = {0};

// functions for state machine
void state_check_config(void);
void state_set_config(void);
void state_run(void);

int findSensors(void);
uint8_t chooseSensorID(char* msg, TX21SensorData* outArray, uint8_t size);
int16_t chooseIntVal(char* msg, int16_t min, int16_t max, int16_t step);
float chooseFloatVal(char* msg, float min, float max, float step,float precision);


void enableRFM12(void);
void resetRFM12(void);

void Timer1Init(void);

int main(int argc, char** argv) {

    global_counter = 0;

    CLKDIVbits.PLLEN       = 1;     //PLL enable
    while(OSCCONbits.LOCK!=1) {};   //wait for stable PLL
    __delay_ms(5000);

    AD1PCFGL = 0xFFFF;      // all Ports digital
    TRISBbits.TRISB15 = 0;
    TRISBbits.TRISB14 = 0;
    TRISBbits.TRISB13 = 0;
    //RESET Line of RFM12b
    TRISBbits.TRISB11 = 1;
    //KNOB_PIN
    TRISBbits.TRISB4 = 1;
    //Rotary Phases
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA4 = 1;

    RPINR0	=	0x0B00;	//set RP11 to external interrupt 1
    INTCON2 = 0xFFFF;   /*Setup INT0, INT1, INT2, interupt on falling edge*/
    IFS1bits.INT1IF = 0;    /*Reset INT1 interrupt flag */
    IEC1bits.INT1IE = 1;    /*Enable INT1 Interrupt Service Routine */
    IPC5bits.INT1IP = 5;	/*set low priority*/

    //RELAIS
    RELAIS_OUT_DIR = 0;
    RELAIS_OUT_PIN = 0;
    Timer1Init();
    lcd2_init();
    encode_init();
    s = SET_CONFIG;
    old = ERROR;
    lcd2_clear();
    lcd2_home();
    lcd2_write_str_xy(0,0,"Aircondition");
    lcd2_write_str_xy(0,1,"Rene Meyer 2014");
    __delay_ms(5000);
    s = CHECK_CONFIG;
    while(1)
    {
        // State changed
	if(old != s)
	{
            lcd2_clear();
            lcd2_home();
            old = s;
            dirty = 1;	
	}
        switch (s)
	{
		case CHECK_CONFIG:
			state_check_config();
		break;
                case SET_CONFIG:
			state_set_config();
		break;
                case RUN:
                        state_run();
		break;
                case ERROR:
			state_set_config();
		break;
	}

    }

    return 0;
}

void state_check_config(void)
{
    // first read configdata
    uint8_t len = sizeof(AirConfigData);
    read_ds2430(6, (uint8_t*)&configData, len);
    // read crc
    uint8_t crc = 0;
    read_ds2430(6+len, (uint8_t*)&crc, sizeof(uint8_t));
    uint8_t realcrc = crc8((uint8_t*)&configData,len);
    lcd2_clear();
    lcd2_home();
    if(crc != realcrc)
    {
        lcd2_write_str_xy(0,0,"Invalid Config");
        lcd2_write_str_xy(0,1,"Press Knob");
        while(!knob_up());
        s = SET_CONFIG;
    }
    else
    {
        lcd2_write_str_xy(0,0,"Config OK");
        __delay_ms(1000);
        s = RUN;
    }
}

void state_set_config(void)
{
    uint16_t found = findSensors();
    if(found == 0)
    {
        lcd2_clear();
        lcd2_home();
        lcd2_write_str_xy(0,0,"No Sensors found!");
        lcd2_write_str_xy(0,1,"Restart Config");
        while(!knob_up());
        s = CHECK_CONFIG;
    }
    else
    {
        configData.inSensorID = chooseSensorID("Choose In Sensor", global_buffer, found);
        configData.outSensorID = chooseSensorID("Choose Out Sensor", global_buffer, found);
        configData.runTime = chooseIntVal("Runtime min",MIN_RUN_MINUTES,MAX_RUN_MINUTES,RUN_STEP);
        configData.pauseTime = chooseIntVal("Pause min",MIN_PAUSE_TIME_MINUTES, MAX_PAUSE_TIME_MINUTES,PAUSE_TIME_MINUTES_STEP);
        configData.threshold = chooseFloatVal("Ta Thresh. g/m^3",MIN_ABS_HUMIDITY_THRESHOLD, MAX_ABS_HUMIDITY_THRESHOLD, ABS_HUMIDITY_STEP,ABS_HUMIDITY_PRECISION);
        //Write Config to EEPROM
        uint8_t len = sizeof(AirConfigData);
        uint8_t buf[len];
        memcpy(buf,&configData,len);
        write_ds2430(6, buf, len);
        //write crc to EEPROM
        uint8_t crc = crc8((uint8_t*)&configData,len);
        uint8_t buf2[2];
        buf2[0]=crc;
        buf2[1]=0;
        write_ds2430(6+len, buf2, sizeof(uint8_t));
        lcd2_clear();
        lcd2_home();
        lcd2_write_str_xy(0,0,"Config. written");
        lcd2_write_str_xy(0,1,"Press Knob ");
        while(!knob_up());
        s = CHECK_CONFIG;
    }

}

void state_run(void)
{
    TX21SensorData inData;
    TX21SensorData outData;
    while(s == RUN)
    {
        char out = 0;
        int pause_counter = 0;
        int out_counter = 0;
        int found = findSensors();
        if(found <= 0)
        {
            lcd2_clear();
            lcd2_home();
            lcd2_write_str_xy(0,0,"Error:");
            lcd2_write_str_xy(0,0,"No Sensors!");
            while(!knob_up());
            s = ERROR;    
            return;
        }
        int inIdx = -1;
        int outIdx = -1;
        int i;
        for(i = 0; i < found; i++)
        {
            if(global_buffer[i].sensorID == configData.inSensorID)
            {
                inIdx = i;
            }
            else if(global_buffer[i].sensorID == configData.outSensorID)
            {
                outIdx = i;
            }
        }
        if(inIdx < 0 || outIdx < 0)
        {
            lcd2_clear();
            lcd2_home();
            lcd2_write_str_xy(0,0,"Error:");
            lcd2_write_str_xy(0,1,"No ID Match!");
            while(!knob_up());
            s = ERROR;
            return;
        }
        inData = global_buffer[inIdx];
        outData = global_buffer[outIdx];
        float absHumIn = getAbsoluteHumidity(&inData);
        float absHumOut = getAbsoluteHumidity(&outData);
        float comp = absHumOut + configData.threshold;
        if(comp < absHumIn)
        {
            out = 1;
            RELAIS_OUT_PIN = 1;
        }
        else
        {
            out = 0;
            RELAIS_OUT_PIN = 0;
        }
        //loop for Running Aircondition
        while((out_counter < configData.runTime * 60) && out && s == RUN)
        {
            char battbuff[8];
            lcd2_clear();
            lcd2_home();
            lcd2_write_str_xy(0,0,"Air Condition");
            lcd2_write_str_xy(0,1,"Run ");
            int secsLeft = (configData.runTime * 60 - out_counter);
            lcd2_write_U16(secsLeft,0);
            lcd2_write_str(" sec");
            __delay_ms(2000);
            lcd2_clear();
            lcd2_home();
            lcd2_write_str("In:");
            lcd2_write_U8_hex(inData.sensorID);
            lcd2_write_str(" ");
            getSensorBatteryStateString(battbuff,sizeof(battbuff),&inData);
            lcd2_write_str(battbuff);
            getSensorValueString(outbuf1,OUTPUTBUFSIZE, &inData);
            lcd2_write_str_xy(0,1,outbuf1);
            __delay_ms(2000);
            lcd2_clear();
            lcd2_home();
            lcd2_write_str("Out:");
            lcd2_write_U8_hex(outData.sensorID);
            lcd2_write_str(" ");
            getSensorBatteryStateString(battbuff,sizeof(battbuff),&outData);
            lcd2_write_str(battbuff);
            getSensorValueString(outbuf1,OUTPUTBUFSIZE, &outData);
            lcd2_write_str_xy(0,1,outbuf1);
            __delay_ms(2000);
            out_counter+=6;
            if(long_pressed())
            {
                s = ERROR;
            }
        }
        out = 0;
        RELAIS_OUT_PIN = 0;
        while(pause_counter < configData.pauseTime * 60 && s == RUN)
        {
            lcd2_clear();
            lcd2_home();
            lcd2_write_str_xy(0,0,"Air Condition");
            lcd2_write_str_xy(0,1,"Pause ");
            int secsLeft = (configData.pauseTime * 60 - pause_counter);
            lcd2_write_U16(secsLeft,0);
            lcd2_write_str(" sec");
            __delay_ms(1000);
            pause_counter += 1;
            if(long_pressed())
            {
                s = ERROR;
            }
        }
    }
}


int findSensors()
{
    global_counter = 0;
    lcd2_clear();
    lcd2_home();
    lcd2_write_str_xy(0,0,"Search TX21IT+");
    lcd2_write_str_xy(0,1,"Sensors");
    __delay_ms(1000);
    uint16_t counter = 0;
    uint16_t found = 0;
    char battbuff[8];
    while(counter < 50 && global_counter < GLOBAL_BUFFER_ELEMENTS)
    {
       enableRFM12();
       __delay_ms(1500);
       lcd2_clear();
       lcd2_home();
       lcd2_write_str("...");
       lcd2_write_U8(global_counter,0);
       lcd2_write_str(" Records");
       __delay_ms(500);
       counter++;
       resetRFM12();
       volatile uint16_t idx = global_counter;
       if(idx > 0)
       {
           TX21SensorData data = global_buffer[idx-1];
           lcd2_clear();
           lcd2_home();
           lcd2_write_U8_hex(data.sensorID);
           lcd2_write_str(" ");
           getSensorBatteryStateString(battbuff,sizeof(battbuff), &data);
           lcd2_write_str(battbuff);
           getSensorValueString(outbuf1,OUTPUTBUFSIZE, &data);
           lcd2_write_str_xy(0,1,outbuf1);
        __delay_ms(500);
       }
    }
    found = global_counter;
    return found;
}

uint8_t chooseSensorID(char* msg, TX21SensorData* outArray, uint8_t size)
{
    uint8_t ret = 0;
    lcd2_clear();
    lcd2_home();
    lcd2_write_str_xy(0,0,msg);
    lcd2_write_str_xy(0,1,"Rotate/press knob");
    while(!knob_up());
    int8_t idx = 0;
    int8_t old_idx = -1;
    TX21SensorData data;
    while(1)
    {
        char delta = encode_read4();
        if(delta < 0 && idx > 0)
        {
            idx--;
        }
        else if(delta > 0 && idx < (size-1))
        {
            idx++;
        }
        if(old_idx != idx)
        {
            old_idx = idx;
            lcd2_clear();
            lcd2_home();
            lcd2_write_U8_hex(outArray[idx].sensorID);
            data = outArray[idx];
            getSensorValueString(outbuf1,OUTPUTBUFSIZE, &data);
            lcd2_write_str_xy(0,1,outbuf1);
        }
        if(knob_up())
        {
            ret = outArray[idx].sensorID;
            break;
        }
     }
    return ret;
}

int16_t chooseIntVal(char* msg, int16_t min, int16_t max, int16_t step)
{
    char ret = -1;
    lcd2_clear();
    lcd2_home();
    int16_t idx = min;
    int16_t old_idx = -1;
    while(1)
    {
        char delta = encode_read4();
        if(delta < 0 && idx >= (min + step))
        {
            idx-=step;
        }
        else if(delta > 0 && idx <= (max-step))
        {
            idx+=step;
        }
        if(old_idx != idx)
        {
            old_idx = idx;
            lcd2_clear();
            lcd2_home();
            lcd2_write_str(msg);
            lcd2_goto_xy(0,1);
            lcd2_write_U16(idx,0);

        }
        if(knob_up())
        {
            ret = idx;
            break;
        }
     }
    return ret;
}

float chooseFloatVal(char* msg, float min, float max, float step,float precision)
{
    float ret = 0.0;
    lcd2_clear();
    lcd2_home();
    float idx = min;
    float old_idx = -1.0;
    while(1)
    {
        char delta = encode_read4();
        if(delta < 0 && idx >= (min + step))
        {
            idx-=step;
        }
        else if(delta > 0 && idx <= (max-step))
        {
            idx+=step;
        }
        if(old_idx != idx)
        {
            old_idx = idx;
            lcd2_clear();
            lcd2_home();
            lcd2_write_str(msg);
            //ftoa(floatbuf,idx,precision);
            char* floatbuf = sc_float_to_str(idx,precision);
            lcd2_goto_xy(0,1);
            lcd2_write_str(floatbuf);
        }
        if(knob_up())
        {
            ret = idx;
            break;
        }
     }
    return ret;
}

void enableRFM12(void)
{
    RFM12B_PORT_INIT();
    RFM12B_INIT(0);

    /*
     RFM12B_CMD(0xCA80); // enable sensitive reset (required for software reset)
     RFM12B_CMD(0xFE00); // software reset command
     __delay_ms(100); // give RFM time to do software reset (which should only take 0.25ms)
    RFM12B_CMD(0xCA81); // disable sensitive reset

     */
    RFM12B_CMD(0x0000);
    RFM12B_CMD(0x0000);
    RFM12B_CMD(0x82C8); // Init Receiver
    RFM12B_CMD(0xCA81); // Enable FIFO
    RFM12B_CMD(0xCA83); // Clear FIFO
    byteCount=0;
}

void resetRFM12(void)
{
    __asm__ volatile("disi #0x3FFF");
    RFM12B_CMD(0xff00);
     byteCount=0;
    __delay_ms(50);
    __asm__ volatile("disi #0x0000");
}



//_INT1Interrupt() is the INT1 interrupt service routine (ISR).
void __attribute__((__interrupt__)) _INT1Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _INT1Interrupt(void)
{
    RFM12B_CMD(0x0000);
    char byte = RFM12B_CMD(0xB000);
    if(byteCount!=RECEIVEBUFSIZE)
    {
        receiveBuff[byteCount] = byte;
        byteCount++;
    }
    else
    {
        byteCount = 0;
        RFM12B_CMD(0xCA81);
        RFM12B_CMD(0xCA83);
        TX21SensorData data;
        if(decodeITPlusFrame((unsigned char*) receiveBuff,&data) && global_counter < GLOBAL_BUFFER_ELEMENTS)
        {
            global_buffer[global_counter] = data;
            global_counter++;
        }
    }
   IFS1bits.INT1IF = 0;    //Clear the INT1 interrupt flag or else
   //the CPU will keep vectoring back to the ISR
}


// 1000Hz
void Timer1Init(void)
{
   PR1 = 8000;
   IPC0bits.T1IP = 1;	 //set interrupt priority
   T1CON = 0x8000;	//turn on the timer prescaler 256
   IFS0bits.T1IF = 0;	 //reset interrupt flag
   IEC0bits.T1IE = 1;	 //turn on the timer1 interrupt
}

//_T1Interrupt() is the T1 interrupt service routine (ISR).
void __attribute__((__interrupt__)) _T1Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
   //RELAIS_OUT_PIN =~ RELAIS_OUT_PIN;	//Toggle output to LED
   rotarydecode();
   debounce_switch();
   IFS0bits.T1IF = 0;
}
