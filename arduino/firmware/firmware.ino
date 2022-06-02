//
#include <SPI.h>

// Motor Driver Library
#include <AMIS30543.h>

//
#include "config.h"

// USB Communication Function Library
#include "usb_lib.h"

// Printer Movement Function Library
#include "printer_control.h"

// Stall Detection Function Library
#include "stall_detection.h"

// Hot-End Control Function Library
#include "hotend_control.hpp"

/////////////////////////////////////////////////////////////////////

// Command Examples:

// y = 365
// <!D(+0000.0000,+0014.0682,+0003.5343,+0000.0000,+0000.0000,f,009.4918)>
// y = 355
// <!D(+0165.3356,+0012.4316,+0003.5343,+0000.0000,+0000.0000,f,018.9835)>

// <!D(+0020.0000,-0000.0000,-0000.0000,+0000.0000,+0000.0000,t)><!D(-0030.0000,-0005.0000,-0000.0000,+0000.0000,+0000.0000,t)><!D(+0010.0000,+0005.0000,-0000.0000,+0000.0000,+0000.0000,t)><!D(+0020.0000,-0000.0000,-0000.0000,+0000.0000,+0000.0000,t)><!D(-0030.0000,-0005.0000,-0000.0000,+0000.0000,+0000.0000,t)><!D(+0010.0000,+0005.0000,-0000.0000,+0000.0000,+0000.0000,t)>

// Positional acc +/- 1deg && Arb Positioning
// <!D(+0000.0000,+0000.0000,+0000.0000,-0000.7854,+0000.0000,t)>

// 5 motor
// <!D(+0020.0000,-0020.0000,+0020.0000,+0000.5000,+0000.5000,t)>

// 4 motor
// <!D(+0020.0000,-0000.0000,-0000.0000,+0000.0000,+0000.0000,t)>
// <!D(-0020.0000,-0000.0000,-0000.0000,+0000.0000,+0000.0000,t)>
// <!D(-0000.0000,-0005.0000,-0000.0000,+0000.0000,+0000.0000,t)>

// 5 motor + extrude
// <!D(+0000.0000,-0000.0000,-0000.0000,+0000.0000,+0000.0000,-02.0000)>


// Set Hot-End Temp
// <!n(200)> 

// Request Hot-End Status
// <!t>

/////////////////////////////////////////////////////////////////////

String oldCircle = "<!D(+0128.0122,+0012.4316,+0003.5343,+0000.0000,+0000.0000,f,004.0000)><!D(+0000.0001,+0000.0001,+0000.0000,+0000.0000,+0000.0000,t,000.0000)><!D(-0000.3014,+0001.0908,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0000.8989,+0001.0713,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0001.4803,+0001.0328,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0002.0354,+0000.9757,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0002.5541,+0000.9013,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0003.0272,+0000.8108,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0003.4464,+0000.7058,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0003.8040,+0000.5882,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0004.0937,+0000.4602,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0004.3104,+0000.3240,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0004.4501,+0000.1819,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0004.5104,+0000.0365,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0004.4903,-0000.1094,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0004.3900,-0000.2534,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0004.2114,-0000.3928,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0003.9577,-0000.5253,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0003.6333,-0000.6484,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0003.2440,-0000.7599,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0002.7969,-0000.8579,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0002.2999,-0000.9406,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0001.7618,-0001.0064,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0001.1922,-0001.0543,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(-0000.6015,-0001.0833,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0000.0000,-0001.0931,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0000.6016,-0001.0834,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0001.1924,-0001.0542,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0001.7619,-0001.0064,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0002.2999,-0000.9405,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0002.7970,-0000.8579,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0003.2441,-0000.7599,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0003.6333,-0000.6484,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0003.9577,-0000.5253,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0004.2115,-0000.3928,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0004.3901,-0000.2533,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0004.4904,-0000.1093,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0004.5105,+0000.0366,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0004.4502,+0000.1819,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0004.3104,+0000.3239,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0004.0937,+0000.4602,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0003.8040,+0000.5883,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0003.4464,+0000.7058,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0003.0273,+0000.8108,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0002.5542,+0000.9013,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0002.0354,+0000.9757,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0001.4804,+0001.0327,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0000.8990,+0001.0714,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0000.3015,+0001.0908,+0000.0000,+0000.0000,+0000.0000,t,000.3340)><!D(+0000.0001,+0000.0001,+0000.0000,+0000.0000,+0000.0000,f,000.0000)>";


// Timer for how a serial event runs, blocking the main loop.
unsigned long serialReceivedTime;

// Char holding the serial request command signifier.
char signifier;

long motorStepAmounts[5];

char hotEndSignifier;

unsigned long movementTime;

// Array of Motor Driver Steppers.
AMIS30543 stepper[6] = {};

