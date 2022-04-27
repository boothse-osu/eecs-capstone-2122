#ifndef USB
#define USB

#include "Arduino.h"

const int steps_per_rotation = 1600; // I forgot again (maybe 800)
const float steps_per_degree = (float)steps_per_rotation / 360;

const char HOMING_REQ   = 'h'; // Maybe make these strings
const char HOMING_CON   = 'H';
const char DATA_REQ     = 'd';
const char DATA         = 'D';
const char DATA_CON     = 'R';
const char EOF_MSG      = 'e';
const char EOF_CON      = 'E';
const char STOP         = 's';
const char MESSAGE      = 'm';

void parse_data();

void homing_sequence();

void send_message(String);

void stop_message(String);

void confirm_data();

void request_data(int);

void confirm_homing();


#endif