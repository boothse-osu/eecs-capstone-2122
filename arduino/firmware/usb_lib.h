#ifndef USB
#define USB

// Cpp - Arduino Library
#include "Arduino.h"

// Stall Detection Function Library
#include "stall_detection.h"

// Printer Movement Function Library
#include "printer_control.h"

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

// Command header presets the printer sends back to the computer
const char HOMING_CON   = 'H'; // Confirmation that homing finished
const char DATA_REQ     = 'd'; // Request for more data
const char DATA_CON     = 'R'; // Confirmation that data was received
const char STOP         = 's'; // Message about printer stop (stops IK)
const char MESSAGE      = 'm'; // Message (does nothing but log)

const int data_length = 3;

// Direction for motors to reach their homed position
const int HOME_DIRECTION[MTR_NUMBER] = {0,1,0,1,0};
// Direction of motors corresponding to positive IK movement
const int POS_DIRECTION[MTR_NUMBER] = {1,0,1,1,1};
// Direction of motors corresponding to negative IK movement
const int NEG_DIRECTION[MTR_NUMBER] = {0,1,0,0,0};

// Translates a 5 motor move command and calculates step amounts for
// each motor. Calls a the movement function and then requests more data.
void handle_move(String);

// Translates a full 6 motor move command and calculates step amounts for
// each motor and the extrusion speed. Calls a the movement function and 
// then requests more data.
void handle_print_move(String);

// Test extrusion that takes in a cm/s and distance and passes them to
// the extrude function
void handle_filament_test(String);

// Calls a homing command and then sends a confirmation of completion
void handle_homing();

// Prompt user for motor, direction, and step amount and runs that motor
void handle_debug();

// Communication Functions

// <!m(msg)>: Simple way to send a message over serial with the correct
//    formatting. 
// - (note) Needs to be called with only strings: send_message("number:"+String(int));
// -  All other data must be turned into strings with String()
void send_message(String);

// <!s(msg)>: Sends a stop message over serial, stopping the IK. 
// - (note) Needs to be called with only strings: send_message("number:"+String(int));
// -  All other data must be turned into strings with String()
void stop_message(String);

// <!R>: Called after we have confirmed we correctly received move data.
void confirm_data();

// <!d(num)>: Requests new data, num amount specificities how many data
//    arrays to be sent
void request_data(int);

// <!H>: Confirms that homing has completed.
void confirm_homing();
#endif
