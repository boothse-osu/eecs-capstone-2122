#include <SPI.h>
#include <AMIS30543.h>

const uint8_t amisDirPin[5] = {PB11, PB12, PB13, PB14, PB15};
const uint8_t amisStepPin[5] = {PC5, PC6, PC7, PC8, PC9};
const uint8_t amisSlaveSelect[5] = {PB3, PB4, PB5, PB6, PB7};
const uint8_t amisSLA[5] = {PA0, PA1, PA4, PB0, PC1};

const int extruder_index = 2;

//  12 mm diameter
// (10 / (diameter_mm * pi)) * steps_per_rotation
const int cm_step_amount = 460;
// 100cm: meter
const int cm_target_number = 300;

bool jobDone = false;

AMIS30543 stepper;

void setup()
{
  SPI.begin();
  Serial.begin(9600);

  stepper.init(amisSlaveSelect[extruder_index]);

  // Drive the NXT/STEP and DIR pins low initially.
  digitalWrite(amisStepPin[extruder_index], LOW);
  pinMode(amisStepPin[extruder_index], OUTPUT);
  digitalWrite(amisDirPin[extruder_index], LOW);
  pinMode(amisDirPin[extruder_index], OUTPUT);

  // Give the driver some time to power up.
  delay(1);

  // Reset the driver to its default settings.
  stepper.resetSettings();

  // Set the current limit.  You should change the number here to
  // an appropriate value for your particular system.
  stepper.setCurrentMilliamps(1800);
  // or stepper.setCurrentMilliamps(1800);

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
  if(jobDone==false)
  {
    setDirection(extruder_index,0);
    int step_count = 0;
    // How many cm to extrude
    for (unsigned int x = 0; x < cm_target_number; x++)
    {
      // Steps to extrude one cm
      for (unsigned int i = 0; i < cm_step_amount; i++)
      {
        step_count++;
        step(extruder_index);
      }
    }
    Serial.print("\nCm extruded: "); Serial.print(cm_target_number);
    Serial.print("\nSteps per cm: "); Serial.print(cm_step_amount);
    Serial.print("\nSteps taken: "); Serial.print(step_count);
    jobDone = true;
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
