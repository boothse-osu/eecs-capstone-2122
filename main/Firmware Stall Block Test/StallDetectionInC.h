//
// Created by Lewis on 1/20/2022.
//

#ifndef CAPSTONE_FIRMWARE_STALLDETECTIONINC_H
#define CAPSTONE_FIRMWARE_STALLDETECTIONINC_H

#define Array_Size 20
#define Motor_Number 5
#define Output_Time 100
#define Data_Length 600
#define Stall_Line 20

struct testPINS {
    int PIN [Motor_Number][Data_Length];
};

struct VoltageAverage {
    int pos, len, sum;
    int voltages[Array_Size];
};

struct MotorData {
    struct VoltageAverage MotorAverages[Motor_Number];
};

struct VoltageAverage createVoltageAverage(void);
struct MotorData createMotorData(void);
void startUp();
double pushRollingAverage(int, struct VoltageAverage*);
int runHomingSequence_c(int, struct testPINS, struct MotorData*);
int runPrintSequence_c(int, int [Data_Length]);


#endif //CAPSTONE_FIRMWARE_STALLDETECTIONINC_H
