#ifndef USB
#define USB

// Cpp - Arduino Library
#include "Arduino.h"

// Stall Detection Function Library
#include "stall_detection.h"




// Printer Movement Function Library
#include "printer_control.h"


// first 3 motors are 1600, last two are 800!!!!!!!!!!!!!!
const int steps_per_rotation = 800;
const int steps_per_rotations[5] = {1600,1600,1600,800,800};



const float step_degree = 360;
const float step_radian = 6.28318530718;
const float step_mode = step_radian;

const float steps_per_degree = (float)steps_per_rotation / 360;
const float steps_per_radian = (float)steps_per_rotation / 6.28318530718;
const float steps_per_x = steps_per_radian;

const char HOMING_REQ   = 'h'; // Maybe make these strings
const char HOMING_CON   = 'H';
const char DATA_REQ     = 'd';
const char MOVE_DATA    = 'D';
const char DATA_CON     = 'R';
const char EOF_MSG      = 'e';
const char EOF_CON      = 'E';
const char STOP         = 's';
const char MESSAGE      = 'm';
const char DEBUG        = '!';
const char TEMP_SET     = 'n';
const char TEMP_REP     = 't';
const char FILAMENT     = 'f';

const int HOME_DIRECTION[MTR_NUMBER] = {0,1,0,1,0};
const int POS_DIRECTION[MTR_NUMBER] = {1,1,1,1,1};
const int NEG_DIRECTION[MTR_NUMBER] = {0,0,0,0,0};

void handle_move(String);

void handle_print_move(String);

void handle_filament_test(String);

void handle_homing();

void handle_debug();

void send_message(String);

void stop_message(String);

void confirm_data();

void request_data(int);

void confirm_homing();


#endif
