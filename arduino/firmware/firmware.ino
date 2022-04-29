#include <SPI.h>
#include <AMIS30543.h>
#include "usb_lib.h";
#include "printer_control.h";
#include "stall_detection.h";
// <!D(+0123.1234,-0234.2345,+0345.3456,-0456.4567,+0567.5678,t)><!e>
// <!D(+0000.0000,-0000.0000,+0000.0000,-0030.0000,+0030.0000,t)><!e>
// <!D(-1000.0000,-0000.0000,+0000.0000,-0030.0000,-0030.0000,t)><!e>
// <!D(-1500.0000,+1000.0000,+2000.0000,-0045.0000,-0045.0000,t)><!e>
// <!D(+0000.0000,-0000.0000,+0000.0000,-0000.0000,+0000.0000,t)><!e>

// <!D(-1000.0000,-0000.0000,-0500.0000,+0030.0000,+0030.0000,t)><!e>
// <!h>
/*
const uint8_t amisDirPin[5] = {PB11, PB12, PB13, PB14, PB15};
const uint8_t amisStepPin[5] = {PC5, PC6, PC7, PC8, PC9};
const uint8_t amisSlaveSelect[5] = {PB3, PB4, PB5, PB6, PB7};
const uint8_t amisSLA[5] = {PA0, PA1, PA4, PB0, PC1};
//*/
unsigned long redirectStart;
char datatype;

AMIS30543 stepper;



void setup()
{
  SPI.begin();
  Serial.begin(9600);


///*
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

    // Hotend Code?
    //Serial.println(analogRead(amisSLA[0]));
} 

void serialEvent()
{
   // <!D(+1234.1234,-2345.2345,+3456.3456,-4567.4567,+5678.5678,t)><!e>
   
    while(Serial.available()) 
    {
        redirectStart = millis();
        
        delay(100); // Delay to recieve full message before you read it
    
        if(Serial.read() == '<' && Serial.read() == '!') {
            //send_message("GOOD COMMAND");
        
            datatype = Serial.read(); 

            send_message("Recieved message in: " + String((double)(millis() - redirectStart)/1000.0) + " seconds");

            if      (datatype == DATA) parse_data();
            else if (datatype == HOMING_REQ) homing_sequence();
            else if (datatype == DEBUG) debug_mode();
            else delay(100);
        }
        else send_message("UNKNOWN CHARACTER RECIEVED");
        
   }
}
