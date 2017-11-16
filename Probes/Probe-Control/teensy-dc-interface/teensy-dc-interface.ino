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
#define CALIB 3

bool debug = true;
bool logging = true;
bool newValue = false;

// CALIB FUNCTIONS
float maxCalibForce = 0.0f;
bool calibrated = false;

void setup() {

  setupLoadCell();
  attachInterrupt(digitalPinToInterrupt(DAT), readForce, FALLING);

  setupMotor();

  // Setup Limit Switches
  pinMode(UPPER_SWITCH_PIN, INPUT);
  pinMode(LOWER_SWITCH_PIN, INPUT);
  pinMode(13, OUTPUT);

  Serial.begin(9600);
}

int startupDelay = true;
void loop() {

  if (startupDelay) {
    delay(1500);
    digitalWrite(13, HIGH);
    setState(0); // initially set to zero state
    startupDelay = false;
  }

  serialControl(); // External Control

  switch (state) // State Machine
  {
    case ZERO: zero();
      break;
    case IDLE: idle();
      break;
    case PROBE: probe();
      break;
    case CALIB: calibration();
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
    case CALIB:
      state = CALIB;
      enterCalibration();
    default:
      break;
  }
}

// ZERO FUNCTIONS
long unsigned int zeroTime = 0;
unsigned int maxZeroTime = 500; //ms
bool recordTime = false;

void enterZero() {
  if (debug) Serial.println("Entered Zero State");
  tare = 0;
  tareSamples = 0;
  recordTime = false;
}

void zero() {
  if (digitalRead(UPPER_SWITCH_PIN) == 0) {
    runMotor(-75); // retract at 75%
  }
  else {
    runMotor(0);

    if (!recordTime) {
      zeroTime = millis();
      recordTime = true;
    }

    tare += getRawForce();
    ++tareSamples;

    if ( (millis() - zeroTime) > maxZeroTime) {
      tare = tare / tareSamples;
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
float finalPWM = 0.3f;
float speedReductionFactor = 0.0f;
float speedAdjustment;

float classificationThreshold = 30.0f;

void enterProbe() {

  if (!calibrated) {
    if (debug) Serial.println("System not calibrated...");
    setState(IDLE);
  }
  else {
    speedReductionFactor = (1 - finalPWM) / (finalPWM * maxCalibForce);
  }
}

void probe()
{
  float force = getForce();
  if (force < 0) force = 0; // Only Positive Values

  speedAdjustment = 1 / (force * speedReductionFactor + 1);
  runMotor(100 * speedAdjustment); // extend

  // Exit Conditions
  if (getNormalizedForceDerivative() > classificationThreshold) {
    if (debug)
      Serial.println("EVENT: Landmine (maybe) found!");
    setState(ZERO);
  }
  else if (digitalRead(LOWER_SWITCH_PIN) == 1) {
    if (debug)
      Serial.println("EVENT: Exited at end of linear travel");
    setState(ZERO);
  }
  else if (getRawForce() > forceLimit) {
    if (debug)
      Serial.println("EVENT: Exited because safe load cell force exceeded");
    setState(ZERO);
  }

  if (logging && newValue)
  {
    // for output to logger!
    Serial.print(millis());
    Serial.print("\t");
    Serial.print(getMotorPosition());
    Serial.print("\t");
    Serial.print(speedAdjustment);
    Serial.print("\t");
    Serial.print(getForce());
    Serial.print("\t");
    Serial.print(getForceDerivative());
    Serial.print("\t");
    Serial.println(getNormalizedForceDerivative());
    newValue = false;
  }
}

void enterCalibration() {
  maxCalibForce = 0.0f;
  calibrated = false;
  if (debug) Serial.println("Entered Calibration State");
}

void calibration() {

  runMotor(50);

  if (getForce() > maxCalibForce)
    maxCalibForce = getForce();

  if (debug) {
    Serial.print("Max Force: ");
    Serial.println(maxCalibForce);
  }
  // Exit Conditions

  if (digitalRead(LOWER_SWITCH_PIN) == 1) {
    if (debug) {
      Serial.print("Calibrated with Maximum Force of ");
      Serial.println(maxCalibForce);
    }
    calibrated = true;
    setState(ZERO);
  }
  else if (getRawForce() > forceLimit) {
    if (debug)
      Serial.println("EVENT: Exited because safe load cell force exceeded, Try Again!");
    setState(ZERO);
  }
}