void setup()
{
  SPI.begin();
  Serial.begin(9600);

  // Shortens the timeout on readString() from 1000 to prevent
  // long blocking. Can go as low as 3 for 64 byte messages.
  Serial.setTimeout(4);

  // Setup for each motor driver
  for(int i = 0; i < 6; i++) {
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
    if (i < 3 || i == 5) {
      stepper[i].setCurrentMilliamps(1800);
    } else {
      stepper[i].setCurrentMilliamps(700);
    }
  
    // Set the number of micro-steps that correspond to one full step.
    stepper[i].setStepMode(4);
  
    stepper[i].setSlaTransparencyOff();
    stepper[i].setSlaGainHalf();
  
    // Enable the motor outputs.
    stepper[i].enableDriver();
  }

  // Setup hot-end with correct pin-modes and set variables.
  HotEndSetup();
}


void loop()
{  
  // Maintain hot-end temp and check for thermal runaway.
  RunHotEnd();

  // serialEvent is called by the arduino at the end of every cycle.
} 

// Handles any commands the printer receives over serial.
void serialEvent()
{ 
  // Checks if there is anything in the serial receive buffer. The
  // buffer holds 64 bytes.
  while(Serial.available()) 
  {
    // Read the entire buffer into a string
    String serialMessage = Serial.readString();

/////////////////////////////////////////////////////////////////////////////////////////////////

    // Signifier shows a move command and the move data is the right 
    // length. Send the data to a data parser that will extract 
    // variables and call a move command.
    if (serialMessage.charAt(2) == MOVE_DATA){
      for(int i = 0; i<DATA_REQUEST_AMOUNT; i++){
        if(serialMessage.charAt(2) != EOF_MSG) {
          
          // IK used to send a T/F for hot-end operation
          hotEndSignifier = serialMessage.charAt(59);
          //SendMessage(String(hotEndSignifier));

          // Parse data for angle changes and calculate steps for each motor
          for(int i = 0; i<5; i++) {
            motorStepAmounts[i] = round(serialMessage.substring((i*11)+4, (i*11)+4+10).toDouble() * (motorStepsPerRotation[i] / radianConversion));
            //SendMessage(String(motorStepAmounts[i]));
          }

          //SendMessage(serialMessage.substring(61,69));
          movementTime = serialMessage.substring(61,69).toDouble() * 1000000;

          //unsigned long time_st = micros();
          MoveCommand(motorStepAmounts, hotEndSignifier, movementTime);
          //Serial.println((micros()-time_st)/1000000);
          
          //Serial.println(serialMessage);
          serialMessage = serialMessage.substring(62);
          //serialMessage = serialMessage.substring(71);
        }
        else {
          SendMessage("End of File Reached.");
          break;
        }
      }
      SendDataRequest(DATA_REQUEST_AMOUNT);
    }

/////////////////////////////////////////////////////////////////////////////////////////////////

    // Signifier shows a homing request. Disable the z-axis motor so 
    // it will fall to its lowest point, initiate a homing sequence, 
    // wait for the z-axis to fall and re-enable it.
    else if (serialMessage.charAt(2) == HOMING_REQ) {
      SetDirection(2,0);
      for(int i = 0; i<50; i++){
        Step(2);
        delayMicroseconds(300);
      }
      stepper[2].disableDriver();
      stepper[4].disableDriver();

      //if(!HomingCommand()) SendStopMessage("Homing Failed");
      //delay(1000*3); // 3sec delay
      stepper[2].enableDriver();
      stepper[4].enableDriver();

      // Confirm homing complete
      SendHomingComplete();
      // Request data needed to start IK
      SendDataRequest(DATA_REQUEST_AMOUNT);
    }

/////////////////////////////////////////////////////////////////////////////////////////////////

    // Signifier shows a filament extrusion test request. Initiate a 
    // filament test.
    else if (serialMessage.charAt(2) == FILAMENT) {
      double cmPerSecond = serialMessage.substring(4,10).toDouble();
      int filamentLength = serialMessage.substring(11,14).toInt();
      ExtrudeCommand(cmPerSecond, filamentLength);
    }

/////////////////////////////////////////////////////////////////////////////////////////////////

    // Signifier shows a hot-end temperature change request. Call hot-end 
    // temperature change handler.
    else if (serialMessage.charAt(2) == TEMP_SET) 
      hotendParse(serialMessage.substring(4));

/////////////////////////////////////////////////////////////////////////////////////////////////

    // Signifier shows a hot-end status request. Send message containing 
    // hot-end information.
    else if (serialMessage.charAt(2) == TEMP_REP) 
      SendMessage("Current temp: " + String(hotin) + "°C, Target temp: " + String(hotset) + "°C");
    
/////////////////////////////////////////////////////////////////////////////////////////////////

    // Signifier shows a debug mode request. Call debug mode handler.
    else if (serialMessage.charAt(2) == DEBUG) 
      DebugHandler();
    
/////////////////////////////////////////////////////////////////////////////////////////////////

    else SendMessage("UNKOWN COMMAND");;

  }
}
