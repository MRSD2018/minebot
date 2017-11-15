// Code for Probe Linear Actuation System
// Author: David Robinson

#include "HX711.h"

#define motorPWM 36
#define motorDIR 37
#define DAT  34
#define CLK   35
#define UPPER_SWITCH_PIN 18
#define LOWER_SWITCH_PIN 21

HX711 loadCell(DAT, CLK);
float tare = 0.0f;
int tareSamples = 0;

int state;    // state machine
#define ZERO  0
#define IDLE  1
#define PROBE 2

bool debug = true;
bool logging = true;

void setup() {

//  setupLoadCell();
//  attachInterrupt(digitalPinToInterrupt(DAT), readForce, FALLING);
  
//  setupMotor();
//
//  // Setup Limit Switches
//  pinMode(UPPER_SWITCH_PIN, INPUT);
//  pinMode(LOWER_SWITCH_PIN, INPUT);

  Serial.begin(38400);
  Serial.println("System Initialized...");

  setState(0); // initially set to zero state
}

void loop() {

  serialControl(); // External Control

  switch (state) // State Machine
  {
    case ZERO: zero();
      break;
    case IDLE: idle();
      break;
    case PROBE: probe();
      break;
    default:
      break;
  }
}

void setState(int s) {
  switch (s) // State Machine
  {
    case ZERO:
      state = ZERO;
      enterZero();
      break;
    case IDLE:
      state = IDLE;
      enterIdle();
      break;
    case PROBE:
      state = PROBE;
      enterProbe();
      break;
    default:
      break;
  }
}

void enterZero() {
  tare = 0;
  tareSamples = 0;
}

void zero() {

  if (digitalRead(UPPER_SWITCH_PIN) == 0) {

    runMotor(-50); // retract at 50%

    //    tare += getRawForce();
    //    ++tareSamples;
  }
  else {
    //    tare = tare / tareSamples;
    tare = getRawForce(); // blocking tmp sln
    if (debug) {
      Serial.print("Limit Switch Found at ");
      Serial.print(getMotorPosition());
      Serial.print(" revs, Tare at ");
      Serial.print(tare);
      Serial.print(" kg(s)");
      Serial.println();
    }
    setMotorZero();
    setState(IDLE);
  }
}

void enterIdle() {
  if (debug) Serial.println("Entered Idle State");
}

void idle()
{
  runMotor(0);  
}

// PROBE FUNCTIONS
float forceLimit = 30.0f; // safety
int speedReductionFactor = 2;

void enterProbe() {}

void probe()
{
  float force = getForce();
  if (force < 0) force = 0; // Only Positive Values

  float speedAdjustment = 1/(force*speedReductionFactor+1);  
  runMotor(100*speedAdjustment); // extend

  // Exit Conditions
  if (digitalRead(LOWER_SWITCH_PIN) == 1) {
    if (debug)
      Serial.println("EVENT: Exited at end of linear travel");
    setState(IDLE);
  }
  else if (getRawForce() > forceLimit) {
    if (debug)
      Serial.println("EVENT: Exited because safe load cell force exceeded");
    setState(IDLE);
  }

  if (logging)
  {
    // for output to logger!
    Serial.print(millis());
    Serial.print("\t");
    Serial.print(getMotorPosition());
    Serial.print("\t");
    Serial.print(getForce());
    Serial.print("\t");
    Serial.print(getForceDerivative());
    Serial.print("\t");
    Serial.print(speedAdjustment);
    Serial.println();
  }
}
