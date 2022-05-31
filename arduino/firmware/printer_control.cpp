// Cpp - Arduino Library
#include "Arduino.h"

//
#include "SPI.h"

// Motor Driver Library
#include "AMIS30543.h"

// USB Communication Function Library
#include "usb_lib.h"

// Printer Movement Function Library
#include "printer_control.h"

// Used to record the time between steps for each motor or each move command
float time_steps [5];
// Set extruder motor step delay
long time_steps_extruder = 1000;
// Time at which the next step should be taken for each motor
float time_nxt_step [5];

// 
// stp_cnt: 5 long array of ints for the amount of steps to move for each motor
// ht_nd: will extrude if 't' and won't if 'f'
// This move is only used to allow for commands from 5 motor only IK
// Will be all deleted when IK is updated
bool move_command(long stp_cnt[5], char ht_nd, long move_time){
  int i;

  // Might not need to be initialized here
  // Records the amount of steps taken
  long steps_taken[5] = {0, 0, 0, 0, 0};

  // Data for stall detection, may be moved to stall_detection.cpp
  //struct StallData voltage_log[MTR_NUMBER];
  //for(int i=0; i<MTR_NUMBER; i++) voltage_log[i] = createVoltageAverage();

  // Set each motor in the correct direction
  for(i = 0; i<5; i++) {
    if(stp_cnt[i] >= 0) setDirection(i,POS_DIRECTION[i]);
    else setDirection(i,NEG_DIRECTION[i]);
  }
  setDirection(extruder_pin,0);

  /*
  for(i = 0; i<5; i++){
    time_steps[i] = (move_time * 1000000) / abs(stp_cnt[i]);
  }
  */
  ///*
  long max = 0;
  for(i = 0; i<5; i++){
    if(abs(stp_cnt[i]) > max) max = abs(stp_cnt[i]);
  }
  unsigned long move_time = max*min_mtr_delay;
  for(i = 0; i<5; i++) {
    time_steps[i] = move_time / abs(stp_cnt[i]);
  }
  //*/

  // Timer that the motors will trigger off
  unsigned long timeBegin = micros();
  
  // Calculate the time of first step for each motor
  for(i = 0; i<5; i++) time_nxt_step[i] = timeBegin + time_steps[i];   
  float time_nxt_step_extruder = timeBegin + time_steps_extruder;

  // Record current time for stepping
  unsigned long timeNow = micros();

  // Set all motors to not done
  bool done_mtrs [5] = {false};
  int motors_done = 0;
  // End when all motors are done
  while(motors_done < 5){
    timeNow = micros();

    //delayMicroseconds(150);
    for(i = 0; i<5; i++){
      
      // If motor is done don't step it
      if(done_mtrs[i]==true) {}
      // If the if now finishing make it as done
      else if(steps_taken[i]==abs(stp_cnt[i])){
        //Serial.println("Done: Motor " + String(i));
        done_mtrs[i] = true;
        motors_done++;
      // If the motor is ready to step
      } else if(timeNow>=time_nxt_step[i]){
        step(i);

        // Stall detection check
        //if(abs(steps_taken[i])%Stall_Check_Step[i]==0 && pushVoltage(i, &voltage_log[i]) == false) {
        //    stop_message("Stall on motor " + String(i));
        //    return false;
        //}

        // Calculate new step trigger time
        time_nxt_step[i] += time_steps[i];
        steps_taken[i]++;
      }
    }
    // If the Extruder needs to step: step it an calculate next step time
    if(ht_nd == 'e' && timeNow>=time_nxt_step_extruder){
      step(extruder_pin);
      time_nxt_step_extruder += time_steps_extruder;
    }
  }
    
  //send_message("Done in "+String((double)(millis()-start_time)/1000.0)+" seconds");
  //for(int i = 0; i<5; i++) send_message("MTR "+String(i)+" steps taken: " + String(steps_taken[i]));
  //for(int i = 0; i<5; i++) send_message("MTR "+String(i)+" microseconds per step: " + String(time_steps[i]));
  return true;
}


