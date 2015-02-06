/* 
 * File:   TX21IT.h
 * Author: rene
 *
 * Created on 25. August 2013, 14:04
 */

#ifndef TX21IT_H
#define	TX21IT_H
#include <stdint.h>


typedef struct sensordata
{
    uint8_t sensorID;
    float temperature;
    uint16_t humidity;
    uint8_t batLow;
}TX21SensorData;

uint8_t decodeITPlusFrame(uint8_t rawdata[5],TX21SensorData* data);

/*
  Name  : CRC-8
  Poly  : 0x31    x^8 + x^5 + x^4 + 1
  Init  : 0xFF
  Revert: false
  XorOut: 0x00
  Check : 0xF7 ("123456789")
  MaxLen: 15 bytes(127 bit)
*/
uint8_t crc8(uint8_t *pcBlock, uint8_t len);


void getSensorIDString(char* buffer, uint8_t buffersize,  TX21SensorData* data);

void getSensorValueString(char* buffer, uint8_t buffersize, TX21SensorData* data);

void getSensorBatteryStateString(char* buffer, uint8_t buffersize, TX21SensorData* data);

float getAbsoluteHumidity(TX21SensorData* data);

float getDewPoint(TX21SensorData* data);

void getAbsHumDewString(char* buffer, uint8_t buffersize, TX21SensorData* data);

#endif	/* TX21IT_H */

