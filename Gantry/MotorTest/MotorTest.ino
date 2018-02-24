//Pins
//Switches
//X axis
#define limSwitch1 34
#define limSwitch2 35
//Y axis
#define limSwitch3 13
#define limSwitch4 14


//Motors
#define X_Motor 30
#define Y_Motor 29

//Encoder X
#define X_channelAPin 38
#define X_channelBPin 39
volatile bool X_channelAVal;
volatile bool X_channelBVal;
int X_encoderTicks = 0;

//Encoder Y
#define Y_channelAPin 15
#define Y_channelBPin 16
volatile bool Y_channelAVal;
volatile bool Y_channelBVal;
int Y_encoderTicks = 0;

//LEDs
#define LED1 25
#define LED2 24
#define LED3 26

//Motor Variables
int zeroSpeed = 75;
int speed_X = 115;
int speed_Y = zeroSpeed;

//Timing Variables
int debounce_time = 100;

//ISR Variables
bool button_interrupt_flag = 0;

//State
int state = 0;

//Initialization Sequence Variables
int X_counts = 0;
int Y_counts = 0;

void setup() {
  Serial.begin(9600);

  //Switch Init
  pinMode(limSwitch1, INPUT);
  pinMode(limSwitch2, INPUT);
  pinMode(limSwitch3, INPUT);
  pinMode(limSwitch4, INPUT);

  //Motor Init
  pinMode(X_Motor, OUTPUT);
  analogWrite(X_Motor, 115);
  pinMode(Y_Motor, OUTPUT);
  analogWrite(Y_Motor, zeroSpeed);

  //Encoder Init
  //X
  pinMode(X_channelAPin, INPUT);
  pinMode(X_channelBPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(X_channelBPin), X_encoderCount, CHANGE);

  //Y
  pinMode(Y_channelAPin, INPUT);
  pinMode(Y_channelBPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(Y_channelBPin), Y_encoderCount, CHANGE);
  
  //LED Init
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, LOW);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED3, LOW);

  //Limit ISR
  attachInterrupt(digitalPinToInterrupt(limSwitch1),buttonState, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch2),buttonState, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch3),buttonState, RISING);
  attachInterrupt(digitalPinToInterrupt(limSwitch4),buttonState, RISING);
}

void X_encoderCount(){
  X_channelAVal = digitalRead(X_channelAPin);
  X_channelBVal = digitalRead(X_channelBPin);
  
//  Serial.print(channelBVal); Serial.print(channelAVal); Serial.println(encoderTicks);
  if (X_channelAVal == X_channelBVal) {
    X_encoderTicks++;
  }
  else {
    X_encoderTicks--;
  }
}

void Y_encoderCount(){
  Y_channelAVal = digitalRead(Y_channelAPin);
  Y_channelBVal = digitalRead(Y_channelBPin);
  
//  Serial.print(channelBVal); Serial.print(channelAVal); Serial.println(encoderTicks);
  if (Y_channelAVal == Y_channelBVal) {
    Y_encoderTicks++;
  }
  else {
    Y_encoderTicks--;
  }
}

void buttonPress(){
  if (!button_interrupt_flag){
    button_interrupt_flag = 1;
  }
}

void buttonState() {
  if (button_interrupt_flag) {
    if (digitalRead(limSwitch1) == HIGH) {
      speed_X =115;
      analogWrite(X_Motor, speed_X);
      debounce(limSwitch1);
      button_interrupt_flag = 0;
      X_counts = abs(X_encoderTicks);
      state = state + 1; 
    }
    if (digitalRead(limSwitch2) == HIGH) {
      speed_X = 30;
      analogWrite(X_Motor, speed_X);
      debounce(limSwitch2);
      button_interrupt_flag = 0;
      X_encoderTicks = 0;
      state = state + 1;
    }
    if (digitalRead(limSwitch3) == HIGH) {
      speed_Y =115;
      analogWrite(Y_Motor, speed_Y);
      debounce(limSwitch3);
      button_interrupt_flag = 0;
      Y_counts = abs(Y_encoderTicks);
      state = state + 1; 
    }
    if (digitalRead(limSwitch4) == HIGH) {
      speed_Y =40;
      analogWrite(Y_Motor, speed_Y);
      debounce(limSwitch4);
      button_interrupt_flag = 0;
      Y_encoderTicks = 0;
      state = state + 1; 
    }
    
  }
}

//debouncing
void debounce(int button)
{
  Serial.print("Debouncing "); Serial.println(button);
  unsigned long now = millis ();
  while (digitalReadFast(button) == HIGH || (millis() - now) <= debounce_time)
  {
    if(digitalReadFast(button) == HIGH){now = millis();}
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonPress();
  if (state == 2) {
    if (X_encoderTicks <= X_counts/2){
      speed_X =zeroSpeed; analogWrite(X_Motor, speed_X);
      speed_Y = 30; analogWrite(Y_Motor, speed_Y);
      } 
  }
  if (state >= 4) {
    if (Y_encoderTicks <= Y_counts/2){
      speed_Y =zeroSpeed; analogWrite(Y_Motor, speed_Y);
      analogWrite(X_Motor, zeroSpeed);
      } 
  }
  Serial.print("X speed:  "); Serial.print(speed_X);Serial.print("  Y speed:  "); Serial.println(speed_Y);
  Serial.print("X axis:  "); Serial.print(X_encoderTicks);Serial.print("  Y axis:  "); Serial.println(Y_encoderTicks);
  Serial.println(state);
  delay(10);
}

//SwitchIST




