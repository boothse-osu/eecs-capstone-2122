//
// Created by Lewis on 2/26/2022.
//

#ifndef CAPSTONE_FIRMWARE_FIRMWAREINC_H
#define CAPSTONE_FIRMWARE_FIRMWAREINC_H

#define Step_Mode 0.25
#define Full_Rotation_Steps (int)(400 / Step_Mode)
#define Distance_To_Step 1
#define Motor_Pins {13,14,15,16,17}

int move_prismatic(int, float); // translate step amount and call stepper
int move_rotational(int, float); // translate step amount and call stepper
int move_motors(float*); // move all motors with correct joint calls
int run_homing(); // run the homing sequence

int stepper(int, int); // move the motor while calling stall detection

void set_SLA_gain(const int*); // allow stall block to set pin values as needed
void set_SLA_transparency_off(const int*); // allow stall block to set pin values as needed
void set_SLA_transparency_on(const int*); // allow stall block to set pin values as needed

int read_analog(int*, int); // get the SLA value for stall block

#endif //CAPSTONE_FIRMWARE_FIRMWAREINC_H
