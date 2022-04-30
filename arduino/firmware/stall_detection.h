#ifndef STALL
#define STALL

#include "Arduino.h"

#define Array_Size 10
#define MTR_NUMBER 5
#define Output_Time 5
#define Data_Length 50
#define Stall_Line 10

#define Step_Mode 0.25
#define Full_Rotation_Steps (int)(400 / Step_Mode)
#define Distance_To_Step 60
#define Motor_Pins {13,14,15,16,17}

struct VoltageAverage {
    int pos, len, sum;
    int voltages[Array_Size] = {Stall_Line*5};
};



struct MotorData {
    struct VoltageAverage MotorAverages[MTR_NUMBER];
};

struct VoltageAverage createVoltageAverage();
struct MotorData createMotorData(void);
bool pushRollingAverage(int, struct VoltageAverage*);
int runHomingSequence_c(int, struct testPINS, struct MotorData*);
int runPrintSequence_c(int, int [Data_Length]);
int read_analog(int*, int); // get the SLA value for stall block

#endif
