#include "digitalWriteFast.h"
//#include "dcMotor.h"
//#include "ForceSensor.h"

#define ANALOG 10
#define channelAPin 2
#define channelBPin 4
#define button 3

//dcMotor motor(PWM, L1, L2, channelAPin, channelBPin);

volatile bool channelAVal;
volatile bool channelBVal;
volatile int encoderTicks;
int test;


volatile int button_interrupt_flag = 0;
const int debounce_time = 20;  //ms
int last_switch_time = 0;
const int debounce_timeout = 1000; // ms

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(channelAPin), encoderCount, CHANGE);
  attachInterrupt(digitalPinToInterrupt(button), limitSwitch, RISING); //change State when B0 input is rising
  pinMode(button,INPUT_PULLUP);
  pinMode(ANALOG, OUTPUT);
}

void loop() {
  // uncomment one or the other to get position or velocity control
  if (encoderTicks > 1800){
    analogWrite(ANALOG, 0);
    Serial.println("Forward");
    
  }
  if (encoderTicks < 85){
    analogWrite(ANALOG, 1023);
    Serial.println("Backwards");
  }
  
  Serial.println(encoderTicks);
  delay(100);
  
}

//Change motor direction when limit switch is hit
void limitSwitch(){
  //Debounce code
  int cur_time = millis();
  if ( (cur_time - last_switch_time) > debounce_timeout)
  {
    last_switch_time = cur_time;
    if (analogRead(ANALOG) > 512) {
      Serial.println("Entered DEBOUCE");
      analogWrite(ANALOG, 0);
    }
    else {
      Serial.println("Entered DEBOUCE");
      analogWrite(ANALOG, 1023);
    }
  }
}


void encoderCount()
{
  channelAVal = digitalReadFast(channelAPin);
  channelBVal = digitalReadFast(channelBPin);
  if (channelAVal == channelBVal) {
    encoderTicks++;
  }
  else {
    encoderTicks--;
  }
}
