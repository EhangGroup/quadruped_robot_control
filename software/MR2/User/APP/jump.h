#ifndef JUMP_H
#define JUMP_H
#include "robocon.h"


void StartJump(float start_time_s);
void TrajectoryJump(float t, float launchTime, float stanceHeight, float downAMP) ;
void ExecuteJump();

extern float jump_angle;

extern float jump_extension; // ������ȳ��� [cm]


#endif
