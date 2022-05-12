#ifndef STALL
#define STALL

// Cpp - Arduino Library
#include "Arduino.h"

// Define Constants
#define MTR_NUMBER 5

// Stall_Array_Size:  How many voltages are used to calculate the
//                      stall-line.
// Stall_Check_Step:  How many steps between each time we check if
//                      the motor has stalled.
// Stall_Trigger_Amt: How many times in a row must a stall trigger
//                      to return a stall code.
// Stall_Line_Frac:   What should the stall-line be compared to the
//                      Average.
//                      Average/Stall_Line_Mult = Stall-Line
const int Stall_Array_Size[5]   =  {20,   20,   20,   10,   10};
const int Stall_Check_Step[5]   =  {5,    5,    5,    1,    1};
const int Stall_Trigger_Amt[5]  =  {2,    2,    2,    3,    3};
const float Stall_Line_Frac[5]  =  {2,    2,    2,    1.75, 1.75};

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
