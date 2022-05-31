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
void handle_move(String str) {
  // Array of steps for the old 5 motor IK
  long mtr_steps[5];

  // IK used to send a T/F for hot-end operation
  char hot_end = str.charAt(55);

  unsigned long time_micro = 0;
  // Log initial motor command
  //send_message(str);
  
  // Parse data for angle changes and calculate steps for each motor
  for(int i = 0; i<5; i++) {
    // (steps_per_rotations[i] / step_mode): step conversion
    // str.substring((i*11), (i*11)+10).toDouble(): grab 10 digit change
    // - (note) 11s get start and end positions accounting for comma
    mtr_steps[i] = round(str.substring((i*11), (i*11)+10).toDouble() * (steps_per_rotations[i] / step_mode));
    //send_message(String(mtr_steps[i]));
  }
  
  // Confirm data has been received
  //confirm_data();

  // Call move command and request more dat if successful
  // Only requests one more command atm due to low memory on micro-controller
  if(move_command(mtr_steps, hot_end, time_micro)) return;
  return;
}

// Translates a full 6 motor move command and calculates step amounts for
// each motor and the extrusion speed. Calls a the movement function and 
// then requests more data.
void handle_print_move(String str) {
  // Array of steps for the 5 main motors
  long mtr_steps[5];
  // Cm per Second for 6th motor
  double cms = str.substring(55,63).toDouble();

  // Log initial motor command
  //send_message(str);

  // Parse data for angle changes and calculate steps for each motor
  for(int i = 0; i<5; i++) {
    // (steps_per_rotations[i] / step_mode): step conversion
    // str.substring((i*11), (i*11)+10).toDouble(): grab 10 digit change
    // - (note) 11s get start and end positions accounting for commas
    mtr_steps[i] = round(str.substring((i*11), (i*11)+10).toDouble() * (steps_per_rotations[i] / step_mode));
    send_message(String(mtr_steps[i]));
  }

  // Confirm data has been received
  confirm_data();


  send_message("CM/S: " + String(cms));
  // If cm/s = 0 delay = 0 to avoid divide by zero
  long delay_time;
  if(cms==0) delay_time = 0;
  else {
    // Translate cm/s into microsecond step delay for motors
    int cm_step_amount = 461;
    delay_time = 1000000/(cm_step_amount*cms);
  }
  send_message("Delay: " + String(delay_time));

  // Call move command and request more dat if successful
  // Only requests one more command atm due to low memory on micro-controller
  //if(print_move_command(mtr_steps,delay_time)) request_data(data_length);
  return;
}

// Prompt user for motor, direction, and step amount and runs that motor
void handle_debug(){
  long mtr_command[5] = {0};

  // Prompt for input
  Serial.println("\nmtr num, direction, 5 digit step amount");
  Serial.println("(0-"+ String(MTR_NUMBER-1)+")  ,(0,1,-,+) ,XXXXX\n");
  Serial.println("X,X,XXXXX");

  // Wait for response
  while(Serial.available() == 0) {
  }
  delay(100);
  String str = Serial.readString();
  send_message(str);
  
  int mtr = str.substring(0,1).toInt();
  String direct = str.substring(2,3);
  long steps = str.substring(4,10).toInt();

  if(direct == "+" || direct == String(POS_DIRECTION[mtr])) delay(1);
  else if(direct == "-" || direct == String(NEG_DIRECTION[mtr])) steps *= -1;
  else send_message("UNKNOWN DIRECTION");

  
  mtr_command[mtr] = steps;
  //new_move_command(mtr_command, 0);
  return;
}

/////////////////////////////////////////////////////////////////////
// Communication Functions:

// <!m(msg)>: Simple way to send a message over serial with the correct
//    formatting. 
// - (note) Needs to be called with only strings: send_message("number:"+String(int));
// -  All other data must be turned into strings with String()
void send_message(String msg) {
  Serial.println("<!" + String(MESSAGE) + "("  + msg + ")>");
}

// <!s(msg)>: Sends a stop message over serial, stopping the IK. 
// - (note) Needs to be called with only strings: send_message("number:"+String(int));
// -  All other data must be turned into strings with String()
void stop_message(String msg) {
  Serial.println("<!" + String(STOP) + "("  + msg + ")>");
}

// <!R>: Called after we have confirmed we correctly received move data.
void confirm_data() {
  Serial.println("<!" + String(DATA_CON) +  ">");
}

// <!d(num)>: Requests new data, num amount specificities how many data
//    arrays to be sent
void request_data(int num) {
  Serial.println("<!" + String(DATA_REQ) + "(" + String(num) + ")>");
}

// <!H>: Confirms that homing has completed.
void confirm_homing() {
  Serial.println("<!" + String(HOMING_CON) + ">");
}
