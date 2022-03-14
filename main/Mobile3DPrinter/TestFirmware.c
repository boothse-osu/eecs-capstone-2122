#include "StallDetectionInC.h"
#include "FirmwareInC.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int homingCompletion, t;
    struct testPINS PINS;

    srand(time(0));                               // NOLINT(cert-msc51-cpp)

    printf("Test Start:\n");
    startUp();

    //////////////////////////////////////Homing/////////////////////////////////////////

    printf("\nHoming Sequence with Gain Set Correctly by Stall Detection:");
    homingCompletion = run_homing();
    printf("%d\n", homingCompletion);


    printf("\nHoming Sequence with Gain Not Set");
    for (int i = 0; i < Motor_Number; i++) {
        int k = rand() % Data_Length;                       // NOLINT(cert-msc50-cpp)
        for (int j = 0; j < k; j++)
            PINS.PIN[i][j] = (rand() % 50) + 120;           // NOLINT(cert-msc50-cpp)
        for (int h = k; h < Data_Length; h++)
            PINS.PIN[i][h] = (rand() % 40) + 10;            // NOLINT(cert-msc50-cpp)
    }
    struct MotorData HomingGainData = createMotorData();
    homingCompletion = runHomingSequence_c(t, PINS, &HomingGainData);
    printf("%d\n\n", homingCompletion);

    //////////////////////////////////////Printing/////////////////////////////////////////

    float moves [] = {-10,10,0, 20, -20};
    int motor_stall_status = move_motors(moves);
}