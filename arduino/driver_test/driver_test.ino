/* This example shows basic use of the AMIS-30543 stepper motor
driver.

It shows how to initialize the driver, set the current limit, set
the micro-stepping mode, and enable the driver.  It shows how to
send pulses to the NXT/STEP pin to get the driver to take steps
and how to switch directions using the DIR pin.  The DO pin is
not used and does not need to be connected.

Before using this example, be sure to change the
setCurrentMilliamps line to have an appropriate current limit for
your system.  Also, see this library's documentation for
information about how to connect the driver:

    http://pololu.github.io/amis-30543-arduino/
*/

#include <SPI.h>
#include <AMIS30543.h>

const uint8_t amisDirPin = 2;
const uint8_t amisStepPin = 3;
const uint8_t amisSlaveSelect = 4;
const uint8_t amisSLA = A0;

const uint8_t hotend = 5;
const uint8_t therm = A1;

AMIS30543 stepper;

void setup()
{
  SPI.begin();
  Serial.begin(9600);
  stepper.init(amisSlaveSelect);

  // Drive the NXT/STEP and DIR pins low initially.
  digitalWrite(amisStepPin, LOW);
  pinMode(amisStepPin, OUTPUT);
  digitalWrite(amisDirPin, LOW);
  pinMode(amisDirPin, OUTPUT);

  analogWrite(hotend, 0);

  // Give the driver some time to power up.
  delay(1);

  // Reset the driver to its default settings.
  stepper.resetSettings();

  // Set the current limit.  You should change the number here to
  // an appropriate value for your particular system.
  stepper.setCurrentMilliamps(400);

  // Set the number of microsteps that correspond to one full step.
  stepper.setStepMode(4);

  stepper.setSlaTransparencyOff();
  stepper.setSlaGainHalf();

  // Enable the motor outputs.
  stepper.enableDriver();
}

void loop()
{
  // Step in the default direction 1000 times.
  setDirection(0);
  for (unsigned int x = 0; x < 800; x++)
  {
    step();
    if(x%10 == 0) {
      Serial.print(analogRead(therm));
      Serial.print(", ");
      Serial.println(analogRead(amisSLA));
    }
  }

  // Wait for 300 ms.
  for (unsigned int x = 0; x < 1600; x++)
  {
    delay(1);
    if(x%10 == 0) {
      Serial.print(analogRead(therm));
      Serial.print(", ");
      Serial.println(analogRead(amisSLA));
    }
  }

  if (Serial.available() > 0) {
    analogWrite(hotend, Serial.parseInt());
  }
}

// Sends a pulse on the NXT/STEP pin to tell the driver to take
// one step, and also delays to control the speed of the motor.
void step()
{
  // The NXT/STEP minimum high pulse width is 2 microseconds.
  digitalWrite(amisStepPin, HIGH);
  delayMicroseconds(3);
  digitalWrite(amisStepPin, LOW);
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
void setDirection(bool dir)
{
  // The NXT/STEP pin must not change for at least 0.5
  // microseconds before and after changing the DIR pin.
  delayMicroseconds(1);
  digitalWrite(amisDirPin, dir);
  delayMicroseconds(1);
}
