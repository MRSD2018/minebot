//******** Define Variables ********//

//Relays
#define relay_Y 3
#define relay_X 6

//Switches
#define state_but 33
extern volatile int but_interrupt_flag = 1;

//LEDs 
#define LED1 25
#define LED2 24
#define LED3 26

//State
int STATE = 0; //Start in a Hold state


//******** Setup for Main ********//
void setup() {
  Serial.begin(9600);

  motor_setup();

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
//State Machine
  if (STATE == 2) {
    initialize();
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
          digitalWrite(relay_X, LOW);
          digitalWrite(relay_Y, LOW);
          STATE = 1;
          break;
          
        case 1:
          Serial.println("State: Initialize");
          digitalWrite(relay_X, LOW);
          digitalWrite(relay_Y, LOW);
          STATE = 2;
          break;
          
        case 2:
          Serial.println("State: Position Control");
          digitalWrite(relay_X, LOW);
          digitalWrite(relay_Y, LOW);
          STATE = 3;
          break;
          
        case 3:
          Serial.println("State: WAITING");
          digitalWrite(relay_X, LOW);
          digitalWrite(relay_Y, LOW);
          STATE = 0;
          break;
      } 
    }
  }
}

