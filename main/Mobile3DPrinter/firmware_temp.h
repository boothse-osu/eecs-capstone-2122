#ifndef FIRMWARE_TEMP
#define FIRMWARE_TEMP

#include <stdio.h>
#include <stdlib.h>


int move_prismatic(int, float);

int move_rotational(int, float);

int move_motors(float*);

int run_homing();

#endif
