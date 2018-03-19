#import <AccelStepper.h>

//******** Define Variables ********//

//Motors
#define X_Motor 29
#define Y_Motor 30
extern int zeroSpeed = 90;

//Rotational Steppre Motor
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
extern volatile bool arrived_X = false;
extern volatile bool arrived_Y = false;
extern volatile int init_state = 0;
extern volatile bool Initialization_Flag = false;

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
  attachInterrupt(digitalPinToInterrupt(state_but), stateChange, RISING);

  //LEDs
  pinMode(LED1, INPUT);
  digitalWrite(LED1, HIGH);
  pinMode(LED2, INPUT);
  digitalWrite(LED2, HIGH);
  pinMode(LED3, INPUT);
  digitalWrite(LED3, HIGH);
  
}

void loop() {
  if (STATE != 2 && STATE != 3 && STATE !=4) {
    digitalWrite(relay_X, LOW);
    digitalWrite(relay_Y, LOW);
    analogWrite(X_Motor, zeroSpeed);
    analogWrite(Y_Motor, zeroSpeed);
  }
  
  if (STATE == 2) {
    initialize();
  }
  
  if (STATE == 3) {
    sweep();
  }

  if (STATE == 4) {
    PIDControl_Y(1000);
    PIDControl_X(500);
    Serial.print(arrived_Y); Serial.print(arrived_X);
  }
  
  delay(10);
}

void stateChange() {
  if (but_interrupt_flag){
    if (digitalRead(state_but) == HIGH){
      but_interrupt_flag = 0;
      debounce(state_but);
      
     switch(STATE) {
        case 0:
          Serial.println("State: Hold");
          STATE = 1;
          break;
          
        case 1:
          Serial.println("State: Initialize");
          digitalWrite(relay_X, LOW);
          digitalWrite(relay_Y, LOW);
          STATE = 2;
          init_state = 0;
          Initialization_Flag = false;
          break;
          
        case 2:
          Serial.println("State: Sweep");
          STATE = 3;
          analogWrite(Y_Motor, 20);
          break;
          
        case 3:
          Serial.println("State: PID");
          STATE = 4;
          break;

        case 4:
          Serial.println("State: WAITING");
          STATE = 0;
          break;
      } 
    }
  }
}

