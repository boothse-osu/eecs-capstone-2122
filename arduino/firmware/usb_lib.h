#ifndef USB
#define USB

#include "Arduino.h"
#include "stall_detection.h"
#include "printer_control.h"

const int steps_per_rotation = 800; // I forgot again (maybe 800)
const float steps_per_degree = (float)steps_per_rotation / 360;
const float steps_per_radian = (float)steps_per_rotation / 6.28318530718;
const float steps_per_x = steps_per_radian;

const char HOMING_REQ   = 'h'; // Maybe make these strings
const char HOMING_CON   = 'H';
const char DATA_REQ     = 'd';
const char DATA         = 'D';
const char DATA_CON     = 'R';
const char EOF_MSG      = 'e';
const char EOF_CON      = 'E';
const char STOP         = 's';
const char MESSAGE      = 'm';
const char DEBUG        = '!';

const int HOME_DIRECTION[MTR_NUMBER] = {0,1,0,1,0};
const int POS_DIRECTION[MTR_NUMBER] = {1,1,1,1,1};
const int NEG_DIRECTION[MTR_NUMBER] = {0,0,0,0,0};

void parse_data(String);

void homing_sequence();

void debug_mode();

void send_message(String);

void stop_message(String);

void confirm_data();

void request_data(int);

void confirm_homing();


#endif
