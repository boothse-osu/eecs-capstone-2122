#include "Arduino.h"

#include "SPI.h"
#include "AMIS30543.h"

#include "usb_lib.h"
#include "printer_control.h"

// Used to record the time between steps for each motor or each move command
float time_steps [5];

// 
// stp_cnt: 5 long array of ints for the amount of steps to move for each motor
// ht_nd: UNUSED AT THIS POINT
bool new_move_command(long stp_cnt[5], bool ht_nd){
    // timer for feedback on how long this function ran
    unsigned long start_time = millis();

    // Might not need to be initialized here
    // Records the amount of steps taken
    long steps_taken[5] = {0, 0, 0, 0, 0};

    // Data for stall detection, may be moved to stall_detection.cpp
    struct VoltageAverage voltage_log[MTR_NUMBER];
    for(int i=0; i<MTR_NUMBER; i++) voltage_log[i] = createVoltageAverage();

    // Set each motor in the correct direction
    for(int i = 0; i<5; i++) {
      if(stp_cnt[i] >= 0) setDirection(i,POS_DIRECTION[i]);
      else setDirection(i,NEG_DIRECTION[i]);
    }

    // Time inbetween steps on a specific motor
    // Should add a case where: if a time_step[i] is less than 150, add
    // time to mvmt_time and recalculate. This will avoid missing steps
    float added_time = 0;
    bool mtr_ready = false;
    while(!mtr_ready) {
        mtr_ready = true;
        for(int i = 0; i<5; i++) {
            if(stp_cnt[i] == 0) time_steps[i] = 0;
            else{
                time_steps[i] = (mvmt_time + added_time) / abs(stp_cnt[i]);
                if(time_steps[i]<min_mtr_delay) {
                    added_time += 500000; // half sec
                    mtr_ready = false;
                }
            }
        }
    }

    // Timer that the motors will trigger off
    unsigned long timeBegin = micros();
    
    // Time at which the next step should be taken for each motor
    float time_nxt_step [5] = {0, 0, 0, 0, 0};
    // Calculate the time of first step for each motor
    for(int i = 0; i<5; i++) time_nxt_step[i] += timeBegin + time_steps[i];
    
    send_message("Starting move command");

    signed int list_sla [abs(stp_cnt[2])] = {0};

    int i;
    unsigned long timeNow = micros();
    unsigned long timeEnd = timeNow + (mvmt_time + added_time);
    while(timeNow < timeEnd){
        timeNow = micros();

        //maybe faster to turn this into function
        for(i = 0; i<5; i++){
          
            //maybe comment out 2nd part of if statement
            if(timeNow>=time_nxt_step[i] && steps_taken[i]!=abs(stp_cnt[i])){
                step(i);
                
                //if(i==2 && abs(steps_taken[i])%1==0) list_sla[abs(steps_taken[i])] = analogRead(amisSLA[i]); // mtr_ready = false;//

                //maybe remove abs from modulo
                //if(abs(steps_taken[i])%Stall_Check_Step[i]==0 && pushRollingAverage(i, &voltage_log[i]) == false) {
                //    stop_message("Stall on motor " + String(i));
                //    return false;
                //}
                time_nxt_step[i] += time_steps[i];
                steps_taken[i]++;
            }
        }
    }

    //for(int i=0; i<abs(stp_cnt[2]); i++) Serial.println(list_sla[i]);
    
    //send_message("Done in "+String((double)(millis()-start_time)/1000.0)+" seconds");
    for(int i = 0; i<5; i++) send_message("MTR "+String(i)+" steps taken: " + String(steps_taken[i]));
    for(int i = 0; i<5; i++) send_message("MTR "+String(i)+" microseconds per step: " + String(time_steps[i]));
    return true;
}

