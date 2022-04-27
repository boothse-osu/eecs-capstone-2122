#include <SPI.h>
//include <AMIS30543.h>
#include "usb_lib.h";
#include "printer_control.h";
#include "stall_detection.h"

unsigned long redirectStart;
char datatype;

void setup()
{
  Serial.begin(9600);
}


void loop()
{
  // Hotend Code?
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

            send_message("Redirected in: " + String((double)(millis() - redirectStart)/1000.0) + " seconds");

            if(datatype == DATA) parse_data();
            else if (datatype == HOMING_REQ) delay(100);
            else delay(100);
        }
        else send_message("UNKNOWN CHARACTER RECIEVED");
        
   }
}
