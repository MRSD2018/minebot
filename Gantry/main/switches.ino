//Timing variables
int debounce_time = 100;

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

void switch_setup () {
  
  //Limit ISR
  attachInterrupt(digitalPinToInterrupt(limSwitch1),limitState_Y_max, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch2),limitState_Y_min, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch3),limitState_X_max, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch4),limitState_X_max, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch5),limitState_X_min, RISING);
//  attachInterrupt(digitalPinToInterrupt(limSwitch6),limitState_X_min, RISING);

}

void debounce(int button) {
  if (Debug) {Serial.print("Debouncing "); Serial.println(button);}
  if (STATE != 1 && button != state_but) {
    if (Debug) {Serial.println("EMERGENCY STOP - LIMIT HIT");}
    STATE = 0;
  }
  if (!but_interrupt_flag) {
    unsigned long now = millis ();
    while (digitalReadFast(button) == HIGH || (millis() - now) <= debounce_time)
    {
      if(digitalReadFast(button) == HIGH){now = millis();}
    }
    but_interrupt_flag = 1;
  }
}

/*****************************************************************/
/* LIMIT ISRs
 *  Associates each limit switch with a state 
 */
 /*****************************************************************/
void limitState_Y_min() {
  if (but_interrupt_flag){ digitalWrite(relay_X, LOW);
    digitalWrite(relay_Y, HIGH);
    speed_Y = Left;
    analogWrite(Y_Motor, speed_Y);
    speed_X = zeroSpeed;
    analogWrite(X_Motor, speed_X);
    if (digitalRead(limSwitch2) == HIGH){
      but_interrupt_flag = 0;
      debounce(limSwitch2);
      if (init_state == 0) {
        init_state = 1;
      }
    }
  }
}

void limitState_Y_max() {
  if (but_interrupt_flag){
    speed_Y = Right;
    analogWrite(Y_Motor, speed_Y);
    speed_X = zeroSpeed;
    analogWrite(X_Motor, speed_X);
    if (digitalRead(limSwitch1) == HIGH){
      but_interrupt_flag = 0;
      debounce(limSwitch1);
      if  (init_state == 1) {
        init_state =  2;
      }
    }
  }
}

void limitState_X_max() {
  if (but_interrupt_flag){
    digitalWrite(relay_X, HIGH);
    digitalWrite(relay_Y, LOW);
    speed_X = Backward;
    analogWrite(X_Motor, speed_X);
    speed_Y = zeroSpeed;
    analogWrite(Y_Motor, speed_Y);
    if (digitalRead(limSwitch3) == HIGH){
      but_interrupt_flag = 0;
      debounce(limSwitch3);
      if  (init_state == 4){
        init_state = 5;
      }
    }
    else if (digitalRead(limSwitch4) == HIGH){
      but_interrupt_flag = 0;
      debounce(limSwitch4);
      if (init_state == 4) {
        init_state = 5;
      }
    }
  }
}

void limitState_X_min() {
  if (but_interrupt_flag){
    digitalWrite(relay_X, HIGH);
    digitalWrite(relay_Y, LOW);
    speed_X = Forward;
    analogWrite(X_Motor, speed_X);
    speed_Y = zeroSpeed;
    analogWrite(Y_Motor, speed_Y);
    if (digitalRead(limSwitch5) == HIGH){
      but_interrupt_flag = 0;
      debounce(limSwitch5);
      if (init_state == 3) {
        init_state = 4;
      }
    }
//    if (digitalRead(limSwitch6) == HIGH && init_state == 3){
//      but_interrupt_flag = 0;
//      debounce(limSwitch6);
//      init_state = 4;
//    }
  }
}


