#pragma once
#include <AutoPID.h>
#include "usb_lib.h"
#include "Arduino.h"

// define pins and constants
#define HOTEND PA10
#define TEMPIN PC4
#define TREADCNT 5
#define KP 15
#define KI .3
#define KD 0
#define BETA 4267
#define BTEMP 298.15 
#define BRES 100000
#define SRES 10000
#define PIDINT 1000
#define RUNINT 5000
#define OVERTEMP 280
#define OVERSET 20
#define TEMPRANGE 10
#define TEMPDELTA 2
#define UNDERHEAT 3
#define DISCON 10

// set up PID control
double hotin, hotset, hotout;
uint32_t lastTempTime, lastPIDTime;
double lastTemp;
int underheatCount;
String alarmMsg;

AutoPID hotpid(&hotin, &hotset, &hotout, 0, 255, KP, KI, KD);

double readTemp() {
  double counts = 0;
  for(int i = 0; i < TREADCNT; i++) {
    counts += analogRead(TEMPIN);
    delayMicroseconds(100);
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
      alarmMsg = "Max temp exceeded";
      alarm = true;
    }
    if (hotin - hotset > OVERSET and hotout > 0) {
      alarmMsg = "Temp far exceeds set point";
      alarm = true;
    }
    if (hotset-hotin > TEMPRANGE and hotin - lastTemp < TEMPDELTA) {
      alarmMsg = "Sensor not responding to heating";
      underheatCount++;
      if (underheatCount >= UNDERHEAT) alarm = true;
    }
    else {
      underheatCount = 0;
    }
    if (hotin < DISCON) {
      alarmMsg = "Sensor connection broken/unstable";
      alarm = true;
    }
    lastTempTime = currentTime;
    lastTemp = hotin;
  }
  return alarm;
}

void hotendParse(String str) {
  hotset = str.toDouble();
}

void hotendSetup() {
  // set pinmodes
  pinMode(TEMPIN, INPUT);
  pinMode(HOTEND, OUTPUT);

  // bang-bang mode when more than 4 degrees off target
  hotpid.setBangBang(4);
  // 4 second interval
  hotpid.setTimeStep(PIDINT);
  hotset = 0;
  lastTempTime = micros();
  hotin = readTemp();
  lastTemp = hotin;
  lastPIDTime = 0;
  underheatCount = 0;
}

void runHotend() {
  // if (Serial.available() > 0) {
  //   hotset = Serial.parseFloat();
  // }

  uint32_t currentTime = millis();
  if (currentTime - lastPIDTime > PIDINT) {
    hotin = readTemp();
    hotpid.run();
    analogWrite(HOTEND, hotout);
    lastPIDTime = currentTime;
  }

  // Serial.print(hotin);
  // Serial.print(", ");
  // Serial.print(hotset);
  // Serial.print(", ");
  // Serial.println(hotout);

  if(runawayCheck()) {
    stop_message("Thermal runaway alarm tripped. Reason: " + alarmMsg);
    analogWrite(HOTEND, 0);
    while(1);
  }
}
