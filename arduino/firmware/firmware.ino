#include <SPI.h>
#include <AMIS30543.h>
#include "usb_lib.h";
#include "printer_control.h";
#include "stall_detection.h";
// <!D(+0123.1234,-0234.2345,+0345.3456,-0456.4567,+0567.5678,t)><!e>
// <!D(+0000.0000,-0000.0000,+0000.0000,-0030.0000,+0030.0000,t)><!e>
// <!D(-1000.0000,-0000.0000,+0000.0000,-0030.0000,-0030.0000,t)><!e>
// <!D(+0030.0000,-0030.0000,+0030.0000,-0030.0000,+0030.0000,t)><!e>
// <!D(-1500.0000,+1000.0000,+2000.0000,-0045.0000,-0045.0000,t)><!e>
// <!D(+0000.0000,-0000.0000,+0000.0000,-0000.0000,+0000.0000,t)><!e>
// <!D(+0030.0000,-0030.0000,+0030.0000,-0030.0000,+0030.0000,t)>
// <!D(+0180.0000,-0120.0000,+0120.0000,-0120.0000,+0120.0000,t)>

// <!D(-1000.0000,-0000.0000,-0500.0000,+0030.0000,+0030.0000,t)><!e>
// <!h>

// <!D(-0030.0000,-0000.0000,+0000.0000,+0000.0000,+0000.0000,t)>
// <!D(-0000.0000,-0000.0000,+0000.0000,+0000.2500,+0000.2500,t)>

// (+0080.0000,-0020.0000,+0060.0000,+0000.5000,+0000.5000,t)>!!!!!!

// <!D(-0000.0000,-0000.0000,+0000.0000,-0000.7854,+0000.0000,t)>

// <!!>
// 4,0,0100

unsigned long redirectStart;
char datatype;

AMIS30543 stepper[5] = {};

void setup()
{
  SPI.begin();
  Serial.begin(9600);
  Serial.setTimeout(10);


///*
  for(int i = 0; i < 5; i++) {
    stepper[i].init(amisSlaveSelect[i]);

    // Drive the NXT/STEP and DIR pins low initially.
    digitalWrite(amisStepPin[i], LOW);
    pinMode(amisStepPin[i], OUTPUT);
    digitalWrite(amisDirPin[i], LOW);
    pinMode(amisDirPin[i], OUTPUT);
  
    // Give the driver some time to power up.
    delay(1);
  
    // Reset the driver to its default settings.
    stepper[i].resetSettings();
  
    // Set the current limit.  You should change the number here to
    // an appropriate value for your particular system.
    if (i < 3) {
      stepper[i].setCurrentMilliamps(1800);
    } else {
      stepper[i].setCurrentMilliamps(700);
    }
  
    // Set the number of microsteps that correspond to one full step.
    stepper[i].setStepMode(4);
  
    stepper[i].setSlaTransparencyOff();
    stepper[i].setSlaGainHalf();
  
    // Enable the motor outputs.
    stepper[i].enableDriver();
  }
//*/
}


void loop()
{

    // Hotend Code?
    //Serial.println(analogRead(amisSLA[0]));
} 

void serialEvent()
{
   // <!D(+1234.1234,-2345.2345,+3456.3456,-4567.4567,+5678.5678,t)><!e>
   
    while(Serial.available()) 
    {
        redirectStart = millis();

        String str = Serial.readString();
        //Serial.println(str);
        //Serial.println(str.substring(0,2));
        //Serial.println(str.charAt(2));
        //Serial.println(str.substring(4,60));
        //Serial.println(str.substring(61,62));
        //Serial.println(str.length());


        if(str.substring(0,2) == "<!") {
            //send_message("GOOD COMMAND");
            
            datatype = str.charAt(2); 

            //send_message("Recieved message in: " + String((double)(millis() - redirectStart)/1000.0) + " seconds");

            if      (datatype == DATA && str.substring(61,62) == ">") parse_data(str.substring(4,60));
            else if (datatype == HOMING_REQ) {
              stepper[2].disableDriver();
              homing_sequence();
              Serial.println("done");
              stepper[2].enableDriver();
            }
            else if (datatype == DEBUG) debug_mode();
            else delay(100);
        }
        else send_message("BAD COMMAND");
   }
   send_message("Done in: " + String((double)(millis() - redirectStart)/1000.0) + " seconds");
}
