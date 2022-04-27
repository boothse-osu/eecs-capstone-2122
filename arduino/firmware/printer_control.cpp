#include "Arduino.h"

//#include "SPI.h"
//#include "AMIS30543.h"

#include "usb_lib.h"
#include "printer_control.h"

float time_steps [5];

void move_command(int stp_cnt[5], bool ht_nd){
    unsigned long start_time = millis();
  
    int steps_taken[5] = {0, 0, 0, 0, 0};
    for(int i = 0; i<5; i++) time_steps[i] = time_per_cm / abs(stp_cnt[i]);

    unsigned long timeBegin = micros();
    float time_nxt_step [5] = {0, 0, 0, 0, 0};
    for(int i = 0; i<5; i++) time_nxt_step[i] += timeBegin + time_steps[i];
    
    send_message("Starting Up");
    Serial.println();
    
    
    unsigned long timeEnd = micros() + time_per_cm;
    unsigned long timeNow;
    unsigned long timeTotal;
    
    while(true) {
        timeTotal = micros();
        for(int i = 0; i<5; i++) {
            timeNow = micros();
            //Serial.println(String((timeNow-time_nxt_step[i])/time_steps[i]));
            for(int k = 0; k<(timeNow-time_nxt_step[i])/time_steps[i]; k++){
                //send_message("Step mtr " + String(i));

                //step
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
}
