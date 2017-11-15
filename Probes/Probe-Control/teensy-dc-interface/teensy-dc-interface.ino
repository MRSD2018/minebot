// Code for Probe Linear Actuation System
// Author: David Robinson

#include "HX711.h"

#define motorPWM 36
#define motorDIR 37
#define DOUT  34
#define CLK   35
#define UPPER_SWITCH_PIN 18
#define LOWER_SWITCH_PIN 21

HX711 loadCell(DOUT, CLK);
float tare = 0.0f;
int tareSamples = 0;

int state;    // state machine
#define ZERO  0
#define IDLE  1
#define PROBE 2

bool debug = true;
bool logging = true;

void setup() {

  setupLoadCell();
  setupMotor();

  // Setup Limit Switches
  pinMode(UPPER_SWITCH_PIN, INPUT);
  pinMode(LOWER_SWITCH_PIN, INPUT);

  Serial.begin(9600);

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
  if (force < 0) force = 0;

  float speedAdjustment = 1/(force*speedReductionFactor+1);  
  runMotor(100*speedAdjustment); // extend

  if (logging)
  {
    // for output to logger!
    Serial.print(millis());
    Serial.print("\t");
    Serial.print(getMotorPosition());
    Serial.print("\t");
    Serial.print(getForce());
    Serial.print("\t");
    Serial.print(speedAdjustment);
    Serial.println();
  }

  if (digitalRead(LOWER_SWITCH_PIN) == 1 || getRawForce() > forceLimit) {
    if (debug)  {
      Serial.print("Probing State Exited at ");
      Serial.print(getMotorPosition());
      Serial.print(" revs");
      Serial.println();
    }
    setState(IDLE);
  }
}
