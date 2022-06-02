#ifndef PRINTER
#define PRINTER

// Cpp - Arduino Library
#include "Arduino.h"

//
#include "SPI.h"

// Motor Driver Library
#include "AMIS30543.h"

//
#include "config.h"

// Stall Detection Function Library
#include "stall_detection.h"

// move motors in accordance to old 5 motor IK data messages
bool MoveCommand(long[5], char, long);

// steps all motors at safe speed until all have stalled
bool HomingCommand();

// test function to allow for extruding at a cm/s (double) for
// a set distance (int)
bool ExtrudeCommand(double, int);

// Sends a pulse on the NXT/STEP pin to tell the driver to take
// one step, and also delays to control the speed of the motor.
void Step(int);

// Writes a high or low value to the direction pin to specify
// what direction to turn the motor.
void SetDirection(int, bool);
#endif
