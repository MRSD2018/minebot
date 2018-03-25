#import <AccelStepper.h>

AccelStepper stepper_probe(1, 21, 19) ; //set rotary stepper driver as Accel stepper type with DIR == 17, and PULSE ==18
AccelStepper stepper_rot(1, 20, 17) ;
#define enable 18

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  stepper_rot.setMaxSpeed(1000);
  stepper_rot.setAcceleration(2000);
  stepper_rot.setSpeed(400);
  stepper_rot.setCurrentPosition(0.0); 
//  stepper_rot.setEnablePin(18);
  pinMode(enable, OUTPUT);

  stepper_probe.setMaxSpeed(1000);
  stepper_probe.setAcceleration(10000);
  stepper_probe.setSpeed(800);
  stepper_probe.setCurrentPosition(0.0); 
}

void loop() {
  // put your main code here, to run repeatedly:
  
//  stepper_rot.disableOutputs();
//  delay(5000);
//  stepper_rot.enableOutputs();
//  stepper_rot.runSpeed();
//  digitalWrite(enable, LOW);
  stepper_rot.runToNewPosition(400);
//  stepper_probe.runToNewPosition(800);
  Serial.print(stepper_rot.currentPosition());
  Serial.print('\n');
  delay(1000);
  stepper_rot.runToNewPosition(800);
//  stepper_probe.runToNewPosition(-800);
  Serial.print(stepper_rot.currentPosition());
  Serial.print('\n');
  delay(1000);
//  digitalWrite(enable, HIGH);
//  delay(1000);
//  stepper_rot.stop();
//  Serial.print(stepper_rot.isRunning());
//  delay(5000);
}