bool print_move_command(long stp_cnt[5], long extrude_delay){
  // timer for feedback on how long this function ran
  unsigned long start_time = millis();

  // Might not need to be initialized here
  // Records the amount of steps taken
  long steps_taken[5] = {0, 0, 0, 0, 0};

  // Data for stall detection, may be moved to stall_detection.cpp
  struct StallData voltage_log[MTR_NUMBER];
  for(int i=0; i<MTR_NUMBER; i++) voltage_log[i] = createVoltageAverage();

  // Set each motor in the correct direction
  for(int i = 0; i<5; i++) {
    if(stp_cnt[i] >= 0) setDirection(i,POS_DIRECTION[i]);
    else setDirection(i,NEG_DIRECTION[i]);
  }

  // Time between steps on a specific motor
  // Should add a case where: if a time_step[i] is less than 150, add
  // time to mvmt_time and recalculate. This will avoid missing steps
  long added_time = 0;
  bool mtr_ready = false;
  while(!mtr_ready) {
    mtr_ready = true;
    for(int i = 0; i<5; i++) {
      if(stp_cnt[i] == 0) time_steps[i] = 0;
      else{
        time_steps[i] = (mvmt_time + added_time) / abs(stp_cnt[i]);
        if(time_steps[i]<min_mtr_delay) {
          added_time += 5000; // half sec
          mtr_ready = false;
        }
      }
    }
  }


  // Set extrusion to correct direction
  if (extrude_delay>0)setDirection(extruder_pin,0);
  else setDirection(extruder_pin,1);
  // Set motor step delay
  long time_steps_extruder = abs(extrude_delay);


  // Timer that the motors will trigger off
  unsigned long timeBegin = micros();
  
  // Time at which the next step should be taken for each motor
  float time_nxt_step [5] = {0, 0, 0, 0, 0};
  // Calculate the time of first step for each motor
  for(int i = 0; i<5; i++) time_nxt_step[i] += timeBegin + time_steps[i];
  float time_nxt_step_extruder = timeBegin + time_steps_extruder;
  

  send_message("Starting move command");

  // SLA Log for testing
  //signed int list_sla [abs(stp_cnt[1])] = {0};

  int i;
  // Record current time for stepping
  unsigned long timeNow = micros();

  // Set all motors to not done
  bool done_mtrs [5] = {false};
  int motors_done = 0;
  // End when all motors are done
  while(motors_done < 5){
    timeNow = micros();

    // Step on each of the joint motors if needed
    delayMicroseconds(200);
    for(i = 0; i<5; i++){
      
      // If motor is done don't step it
      if(done_mtrs[i]==true) {}
      // If the if now finishing make it as done
      else if(steps_taken[i]==abs(stp_cnt[i])){
        //Serial.println("Done: Motor " + String(i));
        done_mtrs[i] = true;
        motors_done++;
      // If the motor is ready to step
      } else if(timeNow>=time_nxt_step[i]){
        step(i);
        
        //if(i==1 && abs(steps_taken[i])%1==0) list_sla[abs(steps_taken[i])] = analogRead(amisSLA[i]); // mtr_ready = false;//

        // Stall detection check
        //maybe remove abs from modulo
        //if(abs(steps_taken[i])%Stall_Check_Step[i]==0 && pushVoltage(i, &voltage_log[i]) == false) {
        //    stop_message("Stall on motor " + String(i));
        //    return false;
        //}

        // Calculate new step trigger time
        time_nxt_step[i] += time_steps[i];
        steps_taken[i]++;
      }
    }
    // If the Extruder needs to step: step it an calculate next step time
    if(timeNow>=time_nxt_step_extruder && extrude_delay != 0){
      step(extruder_pin);
      time_nxt_step_extruder += time_steps_extruder;
    }
  }

  // Log gathered SLA data
  //for(int i=0; i<abs(stp_cnt[1]); i++) Serial.println(list_sla[i]);
  
  // Log information about movement for debugging
  send_message("Done in "+String((double)(millis()-start_time)/1000.0)+" seconds");
  for(int i = 0; i<5; i++) send_message("MTR "+String(i)+" steps taken: " + String(steps_taken[i]));
  for(int i = 0; i<5; i++) send_message("MTR "+String(i)+" microseconds per step: " + String(time_steps[i]));
  return true;
}

bool homing_command(){
    //return true;
    struct StallData voltage_log[MTR_NUMBER];
    for(int i=0; i<MTR_NUMBER; i++) voltage_log[i] = createVoltageAverage();


    bool mtr_running [5] = {true, true, false, false, false};
    int mtrs_done = 0;
    
    // Set all motors to their homing direction
    for(int i = 0; i<5; i++) setDirection(i,HOME_DIRECTION[i]);

    // Home the X and Y motors at the same time
    unsigned long time_nxt_step = micros() + 350;
    send_message("Starting Homing");
    while(true) {
      if(micros()>time_nxt_step){
        time_nxt_step = micros() + 350;
        for(int i = 0; i<2; i++) {
          if(mtr_running[i]) {
            step(i);
            mtr_running[i] = pushVoltage(i, &voltage_log[i]);
            if(mtr_running[i]==false) {
              send_message("Motor " + String(i) + " at home");
              mtrs_done++;
            }
          }
        }
        if(mtrs_done==2) break;
      }
    }

    // Home the rotational motors now that the filament tube is in a
    // safe location
    time_nxt_step = micros() + 7500;
    while(true) {
      if(micros()>time_nxt_step){
        time_nxt_step = micros() + 7500;
        step(3);
        mtr_running[3] = pushVoltage(3, &voltage_log[3]);
        if(mtr_running[3]==false) break;
      }
    }
    send_message("Motor 3 at home");
    
    // Prompt motors done for deactivated motors
    send_message("Motor 4 at home");
    send_message("Motor 2 at home");

  return true;
}

// Temporary Test Function
bool extrude(double cms, int cm){
    int extrude_mtr_index = extruder_pin;

    int cm_step_amount = 461;
    int delay_time = 1000000/(cm_step_amount*cms);

    if (cm>0)setDirection(extrude_mtr_index,0);
    else setDirection(extrude_mtr_index,1);
    
    int cm_target_number = cms;
    int target = cm_step_amount * abs(cm);
    int steps = 0;
    unsigned long time_nxt_step = micros() + 300;
    while(true) {
      if(micros()>time_nxt_step){
        time_nxt_step = micros() + delay_time;
        step(extrude_mtr_index);
        steps++;
        if(steps==target) return true;
      }
    }
}

// Sends a pulse on the NXT/STEP pin to tell the driver to take
// one step, and also delays to control the speed of the motor.
void step(int sel)
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
void setDirection(int sel, bool dir)
{
  // The NXT/STEP pin must not change for at least 0.5
  // microseconds before and after changing the DIR pin.
  delayMicroseconds(1);
  digitalWrite(amisDirPin[sel], dir);
  delayMicroseconds(1);
}
