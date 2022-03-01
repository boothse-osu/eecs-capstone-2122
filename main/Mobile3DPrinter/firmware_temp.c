#include "firmware_temp.h"
#include <stdio.h>
#include <stdlib.h>


int move_prismatic(int motor_number, float move_amount){
    printf("prismatic motor %d run with move amount %f\n", motor_number, move_amount);
    return 0;
}


int move_rotational(int motor_number, float move_amount){
    printf("rotational motor %d run with move amount %f\n", motor_number, move_amount);
    return 0;
}


int move_motors(float* move_amounts){ 
    move_prismatic(1, move_amounts[0]);
    move_prismatic(2, move_amounts[1]);
    move_prismatic(3, move_amounts[2]);
    move_rotational(4, move_amounts[3]);
    move_rotational(5, move_amounts[4]);
  
    return 0;
}


int run_homing(){
    printf("Homing run and successfully completed.\n");
    return 0;
}
