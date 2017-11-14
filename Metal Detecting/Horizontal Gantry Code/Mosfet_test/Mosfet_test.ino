#define PWM 30

#define stateSwitch 28
#define nearLimit 27
#define farLimit 26

#define channelAPin 34
#define channelBPin 33

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
bool active = 0;

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PWM, OUTPUT);
  digitalWrite(PWM, LOW);

  buttonSetup();
  encoderSetup();

}

void loop() {
  // put your main code here, to run repeatedly:
  if (but_interrupt_flag){
    debounce(stateSwitch);
    but_interrupt_flag = 0;
  }
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
    if (!active) { analogWrite(PWM, zeroSpeed);}
    posControl();
  }
    
}

