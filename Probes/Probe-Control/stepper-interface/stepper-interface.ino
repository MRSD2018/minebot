// Code for interfacing with Load Cell
// Author: David Robinson

#include "AccelStepper.h"
#include "HX711.h"

#define DOUT  3
#define CLK   2
#define STEP_PUL 9
#define STEP_DIR 8
#define UPPER_SWITCH_PIN 5
#define LOWER_SWITCH_PIN 6

HX711 loadCell(DOUT, CLK);
float tare = 0.0f;
int tareSamples = 0;

AccelStepper stepper(1, STEP_PUL, STEP_DIR);


int state;    // state machine
#define ZERO  0
#define IDLE  1
#define PROBE 2

bool debug = true;

void setup() {

  setupLoadCell();
  setupMotor();

  // Setup Limit Switches
  pinMode(UPPER_SWITCH_PIN, INPUT);
  pinMode(LOWER_SWITCH_PIN, INPUT);

  Serial.begin(2000000);

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

    runMotor(-100); // retract

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
  Serial.println("Entered Idle State");
}

void idle()
{
  //  if (debug) Serial.println("Idle State!");
  //  Serial.println(getForce());
}

float forceLimit = 1.0f;

void enterProbe() {}

void probe()
{
  float force = getForce();
  runMotor(100); // extend

  //    Serial.print("Force: ");
  //    Serial.print(getForce());
  //    Serial.print("\tPosition: ");
  //    Serial.print(getMotorPosition());
  //    Serial.println();

  if (digitalRead(LOWER_SWITCH_PIN) == 1 || force > forceLimit) {
    if (debug)  {
      Serial.print("Probing State Exited at ");
      Serial.print(getMotorPosition());
      Serial.print(" revs");
      Serial.println();
    }
    setState(IDLE);
  }
}


// LOAD CELL FUNCTIONS
void setupLoadCell() {
  // Determined with Matlab Script
  loadCell.set_scale(67590.7657); // 1/gain
  loadCell.set_offset(-0.8384);
}

float loadValue = 0.0f;
float getRawForce() {
  if (loadCell.is_ready()) {
    loadValue = loadCell.get_value();
  }

  return loadValue; // kgs
}

float getForce() {
  return getRawForce() - tare;
}

// MOTOR FUNCTIONS

int motorDirection = -1;
int speedScale = 30;
float zeroPosition = 0;
int countsPerRotation = 400;

void setupMotor() {
  stepper.setMaxSpeed(3000);
  stepper.setAcceleration(3000);
}

void runMotor(int speed) {
  stepper.setSpeed(speed * speedScale * motorDirection);
  stepper.runSpeed();
}

void setMotorZero() {
  zeroPosition = getRawMotorPosition();
}

float getRawMotorPosition() {
  return (float)stepper.currentPosition() * motorDirection / countsPerRotation;
}

float getMotorPosition() {
  return getRawMotorPosition() - zeroPosition;
}

// SERIAL INPUT FUINCTIONS
void serialControl() {

  if (Serial.available() > 0) {
    String input = Serial.readString();

    switch (input.charAt(0))
    {
      case 'p':
        setState(PROBE);
        if (debug) Serial.println("Probe Command Received");
        break;
      case 's':
        setState(IDLE);
        if (debug) Serial.println("Stop Command Recieved");
        break;
      case 'z':
        setState(ZERO);
        if (debug) Serial.println("Zero Command Recieved");
        break;
      default:
        if (debug) errorMessage(input);
        break;
    }
  }
}

void errorMessage(String input) {
  Serial.println("\"" + input + "\" is an invalid input.");
}