bool homing_command(){
    //return true;
    struct VoltageAverage voltage_log[MTR_NUMBER];
    for(int i=0; i<MTR_NUMBER; i++) voltage_log[i] = createVoltageAverage();

    bool mtr_running [5] = {true, true, false, false, false};
    int mtrs_done = 0;
    
    for(int i = 0; i<5; i++) setDirection(i,HOME_DIRECTION[i]);

    unsigned long time_nxt_step = micros() + 5000;
    while(true) {
      if(micros()>time_nxt_step){
        time_nxt_step = micros() + 5000;
        step(4);
        mtr_running[4] = pushRollingAverage(4, &voltage_log[4]);
        if(mtr_running[4]==false) break;
      }
    }
    send_message("Motor 4 at home");

    time_nxt_step = micros() + 20000;
    while(true) {
      if(micros()>time_nxt_step){
        time_nxt_step = micros() + 5000;
        step(3);
        mtr_running[3] = pushRollingAverage(3, &voltage_log[3]);
        if(mtr_running[3]==false) break;
      }
    }
    send_message("Motor 3 at home");

    time_nxt_step = micros() + 350;
    send_message("Starting Homing");
    while(true) {
      if(micros()>time_nxt_step){
        time_nxt_step = micros() + 350;
        for(int i = 0; i<5; i++) {
          if(mtr_running[i]) {
            step(i);
            mtr_running[i] = pushRollingAverage(i, &voltage_log[i]);
            if(mtr_running[i]==false) {
              send_message("Motor " + String(i) + " at home");
              mtrs_done++;
            }
          }
        }
        if(mtrs_done==2) return true;
      }
    }
    return false;
}


/*
bool move_command(int stp_cnt[5], bool ht_nd){
    struct VoltageAverage voltage_log[MTR_NUMBER];
    for(int i=0; i<MTR_NUMBER; i++) voltage_log[i] = createVoltageAverage();
    unsigned long start_time = millis();

    for(int i = 0; i<5; i++) {
      if(stp_cnt[i] >= 0) setDirection(i,0);
      else setDirection(i,1);
    }
  
    int steps_taken[5] = {0, 0, 0, 0, 0};
    for(int i = 0; i<5; i++) time_steps[i] = mvmt_time / abs(stp_cnt[i]);

    unsigned long timeBegin = micros();
    float time_nxt_step [5] = {0, 0, 0, 0, 0};
    for(int i = 0; i<5; i++) time_nxt_step[i] += timeBegin + time_steps[i];
    
    send_message("Starting Up");
    Serial.println();
    
    
    unsigned long timeEnd = micros() + mvmt_time;
    unsigned long timeNow;
    unsigned long timeTotal;
    unsigned int stall_check = Output_Time;
    while(true) {
        timeTotal = micros();

        for(int i = 0; i<5; i++) {
            timeNow = micros();
            //Serial.println(String((timeNow-time_nxt_step[i])/time_steps[i]));
            for(int k = 0; k<(timeNow-time_nxt_step[i])/time_steps[i]; k++){
                //send_message("Step mtr " + String(i));
                step(i);
                if(stall_check==abs(steps_taken[i]) && false) {
                  stall_check += Output_Time;
                  Serial.println(analogRead(amisSLA[i]));
                  if(pushRollingAverage(analogRead(amisSLA[i]), &voltage_log[i]) == false) {
                    
                    stop_message("Stall on motor " + String(i));
                    return false;
                  }
                }
                //Serial.println(analogRead(amisSLA[i]));
                time_nxt_step[i] += time_steps[i];
                steps_taken[i]+= stp_cnt[i] / abs(stp_cnt[i]);
                if(steps_taken[i]== stp_cnt[i]) break;
            }  
        }
        if(timeTotal > timeEnd) break;
    }
    send_message("Done in "+String((double)(millis()-start_time)/1000.0)+" seconds");
    for(int i = 0; i<5; i++) send_message("MTR "+String(i)+" steps taken: " + String(steps_taken[i]));
    Serial.println();
    for(int i = 0; i<5; i++) send_message("MTR "+String(i)+" microseconds per step: " + String(time_steps[i]));
    return true;
}
*/

///*
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
//*/
