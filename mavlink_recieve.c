#include "mavlink_recieve.h"
#include "r_cg_serial.h"

extern uint8_t Height_Buffer[20];
extern uint8_t Angle_Buffer[40];

float  current_height;
float  Roll,Pitch,Yaw,Roll_Speed,Pitch_Speed,Yaw_Speed;
union 
{ 
     float sub_float; 
     uint8_t buf[4];
} uchar_float;

float  uchar_to_float(uint8_t *buffer)
{
       uchar_float.buf[0] = *(buffer+6);
       uchar_float.buf[1] = *(buffer+7);
       uchar_float.buf[2] = *(buffer+8);
       uchar_float.buf[3] = *(buffer+9);
       return(uchar_float.sub_float);
}

void calculate_height(void)
{
	 static float height_last;
     current_height = uchar_to_float(Height_Buffer);

	 return_hight();
}

void calculate_angle(void)
{
	 float roll_temp,pitch_temp,yaw_temp;
	 roll_temp = uchar_to_float(Angle_Buffer+4)*1000;
	 pitch_temp = uchar_to_float(Angle_Buffer + 8)*1000;
	 yaw_temp = uchar_to_float(Angle_Buffer + 12)*1000;
	 
	 Roll = roll_temp *0.1;
	 Pitch = pitch_temp *0.1;
	 Yaw = yaw_temp *0.1;
	 
	 return_angle();
}