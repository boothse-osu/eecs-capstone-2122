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

// Command Examples:

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

// Timer for how a serial event runs, blocking the main loop.
unsigned long processing_start;

// Char holding the serial request command signifier.
char signifier;

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
  hotendSetup();
}


void loop()
{  
  // Maintain hot-end temp and check for thermal runaway.
  //runHotend();

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
    String serial_message = Serial.readString();

    // Signifier shows a move command and the move data is the right 
    // length. Send the data to a data parser that will extract 
    // variables and call a move command.
    if (serial_message.charAt(2) == MOVE_DATA){// && serial_message.substring(61,62) == ">") {
      for(int i = 0; i<data_length; i++){
        if(serial_message.charAt(2) != EOF_MSG) {
          long mtr_steps[5];
          // IK used to send a T/F for hot-end operation
          char hot_end = str.charAt(59);
          send_message(String(hot_end);)

          // Parse data for angle changes and calculate steps for each motor
          for(int i = 0; i<5; i++) {
            mtr_steps[i] = round(str.substring((i*11)+4, (i*11)+4+10).toDouble() * (steps_per_rotations[i] / step_mode));
            send_message(String(mtr_steps[i]));
          }

          double time_move = serial_message.substring(61,69).toDouble();
          unsigned long time_micro = time_move * 1000000;

          move_command(mtr_steps, hot_end, time_micro);
          
          request_data(data_length);
          
          //serial_message = serial_message.substring(62);
        }
        else {
          send_message("End of File Reached.");
          break;
        }
      }
    }

    else if (serial_message.charAt(2) == MOVE_DATA && serial_message.substring(68,69) == ">") {
      handle_print_move(serial_message.substring(4));
      serial_message = serial_message.substring(69);
    }

    // Signifier shows a homing request. Disable the z-axis motor so 
    // it will fall to its lowest point, initiate a homing sequence, 
    // wait for the z-axis to fall and re-enable it.
    else if (serial_message.charAt(2) == HOMING_REQ) {
      setDirection(2,0);
      for(int i = 0; i<50; i++){
        step(2);
        delayMicroseconds(300);
      }
      stepper[2].disableDriver();
      stepper[4].disableDriver();

      if(!homing_command()) stop_message("Homing Failed");
      //delay(1000*3); // 3sec delay
      stepper[2].enableDriver();
      stepper[4].enableDriver();

      // Confirm homing complete
      confirm_homing();
      // Request data needed to start IK
      request_data(data_length);
    }

    // Signifier shows a filament extrusion test request. Initiate a 
    // filament test.
    else if (serial_message.charAt(2) == FILAMENT) {
      double cms = serial_message.substring(4,10).toDouble();
      int distance = serial_message.substring(11,14).toInt();
      extrude(cms, distance);
    }

    // Signifier shows a hot-end temperature change request. Call hot-end 
    // temperature change handler.
    else if (serial_message.charAt(2) == TEMP_SET) 
      hotendParse(serial_message.substring(4));

    // Signifier shows a hot-end status request. Send message containing 
    // hot-end information.
    else if (serial_message.charAt(2) == TEMP_REP) 
      send_message("Current temp: " + String(hotin) + "°C, Target temp: " + String(hotset) + "°C");
    
    // Signifier shows a debug mode request. Call debug mode handler.
    else if (serial_message.charAt(2) == DEBUG) 
      handle_debug();
    
    else send_message("UNKOWN COMMAND");;

  }
}
