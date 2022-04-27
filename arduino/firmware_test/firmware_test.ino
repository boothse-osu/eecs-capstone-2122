#include <SPI.h>
/*
#include <AMIS30543.h>

const uint8_t amisDirPin[5] = {PB11, PB12, PB13, PB14, PB15};
const uint8_t amisStepPin[5] = {PC5, PC6, PC7, PC8, PC9};
const uint8_t amisSlaveSelect[5] = {PB3, PB4, PB5, PB6, PB7};
const uint8_t amisSLA[5] = {PA0, PA1, PA4, PB0, PC1};


AMIS30543 stepper;
//*/
const int steps_per_rotation = 1600; // I forgot again (maybe 800)
const float steps_per_degree = (float)steps_per_rotation / 360;

const int cm_step_amount = (int)((float)steps_per_rotation * 0.2875); // UNNEEDED NOW, working with roation angles
const int mm_step_amount = (int)((float)cm_step_amount / 10); // UNNEEDED NOW, working with roation angles

bool debug = true;

bool jobDone = false;

void setup()
{
  //SPI.begin();
  Serial.begin(9600);

/*
  for(int i = 0; i < 5; i++) {
    stepper.init(amisSlaveSelect[i]);

    // Drive the NXT/STEP and DIR pins low initially.
    digitalWrite(amisStepPin[i], LOW);
    pinMode(amisStepPin[i], OUTPUT);
    digitalWrite(amisDirPin[i], LOW);
    pinMode(amisDirPin[i], OUTPUT);
  
    // Give the driver some time to power up.
    delay(1);
  
    // Reset the driver to its default settings.
    stepper.resetSettings();
  
    // Set the current limit.  You should change the number here to
    // an appropriate value for your particular system.
    if (i < 3) {
      stepper.setCurrentMilliamps(1800);
    } else {
      stepper.setCurrentMilliamps(700);
    }
  
    // Set the number of microsteps that correspond to one full step.
    stepper.setStepMode(4);
  
    stepper.setSlaTransparencyOff();
    stepper.setSlaGainHalf();
  
    // Enable the motor outputs.
    stepper.enableDriver();
  }
  //*/
}


void loop()
{
    // take input
    // <!data(X)>
    // X ::= (float)xxxx.xxxx - 5;, (bool)x
    // (ffff.ffff;ffff.ffff;ffff.ffff;ffff.ffff;ffff.ffff;x)
    float ex_mtr_dt[5] = {30.0, -30.5, 100.05, 0.0, 0.0};
    bool ex_ht_nd   = false;


    float time_per_cm = 1000000*3; //  3 000 000 = 3 seconds
    int ex_stp_cnt[5] = {400, 250, -315, 1479, 0};
    int steps_taken[5] = {0, 0, 0, 0, 0};
    float time_steps [5];
    
    for(int i = 0; i<5; i++) time_steps[i] = time_per_cm / abs(ex_stp_cnt[i]);

    
    unsigned long timeBegin = micros();
    unsigned long timeEnd = timeBegin + time_per_cm;
    unsigned long timeNow;
    unsigned long timeTotal;

    float time_nxt_step [5] = {0, 0, 0, 0, 0};
    for(int i = 0; i<5; i++) time_nxt_step[i] += timeBegin + time_steps[i];

    
    if (!jobDone) {
        Serial.println("Starting Up\n");
        //Serial.println(micros());
        //Serial.println("timeend " + String(timeBegin + time_per_cm));
        unsigned long start_time = millis();
        while(true) {
            timeTotal = micros();
            for(int i = 0; i<5; i++) {
                timeNow = micros();
                //if(timeNow > time_nxt_step[i]) {
                  //Serial.println(String((timeNow-time_nxt_step[i])/time_steps[i]));
                  for(int k = 0; k<(timeNow-time_nxt_step[i])/time_steps[i]; k++){
                      //step
                      //Serial.println("Step mtr " + String(i));
                      time_nxt_step[i] += time_steps[i];
                      steps_taken[i]+= ex_stp_cnt[i] / abs(ex_stp_cnt[i]);
                      if(steps_taken[i]== ex_stp_cnt[i]) break;
                  }  
                //}
            }
            if(timeTotal > timeEnd) {
                if (debug) jobDone = true;
                Serial.println("Done in "+String((double)(millis()-start_time)/1000.0)+" seconds\n");
                break;
            }
          //Serial.write("looping\n");
        }
        for(int i = 0; i<5; i++) Serial.println("MTR "+String(i)+" steps taken: " + String(steps_taken[i]));
        Serial.println();
        for(int i = 0; i<5; i++) Serial.println("MTR "+String(i)+" microseconds per step: " + String(time_steps[i]));

      /*
        int step[5] = {0, 0, 0, 0, 0};
        int stepped[5] = {0, 0, 0, 0, 0};
        
        for(int i=0; i<5; i++) step[i] = (int)(ex_stp_cnt[i] / 100);

        for(int k=0; k<100; k++) for(int i=0; i<5; i++) stepped[i] += step[i];
        
        for(int i=0; i<5; i++) Serial.print("%d\n", stepped[i]);
        
        
      */
      
    }
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


int move_motors(float* move_amounts, bool ht_nd)
{
}

int run_homing()
{
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

/*

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
  delayMicroseconds(500);
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
