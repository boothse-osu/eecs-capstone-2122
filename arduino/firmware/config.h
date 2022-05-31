#ifndef CONFIG
#define CONFIG

// Cpp - Arduino Library
#include "Arduino.h"
//
#include "SPI.h"
// Motor Driver Library
#include "AMIS30543.h"

// Define Constants
#define MTR_NUMBER 5

// Steps for a full rotation on each motor
const int steps_per_rotations[6] = {1600,1600,1600,800,800,800};

const float step_degree = 360;
const float step_radian = 6.28318530718;
// Type of data sent in data messages
const float step_mode = step_radian;

// Command header presets the printer receives from the computer
const char HOMING_REQ   = 'h'; // Request for homing
const char MOVE_DATA    = 'D'; // Full data for move command
const char DEBUG        = '!'; // Request for debug movement
const char TEMP_SET     = 'n'; // Request to set temp on hot end
const char TEMP_REP     = 't'; // Request for hot end status
const char FILAMENT     = 'f'; // Request for filament extrusion test
const char EOF_MSG      = 'e'; // Signifies the End of the Print File

// Command header presets the printer sends back to the computer
const char HOMING_CON   = 'H'; // Confirmation that homing finished
const char DATA_REQ     = 'd'; // Request for more data
const char DATA_CON     = 'R'; // Confirmation that data was received
const char STOP         = 's'; // Message about printer stop (stops IK)
const char MESSAGE      = 'm'; // Message (does nothing but log)

// How many data message to receive over serial each time
const int data_length = 1;

// Direction for motors to reach their homed position
const int HOME_DIRECTION[MTR_NUMBER] = {0,1,0,1,0};
// Direction of motors corresponding to positive IK movement
const int POS_DIRECTION[MTR_NUMBER] = {1,0,1,1,1};
// Direction of motors corresponding to negative IK movement
const int NEG_DIRECTION[MTR_NUMBER] = {0,1,0,0,0};

// Placeholder value for time to complete movement
// This will be sent from the IK
const float mvmt_time = 0; //  3 000 000 = 3 seconds

// Minimum delays before motor slip
const int min_mtr_delays  [5] = {200, 500, 200, 200, 200};

// Minimum delay before motor slip
const int min_mtr_delay = 200;

// Stall_Array_Size:  How many voltages are used to calculate the
//                      stall-line.
// Stall_Check_Step:  How many steps between each time we check if
//                      the motor has stalled.
// Stall_Trigger_Amt: How many times in a row must a stall trigger
//                      to return a stall code.
// Stall_Line_Frac:   What should the stall-line be compared to the
//                      Average.
//                      Average/Stall_Line_Mult = Stall-Line
const int Stall_Array_Size[5]   =  {50,   50,   50,   25,   25};
const int Stall_Check_Step[5]   =  {5,    5,    5,    1,    1};
const int Stall_Trigger_Amt[5]  =  {2,    2,    2,    5,    5};
const float Stall_Line_Frac[5]  =  {2,    3,    3,    2,    2};

// PINS for Motors: ECE
const uint8_t amisDirPin[6] =       {PB11,  PB12, PB13, PB14, PB15, PA12};
const uint8_t amisStepPin[6] =      {PC5,   PC6,  PC7,  PC8,  PC9,  PA11};
const uint8_t amisSlaveSelect[6] =  {PB3,   PB4,  PB5,  PB6,  PB7,  PC3};
const uint8_t amisSLA[6] =          {PA0,   PA1,  PA4,  PB0,  PC1,  PC0};

// Pin replaced by extruder until board is fixed
const int extruder_pin = 5;

#endif