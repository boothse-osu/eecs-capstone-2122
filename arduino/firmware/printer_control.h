#ifndef PRINTER
#define PRINTER

// Cpp - Arduino Library
#include "Arduino.h"

//
#include "SPI.h"

// Motor Driver Library
#include "AMIS30543.h"

// Stall Detection Function Library
#include "stall_detection.h"

// PINS for Motors: ECE
const uint8_t amisDirPin[6] =       {PB11,  PB12, PB13, PB14, PB15, PA12};
const uint8_t amisStepPin[6] =      {PC5,   PC6,  PC7,  PC8,  PC9,  PA11};
const uint8_t amisSlaveSelect[6] =  {PB3,   PB4,  PB5,  PB6,  PB7,  PC3};
const uint8_t amisSLA[6] =          {PA0,   PA1,  PA4,  PB0,  PC1,  PC0};

// Placeholder value for time to complete movement
// This will be sent from the IK
const float mvmt_time = 0; //  3 000 000 = 3 seconds

// Minimum delays before motor slip
const int min_mtr_delays  [5] = {200, 500, 200, 200, 200};

// Minimum delay before motor slip
const int min_mtr_delay = 200;

// Pin replaced by extruder until board is fixed
const int extruder_pin = 5;

// move motors in accordance to old 5 motor IK data messages
bool new_move_command(long[5], char);//, double);

// moves all 5 motors while extruding on a 6th at a set cm/s
bool print_move_command(long[5], long);

// steps all motors at safe speed until all have stalled
bool homing_command();

// test function to allow for extruding at a cm/s (double) for
// a set distance (int)
bool extrude(double, int);

// Sends a pulse on the NXT/STEP pin to tell the driver to take
// one step, and also delays to control the speed of the motor.
void step(int);

// Writes a high or low value to the direction pin to specify
// what direction to turn the motor.
void setDirection(int, bool);
#endif
