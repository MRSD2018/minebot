#include "dcMotor.h"
#include <digitalWriteFast.h> //thank you jrraines!
//https://code.google.com/archive/p/digitalwritefast/downloads
#include <Servo.h>
#include "ForceSensor.h"
#include "dcMotor.h"
#include "ir_stepper.h"
#include <MsTimer2.h>

//button
#define pushButton 2

//motor
#define PWM 6
#define L1 4
#define L2 9
#define channelAPin 3
#define channelBPin 7

//forceSensor
#define forceSensor A1

//servo
#define pot A0
#define servoPWM 5

//slotSensor
#define slotSensor 12

//irSensor
#define irSensor A2
#define stepperStep 8
#define stepperDir 10
#define stepperEn 11

//items
Servo myservo;
dcMotor motor(PWM, L1, L2, channelAPin, channelBPin);
ForceSensor force(forceSensor);
IR_Stepper ir_stepper;

//button
extern volatile bool but_interrupt_flag;
extern const int debounce_time; //ms

//dcEncoder
extern volatile bool channelAVal;
extern volatile bool channelBVal;
extern volatile long encoderTicks;

//potServo
extern int potIn;
extern int Angle;

//states
int STATE = 0;

//serial
bool useSensor = true;
float manualInput = 0.0f;
float serialMotorActual;
float serialMotorDesired; // setpoint
float serialSensor;
bool serialMotorDirection; // Zero = CW, One = ACW

void setup() {
  
  Serial.setTimeout(100);
  Serial.begin(9600);  
  setupButton();
  
  // SetupPotServo
  setupPotServo();
  myservo.attach(servoPWM);
  
  setupIRStepper();
  setupEncoder();
  setupForce();

}

void loop() {
  checkButtonForState(); //checks if button pressed, debouncing needed, and changes STATE if needed

  handleSerial();
  sendSerialData();  

  switch(STATE)
  {
    case 0: //DC pos control
      posForce();
      break;
    case 1: //DC vel control, direction affected by slotSensor
      velForce();
      break;
    case 2: //ir Stepper
      irStepper();
      break;
    case 3: //pot Servo
      potServo();
      break;

  }  
}
