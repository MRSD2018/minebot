// Test Framework for GUI integration
// Author: David Robinson

int ledPin = 13; // for testing purposes
bool LED = false; // for testing purposes

void setup() {
  Serial.begin(9600); // Establish serial connection
  pinMode(ledPin, OUTPUT); // for testing purposes
}

void initDCPosition()
{
  Serial.println("Starting DC Motor Position Control...");
  // Code Goes Here
}

void runDCPosition()
{
  // Code Goes Here (Runs Every Loop)
}

void initDCVelocity()
{
  Serial.println("Starting DC Motor Velocity Control...");
  // Code Goes Here
}

void runDCVelocity()
{
  // Code Goes Here (Runs Every Loop)
}

void initStepper()
{
  Serial.println("Starting Stepper Motor Position Control...");
  // Code Goes Here
}

void runStepper()
{
  // Code Goes Here (Runs Every Loop)
}

void initServo()
{
  Serial.println("Starting Servo Motor Position Control...");
  // Code Goes Here
}

void runServo()
{
  // Code Goes Here (Runs Every Loop)
}

void handleSerial()
{
  if (Serial.available() > 0) {
    String input = Serial.readString();
    stateMachineSwitch(input);
  }
}

int state = 0; // DC Position Intial State
void stateMachineSwitch(String input)
{
  if (input == "A") // DC Position
  {
    state = 0;
    initDCPosition();
  }
  else if (input == "B") // DC Velocity
  {
    state = 1;
    initDCVelocity();
  }
  else if (input == "C") // Stepper Position
  {
    state = 2;
    initStepper();
  }
  else if (input == "D") // Servo Position
  {
    state = 3;
    initServo();
  }
}

void stateMachine()
{
  switch(state)
  {
      case 0: // DC Position
          runDCPosition();
          break;
      case 1: // DC Velocity
          runDCVelocity();
          break;
      case 2: // Stepper Position
          runStepper();
          break;
      case 3: // Servo Position
          runServo();
          break;
      default: break;
  }
}

int iterator = 0;

void loop() {
  handleSerial();
  stateMachine();

  iterator += 1;

  Serial.print("X_"); // for testing purposes
  Serial.print(iterator); // for testing purposes
  Serial.print("_"); // for testing purposes
  Serial.print(iterator*1.2); // for testing purposes
  Serial.print("_"); // for testing purposes
  Serial.print(iterator); // for testing purposes
  Serial.print("_"); // for testing purposes
  Serial.print(0); // for testing purposes - Zero = CW, One = ACW
  Serial.print("_"); // for testing purposes
  Serial.println(state); // for testing purposes
  
}

