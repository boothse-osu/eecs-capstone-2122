// Cpp - Arduino Library
#include "Arduino.h"

//
#include "SPI.h"

// Motor Driver Library
#include "AMIS30543.h"

//
#include "config.h"

// USB Communication Function Library
#include "usb_lib.h"

// Printer Movement Function Library
#include "printer_control.h"

// Used to record the time between steps for each motor or each move command
float motorDelays [5];
// Set extruder motor step delay

// Time at which the next step should be taken for each motor
float nextStepTimes [5];

unsigned long currentTime;

int i;

unsigned long extruderDelay = 1000;

// motorStepAmounts: 5 long array of ints for the amount of steps to move for each motor
// isHotEndOn: will extrude if 't' and won't if 'f'
bool MoveCommand(long motorStepAmounts[5], char isHotEndOn, long movementTime){
  // Data for stall detection, may be moved to stall_detection.cpp
  //struct stallData slaVoltageLog[MTR_NUMBER];
  //for(int i=0; i<MTR_NUMBER; i++) slaVoltageLog[i] = CreateVoltageAverage();


  // Set each motor in the correct direction
  // Could track if motor is already set in correct location but this code is fast anyways
  for(i = 0; i<5; i++) {
    if(motorStepAmounts[i] >= 0 ) SetDirection(i,POS_DIRECTION[i]);
    else SetDirection(i,NEG_DIRECTION[i]);
  }
  SetDirection(EXTRUDER_PIN,0);


  /*
  //SendMessage(String(movementTime));
  for(i = 0; i<5; i++){
    motorDelays[i] = movementTime / abs(motorStepAmounts[i]);
    //SendMessage(String(motorDelays[i]));
  }
  */
  /*
  for(int k = 0; k<5; k++){
    if((motorDelays[k] != 0 && motorDelays[k] < minMotorDelay) || isHotEndOn == 'f'){ 
  */
      long largestStepAmount = 0;
      for(i = 0; i<5; i++){
        if(abs(motorStepAmounts[i]) > largestStepAmount) {
          largestStepAmount = abs(motorStepAmounts[i]);
          //if(i == 1) largestStepAmount += 1000;
        }
        
      }
      movementTime = largestStepAmount*minMotorDelay;
      for(i = 0; i<5; i++) {
        motorDelays[i] = movementTime / abs(motorStepAmounts[i]);
        Serial.println(motorDelays[i]);
      }
  /*
      break;
    }
  }
  */
  
  // Calculate the time of first step for each motor
  for(i = 0; i<5; i++) nextStepTimes[i] = micros() + motorDelays[i];   
  float nextExtruderStepTime = micros() + extruderDelay;


  // Might not need to be initialized here
  // Records the amount of steps taken
  long stepsTaken[5] = {0, 0, 0, 0, 0};

  // Set all motors to not done
  bool doneMotorList [5] = {false};
  int doneMotorNum = 0;
  // End when all motors are done
  while(doneMotorNum < 5){
    currentTime = micros();

    //delayMicroseconds(150);
    for(i = 0; i<5; i++){
      
      // If motor is done don't step it
      if(doneMotorList[i]==true) {}
      // If the if now finishing make it as done
      else if(stepsTaken[i]==abs(motorStepAmounts[i])){
        //Serial.println("Done: Motor " + String(i));
        doneMotorList[i] = true;
        doneMotorNum++;
      // If the motor is ready to step
      } else if(currentTime>=nextStepTimes[i]){
        Step(i);

        // Stall detection check
        //if(abs(stepsTaken[i])%stallCheckStepInterval[i]==0 && PushVoltage(i, &slaVoltageLog[i]) == false) {
        //    SendStopMessage("Stall on motor " + String(i));
        //    return false;
        //}

        // Calculate new step trigger time
        nextStepTimes[i] += motorDelays[i];
        stepsTaken[i]++;
      }
    }
    // If the Extruder needs to step: step it an calculate next step time
    if(isHotEndOn == 't' && currentTime>=nextExtruderStepTime){
      Step(EXTRUDER_PIN);
      nextExtruderStepTime += extruderDelay;
    }
  }
    
  return true;
}


bool HomingCommand(){
    // This function is very odd, mainly due to a z motor that is too strong to stall
    // and two rotational motors that are weak enough to stall if the filament tube sags

    //return true;
    struct stallData slaVoltageLog[MTR_NUMBER];
    for(int i=0; i<MTR_NUMBER; i++) slaVoltageLog[i] = CreateVoltageAverage();


    bool motorsRunning [5] = {true, true, false, false, false};
    int doneMotorNum = 0;
    
    // Set all motors to their homing direction
    for(int i = 0; i<5; i++) SetDirection(i,NEG_DIRECTION[i]);

    // Home the X and Y motors at the same time
    unsigned long nextStepTime = micros() + 350;
    SendMessage("Starting Homing");
    while(true) {
      if(micros()>nextStepTime){
        nextStepTime = micros() + 350;
        for(int i = 0; i<2; i++) {
          if(motorsRunning[i]) {
            Step(i);
            motorsRunning[i] = PushVoltage(i, &slaVoltageLog[i]);
            if(motorsRunning[i]==false) {
              SendMessage("Motor " + String(i) + " at home");
              doneMotorNum++;
            }
          }
        }
        if(doneMotorNum==2) break;
      }
    }

    // Home the rotational motors now that the filament tube is in a
    // safe location
    nextStepTime = micros() + 7500;
    while(true) {
      if(micros()>nextStepTime){
        nextStepTime = micros() + 7500;
        Step(3);
        motorsRunning[3] = PushVoltage(3, &slaVoltageLog[3]);
        if(motorsRunning[3]==false) break;
      }
    }
    SendMessage("Motor 3 at home");
    
    // Prompt motors done for deactivated motors
    SendMessage("Motor 4 at home");
    SendMessage("Motor 2 at home");

  return true;
}

// Temporary Test Function
bool ExtrudeCommand(double cmPerSecond, int cm){
    int cmExtrudeAmount = 461;
    long extruderDelay = 1000000/(cmExtrudeAmount*cmPerSecond);

    if (cm>0)SetDirection(EXTRUDER_PIN,0);
    else SetDirection(EXTRUDER_PIN,1);
    
    int cm_target_number = cmPerSecond;
    int target = cmExtrudeAmount * abs(cm);
    int steps = 0;
    unsigned long nextStepTime = micros() + 300;
    while(true) {
      if(micros()>nextStepTime){
        nextStepTime = micros() + extruderDelay;
        Step(EXTRUDER_PIN);
        steps++;
        if(steps==target) return true;
      }
    }
}

// Sends a pulse on the NXT/STEP pin to tell the driver to take
// one step, and also delays to control the speed of the motor.
void Step(int sel)
{
  // The NXT/STEP minimum high pulse width is 2 microseconds.
  digitalWrite(amisStepPin[sel], HIGH);
  delayMicroseconds(3);
  digitalWrite(amisStepPin[sel], LOW);
  delayMicroseconds(3);

  // The delay here controls the stepper motor's speed.  You can
  // increase the delay to make the stepper motor go slower.  If
  // you decrease the delay, the stepper motor will go fast, but
  // there is a limit to how fast it can go before it starts
  // missing steps.
  // delayMicroseconds(200);
}

// Writes a high or low value to the direction pin to specify
// what direction to turn the motor.
void SetDirection(int sel, bool dir)
{
  // The NXT/STEP pin must not change for at least 0.5
  // microseconds before and after changing the DIR pin.
  delayMicroseconds(1);
  digitalWrite(amisDirPin[sel], dir);
  delayMicroseconds(1);
}
