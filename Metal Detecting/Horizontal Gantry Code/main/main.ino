//ROS message publishing
#include <ros.h>
#include <ros/time.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Point32.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/Int16.h>

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
long loopTime; //For calculating publish rate

//Subscriber variables
int probeStat = 0;
int desiredPos;
int stateReq;

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
float kd = .5;
float kp = 3;
float ki = 0;
//int currentPos;
int positionError;
double positionErrorSum;
double pwmToWritePos;
int motorInputScaledPos;
double prevPositionError;
int newSpeed;

//Absolutely so many variables for the rotary encoders
extern int clock_pin; // output to clock
extern int CSn_pin; // output to chip select
extern int input_pin; // read AS5045
extern int input_stream; // one bit read from pin
extern long packed_data; // two bytes concatenated from inputstream
extern long angle; // holds processed angle value
extern float angle_Float;
extern float true_angle;
extern float calib_angle;
extern float offset_angle; //17.02
extern float resting_angle;
extern float rotary_angle;
extern float starting_angle;
extern long printing_angle;
extern long angle_mask; // 0x111111111111000000: mask to obtain first 12 digits with position info
extern long status_mask; // 0x000000000000111111; mask to obtain last 6 digits containing status info
extern long status_bits; // holds status/error information
extern int DECn; // bit holding decreasing magnet field error data
extern int INCn; // bit holding increasing magnet field error data
extern int OCF; // bit holding startup-valid bit
extern int COF; // bit holding cordic DSP processing error data
extern int LIN; // bit holding magnet field displacement error data
extern int debug; // SET THIS TO 0 TO DISABLE PRINTING OF ERROR CODES
int wheelEncoderDist;

//temporary variables
int count=0;


/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup() {
  
  rosSetup();
  
//  Serial.begin(9600);

  //Initialize indicator LEDs
  Initialized = 1;
  posDesiredArrived = 0;
  stateReq =3;
  dist = 1700;
  limitNear = 0;
  limitFar = -1700;
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, HIGH);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, HIGH);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED3,HIGH);

  motorSetup();
  buttonSetup();
  encoderSetup();
  rotaryEncoderSetup();
  
  Serial.print("System Ready");
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete 
*/
/**************************************************************************/
void loop() {

  loopTime = millis();
  
  //ROS gantry_status message publisher
  readyGantryStatus();
  nh.spinOnce();  

  // debounce appropriate switch
  if (but_interrupt_flag){
//    if (but_interrupt_flag == 1) {debounce(stateSwitch);}
    if (but_interrupt_flag == 2) {debounce(nearLimit);}
    if (but_interrupt_flag == 3) {debounce(farLimit);}
  }

  //get rotary encoder data
  rotary_encoder();
  
/**************************************************************************/
/*
    State 0 ==> System "waiting". Motor speed set to 0. Initiazization reset.
*/
/**************************************************************************/
  if (stateReq == 0) {
    analogWrite(PWM, zeroSpeed);
    limitIndicator = 0;
    
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  }
  
/**************************************************************************/
/*
    State 1 ==> Initialize by flipping motor-side, then far-side limit switches
*/
/**************************************************************************/
  if (stateReq == 1){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    analogWrite(PWM, zeroSpeed);
    initialize();
  }

/**************************************************************************/
/*
    State 2 ==> Sweep motor WARNING: Motor will move immediately if powered
*/
/**************************************************************************/
  if (stateReq == 2){
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
  if (stateReq == 3) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    posControl();
  }

//  delay(100 -  (millis() - loopTime));
    
}

