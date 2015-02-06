/* 
 * File:   aircondition.h
 * Author: rene
 *
 * Created on 14. Juni 2014, 13:57
 */

#ifndef AIRCONDITION_H
#define	AIRCONDITION_H
#include <stdint.h>

typedef struct AirConfigData
{
    uint8_t outSensorID;
    uint8_t inSensorID;
    uint16_t pauseTime;
    uint16_t runTime;
    float threshold;
}AirConfigData;

#define MAX_RUN_MINUTES 120
#define MIN_RUN_MINUTES 5
#define RUN_STEP 5


#define MAX_ABS_HUMIDITY_THRESHOLD 5.0
#define MIN_ABS_HUMIDITY_THRESHOLD 0.5
#define ABS_HUMIDITY_STEP 0.1
#define ABS_HUMIDITY_PRECISION 1


#define MIN_PAUSE_TIME_MINUTES 5
#define MAX_PAUSE_TIME_MINUTES 120
#define PAUSE_TIME_MINUTES_STEP 5

#define MAX_RECEIVE_RECORDS 30

#endif	/* AIRCONDITION_H */

