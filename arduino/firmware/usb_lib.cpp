// Cpp - Arduino Library
#include "Arduino.h"

//
#include "config.h"

// USB Communication Function Library
#include "usb_lib.h"

// Printer Movement Function Library
#include "printer_control.h"

// Translates a _5_ motor move command and calculates step amounts for
// each motor. Calls a the movement function and then requests more data.
void MoveHandler(String str) {
  // Array of steps for the old 5 motor IK
  long motorStepAmounts[5];

  // IK used to send a T/F for hot-end operation
  char hotEndSignifier = str.charAt(55);

  unsigned long movementTime = 0;
  // Log initial motor command
  //SendMessage(str);
  
  // Parse data for angle changes and calculate steps for each motor
  for(int i = 0; i<5; i++) {
    // (motorStepsPerRotation[i] / radianConversion): step conversion
    // str.substring((i*11), (i*11)+10).toDouble(): grab 10 digit change
    // - (note) 11s get start and end positions accounting for comma
    motorStepAmounts[i] = round(str.substring((i*11), (i*11)+10).toDouble() * (motorStepsPerRotation[i] / radianConversion));
    //SendMessage(String(motorStepAmounts[i]));
  }
  
  // Confirm data has been received
  //SendConfirmData();

  // Call move command and request more dat if successful
  // Only requests one more command atm due to low memory on micro-controller
  if(MoveCommand(motorStepAmounts, hotEndSignifier, movementTime)) return;
  return;
}

// Translates a full 6 motor move command and calculates step amounts for
// each motor and the extrusion speed. Calls a the movement function and 
// then requests more data.
void PrintMoveHandler(String str) {
  // Array of steps for the 5 main motors
  long motorStepAmounts[5];
  // Cm per Second for 6th motor
  double cmPerSecond = str.substring(55,63).toDouble();

  // Log initial motor command
  //SendMessage(str);

  // Parse data for angle changes and calculate steps for each motor
  for(int i = 0; i<5; i++) {
    // (motorStepsPerRotation[i] / radianConversion): step conversion
    // str.substring((i*11), (i*11)+10).toDouble(): grab 10 digit change
    // - (note) 11s get start and end positions accounting for commas
    motorStepAmounts[i] = round(str.substring((i*11), (i*11)+10).toDouble() * (motorStepsPerRotation[i] / radianConversion));
    SendMessage(String(motorStepAmounts[i]));
  }

  // Confirm data has been received
  SendConfirmData();


  SendMessage("CM/S: " + String(cmPerSecond));
  // If cm/s = 0 delay = 0 to avoid divide by zero
  long extruderDelay;
  if(cmPerSecond==0) extruderDelay = 0;
  else {
    // Translate cm/s into microsecond step delay for motors
    int cmExtrudeAmount = 461;
    extruderDelay = 1000000/(cmExtrudeAmount*cmPerSecond);
  }
  SendMessage("Delay: " + String(extruderDelay));

  // Call move command and request more dat if successful
  // Only requests one more command atm due to low memory on micro-controller
  //if(print_move_command(motorStepAmounts,extruderDelay)) SendDataRequest(DATA_REQUEST_AMOUNT);
  return;
}

// Prompt user for motor, direction, and step amount and runs that motor
void DebugHandler(){
  long debugMotorStepAmounts[5] = {0};

  // Prompt for input
  Serial.println("\nmtr num, direction, 5 digit step amount");
  Serial.println("(0-"+ String(MTR_NUMBER-1)+")  ,(0,1,-,+) ,XXXXX\n");
  Serial.println("X,X,XXXXX");

  // Wait for response
  while(Serial.available() == 0) {
  }
  delay(100);
  String debugCommand = Serial.readString();
  SendMessage(debugCommand);
  
  int motorIndex = debugCommand.substring(0,1).toInt();
  String motorDirection = debugCommand.substring(2,3);
  long motorStepAmount = debugCommand.substring(4,10).toInt();

  if(motorDirection == "+" || motorDirection == String(POS_DIRECTION[motorIndex])) delay(1);
  else if(motorDirection == "-" || motorDirection == String(NEG_DIRECTION[motorIndex])) motorStepAmount *= -1;
  else SendMessage("UNKNOWN DIRECTION");

  
  debugMotorStepAmounts[motorIndex] = motorStepAmount;
  //new_move_command(debugMotorStepAmounts, 0);
  return;
}

/////////////////////////////////////////////////////////////////////
// Communication Functions:

// <!m(msg)>: Simple way to send a message over serial with the correct
//    formatting. 
// - (note) Needs to be called with only strings: SendMessage("number:"+String(int));
// -  All other data must be turned into strings with String()
void SendMessage(String msg) {
  Serial.println("<!" + String(MESSAGE) + "("  + msg + ")>");
}

// <!s(msg)>: Sends a stop message over serial, stopping the IK. 
// - (note) Needs to be called with only strings: SendMessage("number:"+String(int));
// -  All other data must be turned into strings with String()
void SendStopMessage(String msg) {
  Serial.println("<!" + String(STOP) + "("  + msg + ")>");
}

// <!R>: Called after we have confirmed we correctly received move data.
void SendConfirmData() {
  Serial.println("<!" + String(DATA_CON) +  ">");
}

// <!d(num)>: Requests new data, num amount specificities how many data
//    arrays to be sent
void SendDataRequest(int num) {
  Serial.println("<!" + String(DATA_REQ) + "(" + String(num) + ")>");
}

// <!H>: Confirms that homing has completed.
void SendHomingComplete() {
  Serial.println("<!" + String(HOMING_CON) + ">");
}
