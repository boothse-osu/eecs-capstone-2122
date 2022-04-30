#include <AutoPID.h>

// define pins and constants
#define LED 13
#define HOTEND PA2
#define TEMPIN PA3
#define TREADCNT 5
#define KP 15
#define KI .3
#define KD 0
#define BETA 4390
#define BTEMP 298.15 
#define BRES 100000
#define SRES 10000
#define RUNINT 5000
#define OVERTEMP 280
#define OVERSET 20
#define TEMPRANGE 10
#define TEMPDELTA 2
#define UNDERHEAT 3

// set up PID control
double hotin, hotset, hotout;
uint32_t lastTempTime;
double lastTemp;
int underheatCount;

AutoPID hotpid(&hotin, &hotset, &hotout, 0, 255, KP, KI, KD);

double readTemp() {
  double counts = 0;
  for(int i = 0; i < TREADCNT; i++) {
    counts += analogRead(TEMPIN);
    delay(10);
  }
  counts /= TREADCNT;
  double res = SRES / (1023/counts - 1);
  double T = 1/(1/BTEMP + log(res/BRES)/BETA);
  return T - 273.15;
}

bool runawayCheck() {
  uint32_t currentTime = millis();
  bool alarm = false;
  if (currentTime - lastTempTime > RUNINT) {
    double tdiff = hotin-hotset;
    if (hotin >= OVERTEMP) {
      Serial.println("overtemp");
      alarm = true;
    }
    if (hotin - hotset > OVERSET and hotout > 0) {
      Serial.println("overset");
      alarm = true;
    }
    if (hotset-hotin > TEMPRANGE and hotin - lastTemp < TEMPDELTA) {
      Serial.println("not heating");
      underheatCount++;
      if (underheatCount >= UNDERHEAT) alarm = true;
    }
    else {
      underheatCount = 0;
    }
    if (hotin < 0) {
      Serial.println("sensor disconnected");
      alarm = true;
    }
    lastTempTime = currentTime;
    lastTemp = hotin;
  }
  if (alarm) {
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  
  // set pinmodes
  pinMode(TEMPIN, INPUT);
  pinMode(HOTEND, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW);

  // bang-bang mode when more than 4 degrees off target
  hotpid.setBangBang(4);
  // 4 second interval
  hotpid.setTimeStep(1000);
  hotset = 0;
  lastTempTime = micros();
  hotin = readTemp();
  lastTemp = hotin;
  underheatCount = 0;
}

void loop() {
  if (Serial.available() > 0) {
    hotset = Serial.parseFloat();
  }
  hotin = readTemp();
  
  hotpid.run();
  analogWrite(HOTEND, hotout);
  Serial.print(hotin);
  Serial.print(", ");
  Serial.print(hotset);
  Serial.print(", ");
  Serial.println(hotout);

  if(runawayCheck()) {
    Serial.println("Alarm tripped, stopping system");
    analogWrite(HOTEND, 0);
    digitalWrite(LED, HIGH);
    while(1);
  }
  delay(100);
}
