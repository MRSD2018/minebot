//ROS message publishing
#include <ros.h>
#include <ros/time.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Point32.h>

/*Define Pins*/
//motor
#define PWM 30

//switches
#define stateSwitch 28
#define nearLimit 27
#define farLimit 26

//encoder
#define channelAPin 34
#define channelBPin 33

/*Variables*/
//ROS Message publisher
ros::NodeHandle nh;
geometry_msgs::Point32 gantry_status;
ros::Publisher gantryStatPub("gantryStat", &gantry_status);

//Gantry state
extern int STATE;

//encoder variables
extern volatile bool channelAVal;
extern volatile bool channelBVal;
extern volatile int encoderTicks;
int posInTicks;
float posInCM;

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

//initialization 
volatile int dist;
volatile float distInCM;

//motor variables
int zeroSpeed = 90;

//Contral variables
int newPos;
double nowTime;
double prevTime=0;
double dt;
float kd = .3;
float kp = .3;
float ki = 0;
int currentPos;
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
  gantryStatPub.publish(&gantry_status);
  nh.spinOnce();  

  // put your main code here, to run repeatedly:
  if (but_interrupt_flag){
    if (but_interrupt_flag == 1) {debounce(stateSwitch);}
    if (but_interrupt_flag == 2) {debounce(nearLimit);}
    if (but_interrupt_flag == 3) {debounce(farLimit);}
    but_interrupt_flag = 0;
  }
  
  //State 0 
  if (STATE == 0) {
    analogWrite(PWM, zeroSpeed);
    limitIndicator = 0;
  }
  if (STATE == 1){
    analogWrite(PWM, zeroSpeed);
    initialize();
  }
  if (STATE == 2){
    sweep();
  }
  if (STATE ==3) {
    posControl();
  }
    
}

