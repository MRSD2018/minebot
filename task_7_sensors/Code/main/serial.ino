
void handleSerial()
{
  if (Serial.available() > 0) {
    String input = Serial.readString();
    parseSerial(input);
  }
}

void parseSerial(String input)
{
  if (input == "A") // DC Position
  {
    STATE = 0;
    ir_stepper.stop_running(); 
  }
  else if (input == "B") // DC Velocity
  {
    STATE = 1;
    ir_stepper.stop_running(); 
  }
  else if (input == "C") // Stepper Position
  {
    STATE = 2;
    ir_stepper.start_running(); 
  }
  else if (input == "D") // Servo Position
  {
    STATE = 3;
    ir_stepper.stop_running(); 
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
  Serial.println(STATE);
}


