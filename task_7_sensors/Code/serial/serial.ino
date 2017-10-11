// Test Framework for GUI integration
// Author: David Robinson

int ledPin = 13; // for testing purposes
bool LED = false; // for testing purposes

void setup() {
  Serial.begin(9600); // Establish serial connection
}

void initDCPosition()
{
  Serial.println("Starting DC Motor Position Control...");
  // Code Goes Here
}

void runDCPosition()
{
  // Code Goes Here (Runs Every Loop)

    // Assign Serial Data
//  serialMotorActual = ?;
//  serialMotorDesired = ?;
//  serialSensor = ?;
//  serialMotorDirection = ?;
}

void initDCVelocity()
{
  Serial.println("Starting DC Motor Velocity Control...");
  // Code Goes Here
}

void runDCVelocity()
{
  // Code Goes Here (Runs Every Loop)

    // Assign Serial Data
//  serialMotorActual = ?;
//  serialMotorDesired = ?;
//  serialSensor = ?;
//  serialMotorDirection = ?;
}

void initStepper()
{
  Serial.println("Starting Stepper Motor Position Control...");
  // Code Goes Here
}

void runStepper()
{
  // Code Goes Here (Runs Every Loop)

    // Assign Serial Data
//  serialMotorActual = ?;
//  serialMotorDesired = ?;
//  serialSensor = ?;
//  serialMotorDirection = ?;
}

void initServo()
{
  Serial.println("Starting Servo Motor Position Control...");
  // Code Goes Here
}

void runServo()
{
  // Code Goes Here (Runs Every Loop)

  // Assign Serial Data
//  serialMotorActual = ?;
//  serialMotorDesired = ?;
//  serialSensor = ?;
//  serialMotorDirection = ?;
}

void handleSerial()
{
  if (Serial.available() > 0) {
    String input = Serial.readString();
    parseSerial(input);
  }
}

int state = 0; // DC Position Intial State
bool useSensor = true;
float manualInput = 0.0f;

void parseSerial(String input)
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
  else if (input == "S") // Use Sensor Data
  {
    useSensor = true;
  }
  else if (input.substring(0, 1) == "M")
  {
    useSensor = false;
    manualInput = input.substring(2, input.length()).toFloat();
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

float serialMotorActual;
float serialMotorDesired; // setpoint
float serialSensor;
bool serialMotorDirection; // Zero = CW, One = ACW

void sendSerialData()
{
  Serial.print("X_");
  Serial.print(serialMotorActual);
  Serial.print("_");
  Serial.print(serialMotorDesired);
  Serial.print("_"); 
  Serial.print(serialSensor); 
  Serial.print("_"); 
  Serial.print(serialMotorDirection); 
  Serial.print("_"); 
  Serial.println(state); 
}

void loop() {
  handleSerial();
  stateMachine();
  sendSerialData();  
}


