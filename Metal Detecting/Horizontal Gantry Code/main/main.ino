//ROS message publishing
#include <ros.h>
#include <ros/time.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Point32.h>
#include <std_msgs/Int16MultiArray.h>

/*Define Pins*/
//motor
#define PWM 30

//LEDs
#define LED1 26
#define LED2 24
#define LED3 25

//switches
#define stateSwitch 33
#define nearLimit 34
#define farLimit 35

//encoder
#define channelAPin 38
#define channelBPin 39

/*Variables*/
//ROS Message publisher
ros::NodeHandle nh;
std_msgs::Int16MultiArray gantry_stat;
ros::Publisher pub_gantry( "gantryStat", &gantry_stat);

//Subscriber variables
int probeStat = 0;
int desiredPos;

//Gantry state
extern int STATE;
int Initialized;
int posDesiredArrived;

//encoder variables
extern volatile bool channelAVal;
extern volatile bool channelBVal;
extern volatile int encoderTicks;
int posInTicks;
float posInMM;

//encoder constants
static float encoderTicksPerRotation = 659.232/2;
static float teethPerRotation = 72;
static float toothPitch = .2;

//switch variables
extern volatile int but_interrupt_flag;
extern const int debounce_time;  //ms
extern int last_switch_time;
extern const int debounce_timeout; // ms

//limit switches
extern volatile int limitNear;
extern volatile int limitFar;
extern volatile int limitIndicator;

//initialization constants
volatile int dist;
volatile float distInMM;

//motor variables
int zeroSpeed = 75;

//Contral variables
int newPos;
double nowTime;
double prevTime=0;
double dt;
float kd = .1;
float kp = .5;
float ki = 0;
//int currentPos;
int positionError;
double positionErrorSum;
double pwmToWritePos;
int motorInputScaledPos;
double prevPositionError;
int newSpeed;


/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup() {
  rosSetup();
  
//  Serial.begin(9600);

  //Initialize indicator LEDs
  Initialized = 0;
  posDesiredArrived = 0;
  desiredPos = 0;
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, HIGH);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, HIGH);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED3,HIGH);

  motorSetup();
  buttonSetup();
  encoderSetup();
  
  
  Serial.print("System Ready");
 
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete 
*/
/**************************************************************************/
void loop() {
  //ROS gantry_status message publisher
  readyGantryStatus();
  nh.spinOnce();  

  // debounce appropriate switch
  if (but_interrupt_flag){
    if (but_interrupt_flag == 1) {debounce(stateSwitch);}
    if (but_interrupt_flag == 2) {debounce(nearLimit);}
    if (but_interrupt_flag == 3) {debounce(farLimit);}
  }
  
/**************************************************************************/
/*
    State 0 ==> System "waiting". Motor speed set to 0. Initiazization reset.
*/
/**************************************************************************/
  if (STATE == 0) {
    analogWrite(PWM, zeroSpeed);
    limitIndicator = 0;
  }
  
/**************************************************************************/
/*
    State 1 ==> Initialize by flipping motor-side, then far-side limit switches
*/
/**************************************************************************/
  if (STATE == 1){
    digitalWrite(LED1, HIGH);
    analogWrite(PWM, zeroSpeed);
    initialize();
  }

/**************************************************************************/
/*
    State 2 ==> Sweep motor WARNING: Motor will move immediately if powered
*/
/**************************************************************************/
  if (STATE == 2){
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    sweep();
  }

/**************************************************************************/
/*
    State 3 ==> Position control
*/
/**************************************************************************/
  if (STATE ==3) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    posControl();
  }
    
}

