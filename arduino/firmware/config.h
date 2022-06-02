#ifndef CONFIG
#define CONFIG

// Cpp - Arduino Library
#include "Arduino.h"

//
#include "SPI.h"

// Motor Driver Library
#include "AMIS30543.h"

// Define Constants
// Amount of movement motors
#define MTR_NUMBER 5
// Index of extruder pin info
#define EXTRUDER_PIN 5
// How many data message to receive over serial each time
#define DATA_REQUEST_AMOUNT 1

// Steps for a full rotation on each motor
const int motorStepsPerRotation[6] = {1600,1600,1600,800,800,800};

// Type of data sent in data messages
const double radianConversion = 6.28318530718;

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

// Direction of motors corresponding to positive IK movement
const int POS_DIRECTION[MTR_NUMBER] = {1,0,1,0,1};
// Direction of motors corresponding to negative IK movement
const int NEG_DIRECTION[MTR_NUMBER] = {0,1,0,1,0};

// Minimum delay before motor slip
const int minMotorDelay = 200;

//long extruderDelay = 1000;

// dataPointsInStallLine:  How many voltages are used to calculate the
//                      stall-line.
// stallCheckStepInterval:  How many steps between each time we check if
//                      the motor has stalled.
// stallTriggerAmount: How many times in a row must a stall trigger
//                      to return a stall code.
// stallLineFraction:   What should the stall-line be compared to the
//                      Average.
//                      .5 = Half of Average
const int dataPointsInStallLine[5]  = {50,   50,   50,   25,   25};
const int stallCheckStepInterval[5] = {5,    5,    5,    1,    1};
const int stallTriggerAmount[5]     = {2,    2,    2,    5,    5};
const float stallLineFraction[5]    = {.5,   .3,   .3,   .5,   .5};

// PINS for Motors: ECE
const uint8_t amisDirPin[6] =       {PB11,  PB12, PB13, PB14, PB15, PA12};
const uint8_t amisStepPin[6] =      {PC5,   PC6,  PC7,  PC8,  PC9,  PA11};
const uint8_t amisSlaveSelect[6] =  {PB3,   PB4,  PB5,  PB6,  PB7,  PC3};
const uint8_t amisSLA[6] =          {PA0,   PA1,  PA4,  PB0,  PC1,  PC0};



#endif
