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
const uint8_t amisDirPin[5] = {PB11, PB12, PB13, PB14, PB15};
const uint8_t amisStepPin[5] = {PC5, PC6, PC7, PC8, PC9};
const uint8_t amisSlaveSelect[5] = {PB3, PB4, PB5, PB6, PB7};
const uint8_t amisSLA[5] = {PA0, PA1, PA4, PB0, PC1};

// Placeholder value for time to complete movement
// This will be sent from the IK
const float mvmt_time = 5000; //  3 000 000 = 3 seconds

// Minimum delay before motor slip
const int min_mtr_delay = 200;

// Pin replaced by extruder until board is fixed
const int extruder_pin = 2;

// move motors in accordance to old 5 motor IK data messages
bool new_move_command(long[5], bool);

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
