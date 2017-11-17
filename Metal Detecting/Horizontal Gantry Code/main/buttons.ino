int STATE;

//switch variables
volatile int but_interrupt_flag = 0;
const int debounce_time = 50;  //ms
int last_switch_time = 0;
const int debounce_timeout = 1000; // ms

//limit switches
volatile int limitNear;
volatile int limitFar;
volatile int limitIndicator = 0;

/**************************************************************************/
/*
    Setup buttons
    *State change button
    *Near limit switch
    *Far limit switch
*/
/**************************************************************************/
void buttonSetup() {
  STATE = 0;
  
  pinMode(stateSwitch, INPUT);
  attachInterrupt(digitalPinToInterrupt(stateSwitch), stateChange, RISING);

  pinMode(nearLimit, INPUT);
  attachInterrupt(digitalPinToInterrupt(nearLimit), LimitNear, RISING);
  
  pinMode(farLimit, INPUT);
  attachInterrupt(digitalPinToInterrupt(farLimit), LimitFar, RISING);
}

/**************************************************************************/
/*
    Debounce button
*/
/**************************************************************************/
void debounce(int button){
  int cur_time = millis();
  if ( (cur_time - last_switch_time) > debounce_timeout){
    last_switch_time = cur_time;
  }
}

/**************************************************************************/
/*
    State machine for determining gantry state.
*/
/**************************************************************************/
void stateChange() {
  if (but_interrupt_flag == 0) {
    switch(STATE) {
        case 0:
          Serial.println("State: Initialize");
          STATE = 1;
          break;
          
        case 1:
          Serial.println("State: Sweep");
          STATE = 2;
          break;
          
        case 2:
          Serial.println("State: Position Control");
          STATE = 3;
          break;
          
        case 3:
          Serial.println("State: WAITING");
          STATE = 0;
          break;
    }
    but_interrupt_flag = 1;
  }
}


/**************************************************************************/
/*
    IF STATE == Initialize
    *Set current position as the system "length"
    ELSE
    *Stop system 
*/
/**************************************************************************/
void LimitNear() {
  if (but_interrupt_flag == 0) {
    if (STATE == 1 && limitIndicator == 0) {
      limitNear = encoderTicks;
      Serial.print("Near Limit ==> "); Serial.println(limitNear);
      limitIndicator = 1;
    }
    if (STATE != 1) {
      STATE = 0;
    }
    but_interrupt_flag = 2;
  }
}


/**************************************************************************/
/*
    IF STATE == Initialize
    *Set current position as the system "zero"
    ELSE
    *Stop system 
*/
/**************************************************************************/
void LimitFar() {
  if (but_interrupt_flag == 0) {
    if (STATE == 1 && limitIndicator == 1) {
      limitFar = encoderTicks;
      Serial.print("Far Limit ==> "); Serial.println(limitFar);
      limitIndicator = 2; 
    }
    if (STATE != 1) {
      analogWrite(PWM, 177);
      STATE = 0;
    }
    but_interrupt_flag = 3;
  }
}

