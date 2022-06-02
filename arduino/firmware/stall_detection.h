#ifndef STALL
#define STALL

// Cpp - Arduino Library
#include "Arduino.h"

//
#include "config.h"


// Struct for holding all stall information for a specific motor.
struct stallData {
    int len, sum, stallTrigger;
    float stallLine;
};

// Sets variables in struct to initial values.
struct stallData CreateVoltageAverage();

// Push a new Voltage for the motor indexed by int and pass all of
// its stall data in a struct.
// Returns: True - No Stall | False - Stall
bool PushVoltage(int, struct stallData*);

#endif
