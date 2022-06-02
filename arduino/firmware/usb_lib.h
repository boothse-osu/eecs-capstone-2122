#ifndef USB
#define USB

// Cpp - Arduino Library
#include "Arduino.h"

//
#include "config.h"

// Stall Detection Function Library
#include "stall_detection.h"

// Printer Movement Function Library
#include "printer_control.h"

// Translates a 5 motor move command and calculates step amounts for
// each motor. Calls a the movement function and then requests more data.
void MoveHandler(String);

// Translates a full 6 motor move command and calculates step amounts for
// each motor and the extrusion speed. Calls a the movement function and 
// then requests more data.
void PrintMoveHandler(String);

// Prompt user for motor, direction, and step amount and runs that motor
void DebugHandler();

// Communication Functions

// <!m(msg)>: Simple way to send a message over serial with the correct
//    formatting. 
// - (note) Needs to be called with only strings: SendMessage("number:"+String(int));
// -  All other data must be turned into strings with String()
void SendMessage(String);

// <!s(msg)>: Sends a stop message over serial, stopping the IK. 
// - (note) Needs to be called with only strings: SendMessage("number:"+String(int));
// -  All other data must be turned into strings with String()
void SendStopMessage(String);

// <!R>: Called after we have confirmed we correctly received move data.
void SendConfirmData();

// <!d(num)>: Requests new data, num amount specificities how many data
//    arrays to be sent
void SendDataRequest(int);

// <!H>: Confirms that homing has completed.
void SendHomingComplete();
#endif
