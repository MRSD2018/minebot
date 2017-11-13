#include "digitalWriteFast.h"

#define PWM 30
#define Switch 28

#define channelAPin 34
#define channelBPin 33

volatile int counter = 0;

//encoder variables
volatile bool channelAVal;
volatile bool channelBVal;
volatile int encoderTicks;

//switch variables
volatile int but_interrupt_flag = 0;
const int debounce_time = 50;  //ms
int last_switch_time = 0;
const int debounce_timeout = 1000; // ms

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PWM, OUTPUT);
  digitalWrite(PWM, LOW);

  pinMode(Switch, INPUT);
  attachInterrupt(digitalPinToInterrupt(Switch), limit_switch, RISING);

  pinMode(channelAPin, INPUT);
  pinMode(channelBPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(channelAPin), encoderCount, CHANGE);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (but_interrupt_flag){
    debounce(Switch);
    Serial.println(counter);
    but_interrupt_flag = 0;
  }
}

//debouncing
void debounce(int button){
  int cur_time = millis();
  if ( (cur_time - last_switch_time) > debounce_timeout){
    last_switch_time = cur_time;
  }
}

//Initialize the system
void limit_switch() {
  if (but_interrupt_flag == 0) {
    but_interrupt_flag = 1;
    counter++;
  }
}

//Encoder Counts
void encoderCount(){
  channelAVal = digitalRead(channelAPin);
  channelBVal = digitalRead(channelBPin);
  if (channelAVal == channelBVal) {
    encoderTicks++;
  }
  else {
    encoderTicks--;
  }
  
  Serial.print("B  -> "); Serial.println(channelBVal);
  Serial.print("A  -> ");Serial.println(channelAVal);
  Serial.println(encoderTicks);
}
