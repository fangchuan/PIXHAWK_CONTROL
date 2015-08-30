#ifndef PIX_CONTROL_H
#define PIX_CONTROL_H

#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
#include "delay.h"

#define  ROLL_INPUT     TDR01       
#define  PITCH_INPUT    TDR02       
#define  THROTTLE_INPUT TDR03       
#define  YAW_INPUT      TDR04       
#define  MODE_INPUT     TDR05       

#define  MAX_INPUT   4000
#define  MIN_INPUT   2000
#define  MID_INPUT   3000

#define  THROTTLE_UP 3250
#define  THROTTLE_DOWN 2600

void unlock_pixhawk(void);
void lock_pixhawk(void);
void alt_hold_mode(void);
void stabilize_mode(void);
void stop_motor(void);
void climb_up(void);
void climb_down(void);
void climb_stop(void);

#endif