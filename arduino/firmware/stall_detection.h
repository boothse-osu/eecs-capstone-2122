#ifndef STALL
#define STALL

// Cpp - Arduino Library
#include "Arduino.h"

//
#include "config.h"


// Struct for holding all stall information for a specific motor.
struct StallData {
    int len, sum, stall_trigger;
    float stall_line;
};

// Sets variables in struct to initial values.
struct StallData createVoltageAverage();

// Push a new Voltage for the motor indexed by int and pass all of
// its stall data in a struct.
// Returns: True - No Stall | False - Stall
bool pushVoltage(int, struct StallData*);

#endif
