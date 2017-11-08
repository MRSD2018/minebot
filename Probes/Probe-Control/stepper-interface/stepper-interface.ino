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
AccelStepper stepper(1, STEP_PUL, STEP_DIR);

// State Machine
// 0 = Start Up
// 1 = Idle
// 2 = Probe
int state = 0; // initially set to zero state

bool debug = true;

void setup() {

  // Determined with Matlab Script
  loadCell.set_scale(67590.7657); // 1/gain
  loadCell.set_offset(-0.8384);

  setupMotor();

  // Setup Limit Switches
  pinMode(UPPER_SWITCH_PIN, INPUT);
  pinMode(LOWER_SWITCH_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {

  serialControl(); // External Control

  switch (state) // State Machine
  {
    case 0: zero();
      break;
    case 1: idle();
      break;
    case 2: probe();
      break;
    default:
      break;
  }
}

void zero()
{
  if (digitalRead(UPPER_SWITCH_PIN) == 0) {
    runMotor(-100); // retract
  }
  else {
    if (debug) {
      Serial.print("Limit Switch Found at ");
      Serial.print(getMotorPosition());
      Serial.print(" counts");
      Serial.println();
    }
    setMotorZero();
    state = 1; // idle
  }
}

void idle()
{
  //  if (debug) Serial.println("Idle State!");
}

void probe()
{
//  float force = loadCell.get_value(); // aquire force signal - affected by blocking steper calls

  if (digitalRead(LOWER_SWITCH_PIN) == 0) {
    runMotor(100);
  }
  else {
    if (debug)  {
      Serial.print("Probing State Exited at ");
      Serial.print(getMotorPosition());
      Serial.print(" counts");
      Serial.println();
    }
    state = 1; // idle
  }
}

// MOTOR FUNCTIONS
int motorDirection = -1;
int speedScale = 30;
float zeroPosition = 0;
int countsPerRotation = 400;

void setupMotor() {
  stepper.setMaxSpeed(3000);
  stepper.setAcceleration(1000);
}

void runMotor(int speed) {
  stepper.setSpeed(speed*speedScale*motorDirection); 
  stepper.runSpeed();
}

void setMotorZero() {
  zeroPosition = getRawMotorPosition();
}

float getRawMotorPosition(){
  return (float)stepper.currentPosition()*motorDirection/countsPerRotation;
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
        state = 2; // probe
        if (debug) Serial.println("Probe Command Received");
        break;
      case 's':
        state = 1; // idle
        if (debug) Serial.println("Stop Command Recieved");
        break;
      case 'z':
        state = 0; // zero
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

