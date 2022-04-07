#include <SPI.h>
#include <AMIS30543.h>

const uint8_t amisDirPin[5] = {PB11, PB12, PB13, PB14, PB15};
const uint8_t amisStepPin[5] = {PC5, PC6, PC7, PC8, PC9};
const uint8_t amisSlaveSelect[5] = {PB3, PB4, PB5, PB6, PB7};
const uint8_t amisSLA[5] = {PA0, PA1, PA4, PB0, PC1};

AMIS30543 stepper;

void setup()
{
  SPI.begin();
  Serial.begin(9600);

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
}

void loop()
{
  // Step in the default direction 1000 times.
  setDirection(0,0);
  setDirection(1,0);
  setDirection(2,0);
  setDirection(3,0);
  setDirection(4,0);
  for (unsigned int x = 0; x < 2400; x++)
  {
    step(0);
    step(1);
    step(2);
    step(3);
    step(4);
    //if(x%10 == 0) {
    //  Serial.print(analogRead(therm));
    //  Serial.print(", ");
    //  Serial.println(analogRead(amisSLA));
    //}
  }

  // Wait for 300 ms.
  for (unsigned int x = 0; x < 1600; x++)
  {
    delay(1);
    //if(x%10 == 0) {
    //  Serial.print(analogRead(therm));
    //  Serial.print(", ");
    //  Serial.println(analogRead(amisSLA));
    //}
  }

  // Step in the default direction 1000 times.
  setDirection(0,1);
  setDirection(1,1);
  setDirection(2,1);
  setDirection(3,1);
  setDirection(4,1);
  for (unsigned int x = 0; x < 2400; x++)
  {
    step(0);
    step(1);
    step(2);
    step(3);
    step(4);
    //if(x%10 == 0) {
    //  Serial.print(analogRead(therm));
    //  Serial.print(", ");
    //  Serial.println(analogRead(amisSLA));
    //}
  }

  // Wait for 300 ms.
  for (unsigned int x = 0; x < 1600; x++)
  {
    delay(1);
    //if(x%10 == 0) {
    //  Serial.print(analogRead(therm));
    //  Serial.print(", ");
    //  Serial.println(analogRead(amisSLA));
    //}
  }
}

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
