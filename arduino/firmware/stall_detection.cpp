#include "Arduino.h"
#include "usb_lib.h"
#include "printer_control.h"
#include "stall_detection.h"


struct VoltageAverage createVoltageAverage() {
    struct VoltageAverage VAvg;
    VAvg.pos = 0;
    VAvg.len = 0;
    VAvg.sum = 0;
    return VAvg;
}



bool pushRollingAverage(int volt, struct VoltageAverage* VAvg) {
    return true;
    VAvg->voltages[VAvg->pos] = volt;
    VAvg->sum += volt;
    VAvg->len++; VAvg->pos++;
    if (VAvg->pos >= Array_Size) VAvg->pos = 0;
    if (VAvg->len > Array_Size) {
        VAvg->len--;
        VAvg->sum -= VAvg->voltages[VAvg->pos];
        if(volt < ((VAvg->sum / VAvg->len)/2)) return false; // testing new stall detection
    }
    //Serial.println(String(VAvg->sum / VAvg->len));
    return true;
}
