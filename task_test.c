#include "test.h"

extern   float current_height;
extern   float Yaw;

void   test_task_1(void)
{
	   if(current_height < TARGET_HEIGHT-10)
	   {
		   alt_hold_mode();
		   climb_up();
	   }
	   else
	       if(current_height >TARGET_HEIGHT)
		   {
			   alt_hold_mode();
		       climb_down();
		   }
		   else
		   {
			   alt_hold_mode();
			   climb_stop();
		   }
}