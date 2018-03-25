#import <AccelStepper.h>

//******** Define Variables ********//

///Motors
#define X_Motor 29
#define Y_Motor 30
extern int zeroSpeed = 90;

//Encoder X
#define X_channelAPin 15
#define X_channelBPin 16
volatile bool X_channelAVal;
volatile bool X_channelBVal;
extern volatile int X_encoderTicks = 0;

//Encoder Y
#define Y_channelAPin 38
#define Y_channelBPin 39
volatile bool Y_channelAVal;
volatile bool Y_channelBVal;
extern volatile int Y_encoderTicks = 0;

//Rotational Stepper Motor
#define Enable 18
AccelStepper stepper_rot(1, 20, 17) ;

//Relays
#define relay_Y 3
#define relay_X 6

//Switches
#define state_but 33
//Limit Switches
//Y axis switches
#define limSwitch1 34 //Y_max
#define limSwitch2 35 //Y_min
//X_max axis switches
#define limSwitch3 14
#define limSwitch4 7
//X_min axis switches
#define limSwitch5 8
//#define limSwitch6 22

extern volatile int but_interrupt_flag = 1;

//LEDs 
#define LED1 25
#define LED2 24
#define LED3 26

//State
extern volatile int STATE = 0; //Start in a Hold state
extern volatile bool arrived = false;
extern volatile bool arrived_X = true;
extern volatile bool arrived_Y = true;
extern volatile bool arrived_R = true;
extern volatile int init_state = 0;
extern volatile bool Initialization_Flag = false;
String incoming = "";
int Y_desired = 2000;
int X_desired = 2000;
int R_desired = 0;

//Gantry Params
extern volatile int X_max = 0;
extern volatile int Y_max = 0;


//******** Setup for Main ********//
void setup() {
  Serial.begin(9600);

  motor_setup();

  switch_setup();

  //Relays
  pinMode(relay_X, OUTPUT);
  pinMode(relay_Y, OUTPUT);
  digitalWrite(relay_X, LOW);
  digitalWrite(relay_Y, LOW);

  //Switches
  pinMode(state_but, INPUT);

  //LEDs
  pinMode(LED1, INPUT);
  digitalWrite(LED1, HIGH);
  pinMode(LED2, INPUT);
  digitalWrite(LED2, HIGH);
  pinMode(LED3, INPUT);
  digitalWrite(LED3, HIGH);

  stepper_rot.setMaxSpeed(1000);
  stepper_rot.setAcceleration(2000);
  stepper_rot.setSpeed(400);
  stepper_rot.setCurrentPosition(0.0);
  pinMode(enable, OUTPUT);
  
}

void loop() {
//  set_speed(zeroSpeed, 180);
//  Serial.println('Y');
//  delay(1000);
//
//  set_speed(180, zeroSpeed);
//  Serial.println('X');

}

