#include "Arduino.h"
#include "usb_lib.h"
#include "printer_control.h"
#include "stall_detection.h"

//#include <stdio.h>

struct VoltageAverage createVoltageAverage(void) {
    struct VoltageAverage VAvg;
    VAvg.pos = 0;
    VAvg.len = 0;
    VAvg.sum = 0;
    return VAvg;
}

struct MotorData createMotorData(void) {
    struct MotorData data;
    for(int i = 0; i < Motor_Number; i++) {
        data.MotorAverages[i] = createVoltageAverage();
    }
    return data;
}

void startUp() {
    int gains [] = {1,0,1,1,1};
    set_SLA_gain(gains);
    int transparency_off [] = {1,0,1,1,1};
    set_SLA_transparency_off(transparency_off);
    int transparency_on [] = {0,1,0,0,0};
    set_SLA_transparency_on(transparency_on);
}

double pushRollingAverage(int volt, struct VoltageAverage* VAvg) {
    VAvg->voltages[VAvg->pos] = volt;
    VAvg->sum += volt;
    VAvg->len++; VAvg->pos++;
    if (VAvg->pos >= Array_Size)
        VAvg->pos = 0;
    if (VAvg->len > Array_Size) {
        VAvg->len--;
        VAvg->sum -= VAvg->voltages[VAvg->pos];
    }
    return (double) VAvg->sum / VAvg->len;
}

int runHomingSequence_c(int time, struct testPINS PINS, struct MotorData* MData){
    double averages[Motor_Number];
    int motors [Motor_Number] = {0};
    int motor;
    for (time = 0; time < 600; time++) {

        for (motor = 0; motor < Motor_Number; motor++) {
            averages[motor] = pushRollingAverage(read_analog(PINS.PIN[motor], time), &MData->MotorAverages[motor]);
            if(averages[motor] < Stall_Line)
                motors[motor] = 1;
        }
        int MSum = 0;
        for (int i = 0; i < Motor_Number; i++)
            MSum += motors[i];
        if(MSum == 5) {
            printf("\nAverage: [    HOMED,     HOMED,     HOMED,     HOMED,     HOMED, ]  Time: %d", time);
            printf("\nHoming Complete.\n");
            return 0;
        }
        if (time % Output_Time == 0) {
            printf("\nAverage: [");
            for(motor = 0; motor < Motor_Number; motor++) {
                if (motors[motor] == 1)
                    printf("    HOMED, ");
                else
                    printf("%f, ", averages[motor]);
            }
            printf("]  Time: %d", time);
        }
    }
    printf("\nHoming was never completed.\n");
    return 1;
}

int runPrintSequence_c(int time, int data_list [Data_Length]){
    int id, motor;
    double average;
    int motors [] = {0};
    struct MotorData PrintingData = createMotorData();

    for (time = 0; time < 600; time++) {
        average = pushRollingAverage(read_analog(data_list, time), &PrintingData.MotorAverages[0]);

        if (average < Stall_Line) {
            printf("%f\n", average);
            return 1;
        }

        if (time % Output_Time == 0) {
            printf("%f\n", average);
        }
    }
    printf("No Stall.\n0\n\n");

    return 0;
}
