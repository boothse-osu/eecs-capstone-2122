//
// Will not intorface correctly
// test for example of arduino code
//

#include "StallDetectionInC.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main() {
    printf("Start of Test File\n");

    int t, motor;
    double averages[Motor_Number];
    int pins [] = {5,6,7,8,9};

    int homingCompletion;
    int stallID;

    srand(time(0));

    printf( "Test Start:\n");
    startUp(pins, 1, 2);

    //////////////////////////////////////Realistic/////////////////////////////////////////

    printf("\nRealistic data for default gain:");

    struct testPINS PINS;
    for (int i = 0; i < Motor_Number; i++) {
        int k = rand() % Data_Length;
        for(int j = 0; j < k; j++)
            PINS.PIN[i][j] = (rand() % 30) + 60;
        for(int h = k; h < Data_Length; h++)
            PINS.PIN[i][h] = rand() % 20;
    }
    printf("\nHoming Sequence with Realistic Data:");
    struct MotorData HomingData = createMotorData();
    homingCompletion = runHomingSequence_c(t, PINS, &HomingData);
    printf("%d\n", homingCompletion);


    for (int i = 0; i < Motor_Number; i++) {
        int k = rand() % Data_Length;
        for(int j = 0; j < k; j++)
            PINS.PIN[i][j] = (rand() % 40) + 70;
        for(int h = k; h < Data_Length; h++)
            PINS.PIN[i][h] = rand() % 20;
    }
    printf("\nPrinting Sequence with Realistic Data:");
    struct MotorData PrintingData = createMotorData();
    stallID = runPrintSequence_c(t, PINS, &PrintingData);
    printf("%d\n", stallID);
    if(stallID != 0)
        printf("Motor on PIN_NUM: %d\n", pins[stallID-1]);

    //////////////////////////////////////Realistic/////////////////////////////////////////

    printf("\nRealistic data for gain of .25:");
    printf("\nHoming Sequence with Realistic Data:");
    for (int i = 0; i < Motor_Number; i++) {
        int k = rand() % Data_Length;
        for(int j = 0; j < k; j++)
            PINS.PIN[i][j] = (rand() % 40) + 25;
        for(int h = k; h < Data_Length; h++)
            PINS.PIN[i][h] = rand() % 20;
    }
    struct MotorData HomingGainData = createMotorData();
    homingCompletion = runHomingSequence_c(t, PINS, &HomingGainData);
    printf("%d\n", homingCompletion);

    printf("\nPrinting Sequence with Realistic Data:");
    for (int i = 0; i < Motor_Number; i++) {
        int k = rand() % Data_Length;
        for(int j = 0; j < k; j++)
            PINS.PIN[i][j] = (rand() % 30) + 10;
        for(int h = k; h < Data_Length; h++)
            PINS.PIN[i][h] = rand() % 20;
    }
    struct MotorData PrintingGainData = createMotorData();
    stallID = runPrintSequence_c(t, PINS, &PrintingGainData);
    printf("%d\n", stallID);
    if(stallID != 0)
        printf("Motor on PIN_NUM: %d\n", pins[stallID-1]);

    ////////////////////////////////////////High////////////////////////////////////////////

    printf("\n\nHoming Sequence with  Max Voltage Data:");
    for (int i = 0; i < Motor_Number; i++) {
        for(int j = 0; j < Data_Length; j++)
            PINS.PIN[i][j] = 1023;
    }
    struct MotorData HomingHighData = createMotorData();
    homingCompletion = runHomingSequence_c(t, PINS, &HomingHighData);
    printf("%d\n", homingCompletion);

    printf("\n\nPrinting Sequence with Max Voltage Data:");
    struct MotorData PrintingHighData = createMotorData();
    stallID = runPrintSequence_c(t, PINS, &PrintingHighData);
    printf("%d\n", stallID);
    if(stallID != 0)
        printf("Motor on PIN_NUM: %d\n", pins[stallID-1]);

    ///////////////////////////////////////Low//////////////////////////////////////////////

    printf("\n\nHoming Sequence with  Min Voltage Data:");
    for (int i = 0; i < Motor_Number; i++) {
        for(int j = 0; j < Data_Length; j++)
            PINS.PIN[i][j] = 0;
    }
    struct MotorData HomingLowData = createMotorData();
    homingCompletion = runHomingSequence_c(t, PINS, &HomingLowData);
    printf("%d\n", homingCompletion);

    printf("\nPrinting Sequence with Min Voltage Data:");
    struct MotorData PrintingLowData = createMotorData();
    stallID = runPrintSequence_c(t, PINS, &PrintingLowData);
    printf("%d\n", stallID);
    if(stallID != 0)
        printf("Motor on PIN_NUM: %d\n", pins[stallID-1]);

    return 0;
}
