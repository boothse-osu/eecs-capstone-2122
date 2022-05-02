#include "Arduino.h"
#include "usb_lib.h"
#include "printer_control.h"
#include "stall_detection.h"

struct VoltageAverage createVoltageAverage() {
    struct VoltageAverage VAvg;
    VAvg.pos = 0;
    VAvg.len = 0;
    VAvg.sum = 0;
    VAvg.stall_trigger = 0;
    VAvg.stall_line = 0.0;
    return VAvg;
}

// record the average and set a stall line under that
// then we can ignore adding to the average again
bool pushRollingAverage(int index, struct VoltageAverage* VAvg) {
    //return true;

    if(VAvg->len > Stall_Array_Size[index]) {
      int test = analogRead(amisSLA[index]);
      //Serial.println("volt: " + String(test));
      if (test <= VAvg->stall_line) {
        VAvg->stall_trigger++;
        if(VAvg->stall_trigger == Stall_Trigger_Amt[index]) return false;
      }
      else {
        VAvg->stall_trigger = 0;
      }
    }
    else {
      VAvg->sum += analogRead(amisSLA[index]);
      VAvg->len++;
      VAvg->stall_line = ((VAvg->sum / VAvg->len)/Stall_Line_Mult[index])+5;
      //Serial.println(String(((VAvg->sum / VAvg->len)/2)+5));
    }
    return true;


/*    
    VAvg->voltages[VAvg->pos] = analogRead(amisSLA[index]);
    VAvg->sum += VAvg->voltages[VAvg->pos];
    VAvg->len++; VAvg->pos++;
    if (VAvg->pos >= Array_Size) VAvg->pos = 0;
    if (VAvg->len > Array_Size) {
        VAvg->len--;
        VAvg->sum -= VAvg->voltages[VAvg->pos];
        if(VAvg->voltages[VAvg->pos] < ((VAvg->sum / VAvg->len)/2)) return false; // testing new stall detection
    }
    //Serial.println(String(VAvg->sum / VAvg->len));
    return true;
*/
}
