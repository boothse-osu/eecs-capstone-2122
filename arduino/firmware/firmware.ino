//
#include <SPI.h>

// Motor Driver Library
#include <AMIS30543.h>

// USB Communication Function Library
#include "usb_lib.h"

// Printer Movement Function Library
#include "printer_control.h"

// Stall Detection Function Library
#include "stall_detection.h"

// Hot-End Control Function Library
#include "hotend_control.hpp"

/////////////////////////////////////////////////////////////////////

// Requirement Grading:

// Positional acc +/- 1deg && Arb Positioning
// <!D(+0000.0000,+0000.0000,+0000.0000,-0000.7854,+0000.0000,t)>

// 5 motor
// <!D(+0020.0000,-0020.0000,+0020.0000,+0000.5000,+0000.5000,t)>

// 4 motor
// <!D(+0020.0000,-0020.0000,-0000.0000,+0000.5000,+0000.5000,t)>

// Serial
// <!!>

// Stall Detection
// <!!> 
// 4,0,00100

// Filament
// <!f>

// Thermal Runaway
// <!n(200)> <!t> disconnect sensor

// Safety
// visual inspection

/////////////////////////////////////////////////////////////////////

// Timer for how a serial event runs, blocking the main loop.
unsigned long processing_start;

// Char holding the serial request command signifier.
char signifier;

// Array of Motor Driver Steppers.
AMIS30543 stepper[5] = {};

void setup()
{
  SPI.begin();
  Serial.begin(9600);

  // Shortens the timeout on readString() from 1000 to prevent
  // long blocking.
  Serial.setTimeout(10);

  // Setup for each motor driver
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

  // Setup hotend with correct pinmodes and set variables.
  hotendSetup();
}


void loop()
{
  // Maintain hot-end temp and check for thermal runaway.
  runHotend();

  // serialEvent is called by the arduino at the end of every cycle.
} 

// Handles any commands the printer recieves over serial.
void serialEvent()
{ 
  // Checks if there is anything in the serial recieve buffer. The
  // buffer holds 64 bytes.
  while(Serial.available()) 
  {
    // Start the timer to record serial processing time.
    processing_start = millis();

    // Read the entire buffer into a string
    String serial_message = Serial.readString();

    // Checks that the string from buffer is a serial command. i.e.
    // <!{Command Signifier}{optional: (data)}>
    // <!h>       - Homing Command
    // <!n(200)>  - Set Hot-end to New Temp (200)
    if(serial_message.substring(0,2) == "<!") {

      // Get command signifier char from buffer string.
      signifier = serial_message.charAt(2); 


      // Re-direct to the command that was called.

      // Signifier shows a move command and the move data is the right 
      // length. Send the data to a data parser that will extract 
      // variables and call a move command.
      if (signifier == MOVE_DATA && serial_message.substring(61,62) == ">") 
        handle_move(serial_message.substring(4,60));

      else if (signifier == MOVE_DATA && serial_message.substring(61,62) == ">") 
        handle_print_move(serial_message.substring(4));

      // Signifier shows a homing request. Disable the z-axis motor so 
      // it will fall to its lowest point, initiate a homing sequence, 
      // wait for the z-axis to fall and re-enable it.
      else if (signifier == HOMING_REQ) {
        setDirection(2,0);
        for(int i = 0; i<50; i++){
          step(2);
          delayMicroseconds(300);
        }
        stepper[2].disableDriver();
        stepper[4].disableDriver();
        handle_homing();
        //delay(1000*3); // 3sec delay
        stepper[2].enableDriver();
        stepper[4].enableDriver();
      }

      // Signifier shows a filament extrusion test request. Initiate a 
      // filament test.
      else if (signifier == FILAMENT) 
        handle_filament_test(serial_message.substring(4));

      // Signifier shows a hot-end tempature change request. Call hot-end 
      // tempature change handler.
      else if (signifier == TEMP_SET) 
        hotendParse(serial_message.substring(4));

      // Signifier shows a hot-end status request. Send message containing 
      // hot-end information.
      else if (signifier == TEMP_REP) 
        send_message("Current temp: " + String(hotin) + "°C, Target temp: " + String(hotset) + "°C");
      
      // Signifer shows a debug mode request. Call debug mode handler.
      else if (signifier == DEBUG) 
        handle_debug();
      
      else send_message("UNKOWN COMMAND");;
    }
    else send_message("BAD COMMAND");
  }

  // Send message of the time it took to process buffer.
  send_message("Done in: " + String((double)(millis() - processing_start)/1000.0) + " seconds");
}
