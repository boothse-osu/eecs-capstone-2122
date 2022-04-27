#ifndef PRINTER
#define PRINTER

#include "Arduino.h"

/*
const uint8_t amisDirPin[5] = {PB11, PB12, PB13, PB14, PB15};
const uint8_t amisStepPin[5] = {PC5, PC6, PC7, PC8, PC9};
const uint8_t amisSlaveSelect[5] = {PB3, PB4, PB5, PB6, PB7};
const uint8_t amisSLA[5] = {PA0, PA1, PA4, PB0, PC1};
//*/
const float time_per_cm = 1000000*2; //  3 000 000 = 3 seconds

//AMIS30543 stepper;

//const int cm_step_amount = (int)((float)steps_per_rotation * 0.2875); // UNNEEDED NOW, working with roation angles
//const int mm_step_amount = (int)((float)cm_step_amount / 10); // UNNEEDED NOW, working with roation angles

void move_command(int[5], bool);

#endif
