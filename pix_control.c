#include "pix_control.h"

//TDR01------roll
//TDR02------pitch
//TDR03------throttle
//TDR04------yaw
//TDR05------mode

extern float current_height;

//unlock pixhawk   max_yaw_input
void unlock_pixhawk(void)
{
	 YAW_INPUT = MAX_INPUT;
	 delay_nms(5000);
	 YAW_INPUT = MID_INPUT;
}
//lock pixhawk  min_yaw_input
void lock_pixhawk(void)
{
	 YAW_INPUT = MIN_INPUT;
	 delay_nms(5000);
	 YAW_INPUT = MID_INPUT;
}
//alt_hod_mode
void alt_hold_mode(void)
{
	 MODE_INPUT = MID_INPUT;//altitude hold mode
}

//stabilize_mode
void stabilize_mode(void)
{
	 MODE_INPUT = MIN_INPUT;//stabilize mode
}
//land
void alt_land(void)
{
	MODE_INPUT = MID_INPUT;//altitude hold mode
	THROTTLE_INPUT = THROTTLE_DOWN;
	delay_nms(3000);
}
//stop motor
void stop_motor(void)
{
	MODE_INPUT = MAX_INPUT;//stabilize mode
	THROTTLE_INPUT = MIN_INPUT;
}
//climb up or down in alt_hold_mode
void climb_up(void)
{
	 THROTTLE_INPUT = THROTTLE_UP;
}
void climb_down(void)
{
	 THROTTLE_INPUT = THROTTLE_DOWN;
}
void climb_stop(void)
{
	 THROTTLE_INPUT = MID_INPUT;
}