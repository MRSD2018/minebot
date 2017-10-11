#include <TimerOne.h>

class IR_Stepper{
private:
  int analogInPin;
  int stepPin;
  int dirPin;
  int enablePin;
  int stepperCurPos;
  int stepperDesiredPos;
  bool dirHigh;
  float filterConst;
  int ir_ret;
 
public:
 IR_Stepper();
 IR_Stepper(int analogInPin, int stepPin, int dirPin, int enablePin);
 void step();
 void fast_step();
 void go_to_des_pos();
 void start_running();
 void stop_running();
 float get_current_position();
 float get_desired_position();
 float get_ir_sensor();
};

IR_Stepper::IR_Stepper(){};

IR_Stepper::IR_Stepper(int analogInPin, int stepPin, int dirPin, int enablePin)
{
  /*
  for debugging
  Serial.print("initialized");
  Serial.print('\n');
  */
  
  this->analogInPin = analogInPin;
  this->stepPin = stepPin;
  this->dirPin = dirPin;
  this->enablePin = enablePin;
  this->stepperCurPos = 0;
  this->stepperDesiredPos = 0;
  this->dirHigh = true;
  this->filterConst = 0.50f;
  pinMode(this->analogInPin, INPUT);
  pinMode(this->stepPin, OUTPUT);
  pinMode(this->dirPin, OUTPUT);
  pinMode(this->enablePin, OUTPUT);
  digitalWrite(this->dirPin, HIGH);
  digitalWrite(this->enablePin, LOW);
  
}

void IR_Stepper::step()
{
  // read infrared sensor and scale it arbitrarily
  this->ir_ret = analogRead(this->analogInPin) / 3;
  
  // filter sensor return (simple 1st order filter) and set desired position
  this->stepperDesiredPos = (int)(this->stepperDesiredPos * this->filterConst + ir_ret * (1 - this->filterConst));
}

void IR_Stepper::go_to_des_pos()
{
  int dist;
  int curPos = this->stepperCurPos;
  int desPos = this->stepperDesiredPos;
  
  /* 
  for debugging
  Serial.print(desPos);
  Serial.print('\n');  
  */
  
  // go up or go down
  if ((curPos - desPos) > 0) digitalWrite(this->dirPin, HIGH);
  else                       digitalWrite(this->dirPin, LOW);
  
  // delay to let direction change take effect, should take 500 ns
  delayMicroseconds(1);
  dist = abs(curPos - desPos);
  
  // go the distance
  for (int i = 0 ; i < dist ; i++)
  {
    digitalWrite(this->stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(this->stepPin, LOW);
    delayMicroseconds(500);
  }

  // we assume we got to the end goal (we did, right?)
  this->stepperCurPos = desPos;
}

void IR_Stepper::start_running() 
{ 
  digitalWrite(this->enablePin, LOW);
  // Timer1.resume(); 
}
void IR_Stepper::stop_running() 
{ 
  digitalWrite(this->enablePin, HIGH);
  // Timer1.stop(); 
}

float IR_Stepper::get_current_position()
{
  return this->stepperCurPos;
}

float IR_Stepper::get_desired_position()
{
  return this->stepperDesiredPos;
}

float IR_Stepper::get_ir_sensor()
{
  return this->ir_ret;
}
