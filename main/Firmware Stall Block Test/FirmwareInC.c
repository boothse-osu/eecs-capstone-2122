//
// Created by Lewis on 2/26/2022.
// Code used for Verfication only
//

#include "FirmwareInC.h"
#include "StallDetectionInC.h"
#include <stdio.h>
#include <stdlib.h>

// translate step amount and call stepper for simple prismatic joints
int move_prismatic(int motor_number, float move_amount){
    int step_amount;
    // get pins associated with motor drivers
    int PINS [] = Motor_Pins;
    // calculate steps needed to travel distance given joint type and step mode
    step_amount = (int)(move_amount * Distance_To_Step);
    printf("Stepping %d times on prismatic motor %d.\n", step_amount, motor_number+1);
    int stall_trigger = stepper(PINS[motor_number], step_amount);
    if(stall_trigger == 1){
        printf("Stall on motor %d\n%d\n\n", motor_number+1, motor_number+1);
        return motor_number+1;
    }
    return 0;
}

// translate step amount and call stepper for simple rotational joints
int move_rotational(int motor_number, float move_amount){
    int step_amount;
    // get pins associated with motor drivers
    int PINS [] = Motor_Pins;
    // TODO math for rotation conversion
    float rotation_to_step = (float)360.0 / Full_Rotation_Steps;
    step_amount = (int)(move_amount * rotation_to_step);
    printf("Stepping %d times on rotational motor %d.\n", step_amount, motor_number+1);
    int stall_trigger = stepper(PINS[motor_number], step_amount);
    if(stall_trigger == 1){
        printf("Stall on motor %d\n%d\n\n", motor_number+1, motor_number+1);
        return motor_number+1;
    }
    return 0;
}

// move all motors with correct joint calls
int move_motors(float* move_amounts){
    int stall_trigger;
    for(int i = 0; i < 3; i++){
        stall_trigger = move_prismatic(i, move_amounts[i]);
        if(stall_trigger != 0)
            return stall_trigger;
    }
    for(int i = 3; i < 5; i++){
        stall_trigger = move_rotational(i, move_amounts[i]);
        if(stall_trigger != 0)
            return stall_trigger;
    }
    return 0;
}

// run the homing sequence
int run_homing(){
    struct testPINS PIN_list;
    int t = 0;
    for (int i = 0; i < Motor_Number; i++) {
        int k = rand() % Data_Length;                       // NOLINT(cert-msc50-cpp)
        for(int j = 0; j < k; j++)
            PIN_list.PIN[i][j] = (rand() % 30) + 60;        // NOLINT(cert-msc50-cpp)
        for(int h = k; h < Data_Length; h++)
            PIN_list.PIN[i][h] = rand() % 20;               // NOLINT(cert-msc50-cpp)
    }
    struct MotorData HomingData = createMotorData();
    return runHomingSequence_c(t, PIN_list, &HomingData);
}

// move the motor while calling stall detection
int stepper(int motor_pin, int step_amount){
    int data_list [Data_Length];
    int t = 0;
    int k = rand() % Data_Length;                           // NOLINT(cert-msc50-cpp)
    for(int j = 0; j < k; j++)
        data_list[j] = (rand() % 40) + 70;                  // NOLINT(cert-msc50-cpp)
    for(int h = k; h < Data_Length; h++)
        data_list[h] = rand() % 20;                         // NOLINT(cert-msc50-cpp)

    int stall_trigger = runPrintSequence_c(t, data_list);
    if (stall_trigger == 1)
        return 1;
    return 0;

}

//Clears the SLAG, setting the SLA pin gain to 0.5
//This can be lowered to 0.25 but moving at low voltages
//becomes similar to at rest
void set_SLA_gain(const int* motor_driver_nums){
    int PINS [] = Motor_Pins;
    for(int i = 0; i < Motor_Number; i++){
        if(motor_driver_nums[i] == 1){
            //stepper.setSlaGainDefault(pin)	 //Arduino Code
            printf("SLA Gain set to 0.5, on pin %d\n", PINS[i]);
        } else {
            printf("SLA Gain not set on pin %d\n", PINS[i]);
        }
    }
}

// allow stall block to set pin values as needed
void set_SLA_transparency_off(const int* motor_driver_nums){
    int PINS [] = Motor_Pins;
    for(int i = 0; i < Motor_Number; i++){
        if(motor_driver_nums[i] == 1){
            //stepper.setSlaTransparencyOff(pin) //Arduino Code
            printf("SLA PIN transparency set to off, on pin %d\n", PINS[i]);
        } else {
            //printf("SLA PIN transparency not set on pin %d\n", PINS[i]);
        }
    }
}

// allow stall block to set pin values as needed
void set_SLA_transparency_on(const int* motor_driver_nums){
    int PINS [] = Motor_Pins;
    for(int i = 0; i < Motor_Number; i++){
        if(motor_driver_nums[i] == 1){
            //stepper.setSlaTransparencyOn(pin) //Arduino Code
            printf("SLA PIN transparency set to on, on pin %d\n", PINS[i]);
        } else {
            //printf("SLA PIN transparency not set on pin %d\n", PINS[i]);
        }
    }
}

// get the SLA value for stall block
int read_analog(int* voltage_data, int driver_sla_num){
    int PINS [] = Motor_Pins;
    if(driver_sla_num < 5) {
        int pin = PINS[driver_sla_num];
    }
    return voltage_data[driver_sla_num];
    //return readAnalog(pin) //Arduino Code: 0-1023
}
