#include "Arduino.h"
#include "usb_lib.h"
#include "printer_control.h"

unsigned long timeBegin;

void parse_data(String str) {
    timeBegin = millis();
    //String str;
    long mtr_steps[5];
    String hot_end;
    //Serial.read(); // (
    //for(int i = 0; i<56; i++) str += (char)Serial.read();
    //Serial.read(); // )
    //Serial.read(); // >
    
    //Serial.read(); // < (USE EVEN WHEN NOT USING EOF)
    //Serial.read(); // !
    //Serial.read(); // e
    //Serial.read(); // <

    send_message(str);
    unsigned long parseBegin = millis();
    //send_message("Read in: " + String((double)(millis() - timeBegin)/1000.0) + " seconds");

    for(int i = 0; i<5; i++) {
      mtr_steps[i] = round(str.substring((i*11), (i*11)+10).toDouble() * steps_per_x);
      Serial.println(mtr_steps[i]);
    }
    
    hot_end = str[55];
    confirm_data();

    unsigned long calcStepsBegin = millis();
    //send_message("Parsed in: " + String((double)(millis() - parseBegin)/1000.0) + " seconds");

    if(new_move_command(mtr_steps,true)) request_data(1);
    else return;
    // steps
    
    //send_message("Total Time: " + String((double)(millis() - timeBegin)/1000.0) + " seconds");
    //Serial.println();
}

void homing_sequence(){
    if(homing_command()) {
      confirm_homing();
      request_data(1);
    }
    else stop_message("Homing Failed");
}


void debug_mode(){
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
    long steps = str.substring(4,9).toInt();

    if(direct == "+" || direct == String(POS_DIRECTION[mtr])) delay(1);
    else if(direct == "-" || direct == String(NEG_DIRECTION[mtr])) steps *= -1;
    else Serial.println("UNKNOWN DIRECTION");

    
    mtr_command[mtr] = steps;
    new_move_command(mtr_command, 0);
    

}

void send_message(String msg) {
  Serial.println("<!" + String(MESSAGE) + "("  + msg + ")>");
}

void stop_message(String msg) {
  Serial.println("<!" + String(STOP) + "("  + msg + ")>");
}

void confirm_data() {
  Serial.println("<!" + String(DATA_CON) +  ">");
}

void request_data(int num) {
  Serial.println("<!" + String(DATA_REQ) + "(" + String(num) + ")>");
}

void confirm_homing() {
  Serial.println("<!" + String(HOMING_CON) + ">");
}
