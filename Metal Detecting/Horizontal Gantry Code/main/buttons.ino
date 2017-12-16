//int STATE;

//switch variables
volatile int but_interrupt_flag = 0;
const int debounce_time = 50;  //ms
int last_switch_time = 0;
const int debounce_timeout = 500; // ms

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
//  STATE = 0;
  
  pinMode(stateSwitch, INPUT);
//  attachInterrupt(digitalPinToInterrupt(stateSwitch), stateChange, RISING);

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
  if (digitalRead(button) == HIGH){
    last_switch_time = millis();
  }
  if ( (cur_time - last_switch_time) > debounce_timeout){
    last_switch_time = cur_time;
    but_interrupt_flag = 0;
  }
}

/**************************************************************************/
/*
    State machine for determining gantry state.
*/
/**************************************************************************/
//void stateChange() {
//  if (but_interrupt_flag == 0) {
//    switch(STATE) {
//        case 0:
//          Serial.println("State: Initialize");
//          digitalWrite(LED1, HIGH);
//          digitalWrite(LED2, LOW);
//          digitalWrite(LED3, LOW);
//          STATE = 1;
//          break;
//          
//        case 1:
//          Serial.println("State: Sweep");
//          digitalWrite(LED1, LOW);
//          digitalWrite(LED2, HIGH);
//          digitalWrite(LED3, LOW);
//          STATE = 2;
//          break;
//          
//        case 2:
//          Serial.println("State: Position Control");
//          digitalWrite(LED1, LOW);
//          digitalWrite(LED2, LOW);
//          digitalWrite(LED3, HIGH);
//          STATE = 3;
//          break;
//          
//        case 3:
//          STATE = 3;
////          Serial.println("State: WAITING");
////          digitalWrite(LED1, HIGH);
////          digitalWrite(LED2, HIGH);
////          digitalWrite(LED3, HIGH);
////          STATE = 0;
//          break;
//    }
//    but_interrupt_flag = 1;
//  }
//}


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
    if (stateReq == 1 && limitIndicator == 0) {
      limitNear = encoderTicks;
      Serial.print("Near Limit ==> "); Serial.println(limitNear);
      limitIndicator = 1;
      digitalWrite(LED2, HIGH);
    }
    if (stateReq != 1) {
      analogWrite(PWM, zeroSpeed);
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
    if (stateReq == 1 && limitIndicator == 1) {
      limitFar = encoderTicks;
      Serial.print("Far Limit ==> "); Serial.println(limitFar);
      limitIndicator = 2; 
      digitalWrite(LED3, HIGH);
    }
    if (stateReq != 1) {
      analogWrite(PWM, zeroSpeed);
    }
    but_interrupt_flag = 3;
  }
}

