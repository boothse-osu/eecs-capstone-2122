// Cpp - Arduino Library
#include "Arduino.h"

// USB Communication Function Library
#include "usb_lib.h"

// Printer Movement Function Library
// Contains PIN info
#include "printer_control.h"

// Stall Detection Function Library
#include "stall_detection.h"

// Constructor for VoltageAverage structs. Sets everything to initial 
// values
struct StallData createVoltageAverage() {
    struct StallData SData;
    SData.len = 0;
    SData.sum = 0;
    SData.stall_trigger = 0;
    SData.stall_line = 0.0;
    return SData;
}


// Push a new Voltage for motor number index and pass all of
// its stall data in a struct SData.
// Returns: True - No Stall | False - Stall
// - (note) No longer using rolling average due to stalls on some motors being
// - too gradual.
bool pushVoltage(int index, struct StallData* SData) {
    // If enough voltages have been pushed to have an accurate average.
    if(SData->len > Stall_Array_Size[index]) {

      // Read the SLA voltage from the indexed motor driver.
      int test = analogRead(amisSLA[index]);
      
      // If voltage is below stall line, add a trigger. Otherwise, reset
      // trigger.
      if (test <= SData->stall_line) {
        SData->stall_trigger++;

        // If a stall has been triggered enough times in a row for the 
        // specific motor, return a false (indicating stall)
        if(SData->stall_trigger == Stall_Trigger_Amt[index]){
          //Serial.println("Line: " + String(SData->stall_line));
          return false;
        }
      }
      else SData->stall_trigger = 0;
    }
    // If we are still fine-tuning the average: grab the voltage, increment
    // the length of data, and recalculate the new stall-line
    else {
      SData->sum += analogRead(amisSLA[index]);
      SData->len++;
      SData->stall_line = ((SData->sum / SData->len)/Stall_Line_Frac[index])+5;
    }
    return true;
}
