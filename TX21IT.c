/* 
 * File:   TX21IT.c
 * Author: rene
 *
 * Created on 25. August 2013, 14:03
 */

#include <string.h>
#include <math.h>
#include "stringconversion.h"
#include "TX21IT.h"

const static char BATT_OK[] = "BAT OK";
const static char BATT_LOW[] = "BAT LOW";
const static char SENSOR_STRING[] ="ID:";
const static char ABS_STRING[]  = "Ha:";
const static char TDEW_STRING[]  = "Td:";


uint8_t decodeITPlusFrame(uint8_t temp[5],TX21SensorData* data)
{
    uint8_t ret = 0;
    uint8_t crcEx = temp[4];
    uint8_t crcResult = crc8((unsigned char*)temp,4);

    if(crcEx == crcResult)
    {
        uint8_t high_nibble = temp[0] <<4;
        uint8_t low_nibble =  temp[1] & 0xF0;
        low_nibble = low_nibble / 0xF;
        low_nibble = low_nibble & 0xFC;
        data->sensorID = (high_nibble + low_nibble) >> 2;
        uint8_t T10 = (temp[1] << 4);
        T10 = T10 / 16;
        uint8_t T1 = (temp[2] & 0xF0) >> 4;
        uint8_t T01 = (temp[2] << 4);
        T01 = T01 / 16;
        float test = (float)T10;
        test = test * 10 - 40.0;
        test += T1;
        test += ((float)T01/10.0);
        data->temperature = test;
        data->batLow = temp[3] & 0b10000000;
        data->humidity= temp[3] & 0b01111111;
        ret = 1;
    }
    return ret;
}

/*
  Name  : CRC-8
  Poly  : 0x31    x^8 + x^5 + x^4 + 1
  Init  : 0x00
  Revert: false
  XorOut: 0x00
  Check : 0xF7 ("123456789")
  MaxLen: 15 bytes(127 bit)
*/
uint8_t crc8(uint8_t *pcBlock, uint8_t len)
{
    uint8_t crc = 0x00;
    uint8_t i;

    while (len--)
    {
        crc ^= *pcBlock++;

        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }

    return crc;
}


void getSensorValueString(char* buffer, uint8_t buffersize, TX21SensorData* data)
{
    memset(buffer,0x00,buffersize);
    buffer[0] = 'T';
    char* floatString = sc_float_to_str(data->temperature,1);
    uint8_t size = strlen(floatString);
    strncpy(buffer+1,floatString,size);
    buffer[1+size] = 0xDF;//degree Celsius like
    buffer[2+size] = 'H';
    char* intString = sc_S08_to_str(data->humidity,0);
    uint8_t size2 = strlen(intString);
    strncpy(3+buffer+size,intString,size2);
    buffer[3+size+size2]=0b00100101;
}

void getSensorBatteryStateString( char* buffer, uint8_t buffersize, TX21SensorData* data)
{
    memset(buffer,0x00,buffersize);
    if(data->batLow)
    {
        strncpy(buffer, BATT_LOW, sizeof(BATT_LOW));
    }
    else
    {
       strncpy(buffer, BATT_OK, sizeof(BATT_OK));
    }
}

void getSensorIDString(char* buffer, uint8_t buffersize, TX21SensorData* data)
{
    memset(buffer,0x00,buffersize);
    char* sensorID = sc_S08_to_str(data->sensorID,0);
    strcat(buffer, SENSOR_STRING);
    strcat(buffer, sensorID);
}

void getAbsHumDewString(char* buffer, uint8_t buffersize, TX21SensorData* data)
{
    float val = getAbsoluteHumidity(data);
    memset(buffer,0x00,buffersize);
    strcpy(buffer, ABS_STRING);
    strcat(buffer, sc_float_to_str(val,1));
    float dew = getDewPoint(data);
    strcat(buffer,TDEW_STRING);
    strcat(buffer,sc_float_to_str(dew,1));
    uint8_t size = strlen(buffer);
    buffer[size]=0xDF; //degree Celsius like
}

float getAbsoluteHumidity(TX21SensorData* data)
{
    double temp = (double) data->temperature;
    double hum = (double) data->humidity;
    double sdd, dd, af;
    sdd=6.1078 * pow(10,(7.5*temp)/(237.3+temp));
    dd=hum/100.0*sdd;
    af=216.687*dd/(273.15+temp);
    return (float)af;
}

float getDewPoint(TX21SensorData* data)
{
    double t = (double) data->temperature;
    double h = (double) data->humidity;
    double logEx;
    double taupunkt;
    logEx = 0.66077+7.5*t / (237.3+t) + (log10(h)-2);
    taupunkt = (logEx - 0.66077)*237.3/(0.66077+7.5-logEx);
    return (float)taupunkt;
}










