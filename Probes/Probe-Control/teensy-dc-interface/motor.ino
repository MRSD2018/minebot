// MOTOR FUNCTIONS
int motorDirection = 1;
int speedScale = 30;            // TUNE
float zeroPosition = 0;
int countsPerRotation = 400;    // CHANGE WITH NEW ENCODER
int rads = 2 * PI;

int maxPWM = 230;
int minPWM = 0;

int retpwm = 0;

int setPWM(float pwm) {  

  pwm = abs(pwm);
  pwm = pwm/100*maxPWM; // convert % to PWM
  
  if (pwm < minPWM)
    return minPWM;
  else if (pwm > maxPWM)
    return maxPWM;
  else return pwm;
}

void setupMotor() {
  pinMode(motorPWM, OUTPUT);
  pinMode(motorDIR, OUTPUT);
}

void runMotor(float speed) { // Percentage

  // Set direction
  if (speed*motorDirection < 0)
    digitalWrite(motorDIR, LOW);
  else
    digitalWrite(motorDIR, HIGH);
  
  // Set direciton
  retpwm = setPWM(speed);
  analogWrite(motorPWM, setPWM(speed));
}

void setMotorZero() {
  zeroPosition = getRawMotorPosition();
}

float getRawMotorPosition() { // UPDATE WITH ENCODER
//  return (float)stepper.currentPosition() * motorDirection / countsPerRotation * rads;
  return 0.0;
}

float getMotorPosition() {
  return getRawMotorPosition() - zeroPosition;
}

float getMotorSpeed() {
  return retpwm;  
}



