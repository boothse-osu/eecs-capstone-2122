#include <SPI.h>

#include <AMIS30543.h>

const uint8_t amisDirPin[5] = {PB11, PB12, PB13, PB14, PB15};
const uint8_t amisStepPin[5] = {PC5, PC6, PC7, PC8, PC9};
const uint8_t amisSlaveSelect[5] = {PB3, PB4, PB5, PB6, PB7};
const uint8_t amisSLA[5] = {PA0, PA1, PA4, PB0, PC1};

// Radius of circle in steps
const int cm_radius = 2000;

bool jobDone = false;

AMIS30543 stepper;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  Serial.begin(9600);

  for(int i = 0; i < 2; i++) {
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
    stepper.setCurrentMilliamps(1800);
  
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
  // Stops the loop after the job has been completed
  if(jobDone==false)
  {
    int x = cm_radius;
    int y = 0;
    int x_x = 0;
    int y_y = 0;
    int change = 0;
    int x_shift = 0;
    int y_shift = 0;
    for (int angle = 0; angle < 361; angle++)
    {
      if (angle==0 || angle==360)
      {
        x_x = cm_radius;
        y_y = 0;
      }
      else if (angle==90)
      {
        x_x = 0;
        y_y = cm_radius;
      }
      else if (angle < 180)
      {
        x_x = x_triangle(angle);
        y_y = y_triangle(angle);
      }

      else if (angle==180)
      {
        x_x = -cm_radius;
        y_y = 0;
      }
      else if (angle==270)
      {
        x_x = 0;
        y_y = -cm_radius;
      }
      else
      {
        x_x = -x_triangle(angle-180);
        y_y = -y_triangle(angle-180);
      }
      
      if(x != x_x)
      {
        change = x_x-x;
        Serial.print("\nX: ");
        Serial.print(change);
        x_shift += change;
        if(change > 0) setDirection(1,0);
        else setDirection(1,1);
        for(int i = 0; i<abs(change); i++) step(1); // add abs to change
        //step(1);
        x = x_x;
      }
      if(y != y_y)
      {
        change = y_y-y;
        Serial.print("\nY: ");
        Serial.print(change);
        y_shift += change;
        if(change > 0) setDirection(0,0);
        else setDirection(0,1);
        for(int i = 0; i<abs(change); i++) step(0); // add abs to change
        //step(0);
        y = y_y;
      }
    }
    Serial.print("\nX_shift: ");
    Serial.print(x_shift);
    Serial.print("\nY_shift: ");
    Serial.print(y_shift);
    jobDone = true;
  }
}


float sin_deg(int angle)
{
  // converts degrees to radians
  float rad = (float) angle * 71 / 4068;
  // return the sin of the radian
  return sin(rad);
}

int x_triangle(int angle_x)
{
  // calculate the width of a right triangle on the circle
  return (int) (((float) cm_radius * sin_deg(90 - angle_x)) / sin_deg(90));
}

int y_triangle(int angle_y)
{
  // calculate the height of a right triangle on the circle
  return (int) (((float) cm_radius * sin_deg(angle_y)) / sin_deg(90));
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
