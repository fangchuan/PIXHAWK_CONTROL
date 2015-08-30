#ifndef MAVLINK_RECIEVE_H
#define MAVLINK_RECIEVE_H

#include "r_cg_macrodriver.h"
#include "r_cg_port.h"

#define MAVLINK_STX  0xFE
#define MSG_SONAR_LENGTH 8
#define MSG_SONAR_ID     173
#define MSG_HEIGHT_LENGTH 16

#define MSG_ATTITUDE_LENGTH 28
#define MSG_ATTITUDE_ID  30
#define MSG_EULER_LENGTH 36

float  uchar_to_float(uint8_t *buffer);
void calculate_angle(void);
void  calculate_height(void);

#endif