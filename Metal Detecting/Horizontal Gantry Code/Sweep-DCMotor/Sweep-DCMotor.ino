#include "digitalWriteFast.h"
//#include "dcMotor.h"
//#include "ForceSensor.h"

#define ANALOG A0
#define L1 4
#define L2 5
#define channelAPin 2
#define channelBPin 6

//dcMotor motor(PWM, L1, L2, channelAPin, channelBPin);

volatile bool channelAVal;
volatile bool channelBVal;
volatile int encoderTicks;
volatile int counter =0;

volatile int button_interrupt_flag = 0;
const int debounce_time = 20;  //ms
int last_switch_time = 0;
const int debounce_timeout = 1000; // ms

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(channelAPin), encoderCount, CHANGE);
  attachInterrupt(1, limitSwitch, RISING); //change State when B0 input is rising
  digitalWrite(L1, LOW);
  digitalWrite(L2, HIGH);
}

void loop() {
  // uncomment one or the other to get position or velocity control
  analogWrite(ANALOG, 512);
  
}

//Change motor direction when limit switch is hit
void limitSwitch(){
  //Debounce code
  int cur_time = millis();
  if ( (cur_time - last_switch_time) > debounce_timeout)
  {
    last_switch_time = cur_time;
    counter++;
    Serial.print(counter);
    digitalWrite(L1, !digitalRead(L1));
    digitalWrite(L2, !digitalRead(L2)); //change motor direction
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
