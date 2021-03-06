// Code for Probe Linear Actuation System
// Author: David Robinson

#include "HX711.h"
#include <Encoder.h>

#define motorPWM 36 //35
#define motorDIR 35 //34
#define DAT   32 //37
#define CLK   31 //36
#define UPPER_SWITCH_PIN 20 //33
#define LOWER_SWITCH_PIN 23 //31

Encoder enc(15, 14);
HX711 loadCell(DAT, CLK);
float tare = 0.0f;
int tareSamples = 0;

int state;    // state machine
static int initialState = 0;
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
  setupSwitches();
  pinMode(13, OUTPUT);

  Serial.begin(9600);
}

int startupDelay = true;
void loop() {

  if (startupDelay) {
    delay(1500);
    digitalWrite(13, HIGH);
    setState(initialState);
    startupDelay = false;
  }

  serialControl(); // External Control
  readSwitches();

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
  if (!upperSwitch()) {
    runMotor(-100); // retract at 75%
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
float forceLimit = 10.0f; // safety
float finalPWM = 0.3f;
float speedReductionFactor = 0.0f;
float speedAdjustment;

float classificationThreshold = 20.0f;
float maxCalibForceMultiplier = 1.5f;

void enterProbe() {

  if (!calibrated) {
    if (debug) Serial.println("System not calibrated...");
    setState(IDLE);
  }
  else {
    if (debug) Serial.println("Entering Probe State");
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
      Serial.println("EVENT: Force Derivative Limit Exceeded, Landmine (maybe) found!");
    setState(ZERO);
    logValues();
  }
  else if (force > (maxCalibForce * maxCalibForceMultiplier)) {
    if (debug)
      Serial.println("EVENT: Max Calibration Force Exceeded, Landmine (maybe) found!");
    setState(ZERO);
    logValues();
  }
  else if (lowerSwitch()) {
    if (debug)
      Serial.println("EVENT: Exited at end of linear travel");
    setState(ZERO);
    logValues();
  }
  else if (getRawForce() > forceLimit) {
    if (debug)
      Serial.println("EVENT: Exited because safe load cell force exceeded");
    setState(ZERO);
  }

  if (logging && newValue)
  {
    logValues();
    newValue = false;
  }
}

void logValues() {
  // for output to logger!
  Serial.print(millis());
  Serial.print("\t");
  Serial.print(getMotorPosition());
  Serial.print("\t");
  Serial.print(speedAdjustment);
  Serial.print("\t");
  Serial.print(getForce());
  Serial.print("\t");
  Serial.println(getNormalizedForceDerivative());
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
  if (lowerSwitch()) {
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



