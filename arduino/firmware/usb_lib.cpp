#include "Arduino.h"
#include "usb_lib.h"
#include "printer_control.h"

unsigned long timeBegin;

void parse_data() {
    timeBegin = millis();
    String str;
    int mtr_steps[5];
    String hot_end;
    Serial.read(); // (
    for(int i = 0; i<56; i++) str += (char)Serial.read();
    Serial.read(); // )
    Serial.read(); // >
    
    Serial.read(); // < (USE EVEN WHEN NOT USING EOF)
    Serial.read(); // !
    Serial.read(); // e
    Serial.read(); // <

    //send_message(str);
    unsigned long parseBegin = millis();
    send_message("Read in: " + String((double)(millis() - timeBegin)/1000.0) + " seconds");

    for(int i = 0; i<5; i++) {
        mtr_steps[i] = str.substring((i*11), (i*11)+10).toFloat() * steps_per_degree;
        //Serial.println(mtr_steps[i]);
    }
    hot_end = str[55];
    confirm_data();

    unsigned long calcStepsBegin = millis();
    send_message("Parsed in: " + String((double)(millis() - parseBegin)/1000.0) + " seconds");

    move_command(mtr_steps,true);
    // steps
    
    request_data(1);
    //send_message("Total Time: " + String((double)(millis() - timeBegin)/1000.0) + " seconds");
    Serial.println();
}

void homing_sequence(){
    confirm_homing();
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
  Serial.println("<!" + String(DATA_CON) + ">");
}
