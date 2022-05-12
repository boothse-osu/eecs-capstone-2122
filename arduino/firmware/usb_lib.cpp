// Cpp - Arduino Library
#include "Arduino.h"

// USB Communication Function Library
#include "usb_lib.h"

// Printer Movement Function Library
#include "printer_control.h"

void handle_move(String str) {
    long mtr_steps[5];
    String hot_end = str.substring(55,56);


    send_message(str);
    unsigned long parseBegin = millis();

    for(int i = 0; i<5; i++) {
      mtr_steps[i] = round(str.substring((i*11), (i*11)+10).toDouble() * (steps_per_rotations[i] / step_mode));
      Serial.println(mtr_steps[i]);
    }
    

    confirm_data();

    unsigned long calcStepsBegin = millis();

    if(new_move_command(mtr_steps,true)) request_data(1);
    else return;
}

void handle_filament_test(String str){
    double cm = str.substring(0,6).toDouble();
    int distance = str.substring(7,10).toInt();
    //Serial.println(cm,4);
    //Serial.println(distance);
    //return;
    //int seconds = str.substring(5,10).toInt();
    //Serial.println(cm, 4);
    //return;
    //Serial.println(delay_time);
    if(extrude(cm, distance)) {
      send_message("Done Extruding");
    }
}

void handle_homing(){
    if(homing_command()) {
      confirm_homing();
      request_data(1);
    }
    else stop_message("Homing Failed");
}

void handle_debug(){
    long mtr_command[5] = {0};
    Serial.read(); Serial.read(); 
    Serial.println("\nmtr num, direction, 4 digit step amount");
    Serial.println("(0-"+ String(MTR_NUMBER-1)+")  ,(0,1,-,+) ,XXXX\n");
    Serial.println("X,X,XXXX");
    while(Serial.available() == 0) {
    }
    delay(100);
    String str = Serial.readString();
    //for(int i = 0; i<8; i++) str += (char)Serial.read();
    //Serial.read();
    Serial.println(str);
    
    int mtr = str.substring(0,1).toInt();
    String direct = str.substring(2,3);
    long steps = str.substring(4,10).toInt(); // 9 for 4 digit number

    if(direct == "+" || direct == String(POS_DIRECTION[mtr])) delay(1);
    else if(direct == "-" || direct == String(NEG_DIRECTION[mtr])) steps *= -1;
    else Serial.println("UNKNOWN DIRECTION");

    
    mtr_command[mtr] = steps;
    new_move_command(mtr_command, 0);
    

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

// <!d(num)>:
void request_data(int num) {
  Serial.println("<!" + String(DATA_REQ) + "(" + String(num) + ")>");
}

// <!H>: Confirms that homing has completed.
void confirm_homing() {
  Serial.println("<!" + String(HOMING_CON) + ">");
}
