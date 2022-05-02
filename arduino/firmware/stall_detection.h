#ifndef STALL
#define STALL

#include "Arduino.h"

#define Array_Size 20
#define MTR_NUMBER 5
#define Output_Time 5

#define Data_Length 50
//#define Stall_Line 10

#define Step_Mode 0.25
#define Full_Rotation_Steps (int)(400 / Step_Mode)
#define Distance_To_Step 60
#define Motor_Pins {13,14,15,16,17}

const int Stall_Array_Size[5]   =  {10,   10,   10,   10,   10};
const int Stall_Check_Step[5]   =  {5,    5,    5,    2,    1};
const int Stall_Trigger_Amt[5]  =  {2,    2,    2,    2,    3};
const float Stall_Line_Mult[5]  =  {2,    2,    2,    2,    1.75};


struct VoltageAverage {
    int pos, len, sum, stall_trigger;
    float stall_line;
    int voltages[Array_Size] = {0};
};

struct VoltageAverage createVoltageAverage();
bool pushRollingAverage(int, struct VoltageAverage*);

#endif
