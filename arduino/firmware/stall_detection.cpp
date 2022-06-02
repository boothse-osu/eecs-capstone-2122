// Cpp - Arduino Library
#include "Arduino.h"

//
#include "config.h"

// USB Communication Function Library
#include "usb_lib.h"

// Printer Movement Function Library
// Contains PIN info
#include "printer_control.h"

// Stall Detection Function Library
#include "stall_detection.h"

// Constructor for VoltageAverage structs. Sets everything to initial 
// values
struct stallData CreateVoltageAverage() {
    struct stallData stallLog;
    stallLog.len = 0;
    stallLog.sum = 0;
    stallLog.stallTrigger = 0;
    stallLog.stallLine = 0.0;
    return stallLog;
}


// Push a new Voltage for motor number index and pass all of
// its stall data in a struct stallLog.
// Returns: True - No Stall | False - Stall
// - (note) No longer using rolling average due to stalls on some motors being
// - too gradual.
bool PushVoltage(int index, struct stallData* stallLog) {
    // If enough voltages have been pushed to have an accurate average.
    if(stallLog->len > dataPointsInStallLine[index]) {

      // Read the SLA voltage from the indexed motor driver.
      int slaVoltage = analogRead(amisSLA[index]);
      
      // If voltage is below stall line, add a trigger. Otherwise, reset
      // trigger.
      if (slaVoltage <= stallLog->stallLine) {
        stallLog->stallTrigger++;

        // If a stall has been triggered enough times in a row for the 
        // specific motor, return a false (indicating stall)
        if(stallLog->stallTrigger == stallTriggerAmount[index]){
          //Serial.println("Line: " + String(stallLog->stallLine));
          return false;
        }
      }
      else stallLog->stallTrigger = 0;
    }
    // If we are still fine-tuning the average: grab the voltage, increment
    // the length of data, and recalculate the new stall-line
    else {
      stallLog->sum += analogRead(amisSLA[index]);
      stallLog->len++;
      stallLog->stallLine = ((stallLog->sum / stallLog->len)*stallLineFraction[index])+5;
    }
    return true;
}
