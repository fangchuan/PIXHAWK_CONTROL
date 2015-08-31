#include "test.h"

extern   float current_height;
extern   float Yaw;

void   test_task_1(void)
{
	   alt_hold_mode();
	   if(current_height < TARGET_HEIGHT-0.1)
	   {
		   climb_up();
	   }
	   else
	       if(current_height >TARGET_HEIGHT+0.1)
		   {
		       climb_down();
		   }
		   else
		   {
			   climb_stop();
		   }
}