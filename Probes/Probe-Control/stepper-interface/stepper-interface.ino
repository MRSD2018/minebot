// Code for interfacing with Load Cell
// Author: David Robinson

#include "AccelStepper.h"

#define UPPER_SWITCH_PIN 5
#define LOWER_SWITCH_PIN 6
#define STEP_PUL 9
#define STEP_DIR 8

AccelStepper stepper(1, STEP_PUL, STEP_DIR);

// State Machine
// 0 = Start Up
// 1 = Idle
// 2 = Probe
int state = 0;

bool debug = true;

void setup() {
  // Setup Limit Switches
  pinMode(UPPER_SWITCH_PIN, INPUT);
  pinMode(LOWER_SWITCH_PIN, INPUT);

  stepper.setMaxSpeed(3000);
  stepper.setAcceleration(1000);

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
  if (digitalRead(UPPER_SWITCH_PIN) == 0)
  {
    stepper.setSpeed(3000); // retract
    stepper.runSpeed();
  }
  else
  {
    if (debug)
    {
      Serial.print("Limit Switch Found at ");
      Serial.print(stepper.currentPosition ());
      Serial.print(" counts");
      Serial.println();
    }
    stepper.setCurrentPosition(0);
    state = 1;
  }
}

void idle()
{
  if (debug) Serial.println("Idle State!");
}

void probe()
{
  if (debug)  Serial.println(stepper.currentPosition());
  if (digitalRead(LOWER_SWITCH_PIN) == 0)
  {
    stepper.setSpeed(-2000); // extend
    stepper.runSpeed();
  }
}

void serialControl() {

  if (Serial.available() > 0) {
    String input = Serial.readString();

    Serial.print("STRING RECIEVED: ");
    Serial.println(input);

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

// Legacy...
void manualSetup()
{
  pinMode(STEP_DIR, OUTPUT); // Manual control setup
  pinMode(STEP_PUL, OUTPUT);
}

void manualControl()
{
  int speed = 300;
  if (digitalRead(LOWER_SWITCH_PIN) == 1)
    digitalWrite(STEP_DIR, LOW);
  else
    digitalWrite(STEP_DIR, HIGH);

  digitalWrite(STEP_PUL, LOW);
  delayMicroseconds(speed);
  digitalWrite(STEP_PUL, HIGH);
  delayMicroseconds(speed);
}

