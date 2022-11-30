#pragma once
#define STOP_STATE 0
#define LOWER_STATE 1
#define CLIMB_STATE 2
//globals
static float threshold = 0; 

// 0 = stop, 1 = lower, 2 = climb
static int state = 0;