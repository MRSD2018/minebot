#include "dcMotor.h"
#include <digitalWri/teFast.h> //thank you jrraines!
//https://code.google.com/archive/p/digitalwritefast/downloads

#define pushButton 2
#define channelA 3


dcMotor dc(3,4,5); //PWM, L1, L2

extern volatile bool but_interrupt_flag = 0;
extern const int debounce_time = 20; //ms
extern volatile int channelAVal;
extern volatile int channelBVal;
extern volatile long encoderTicks = 0;


void setup() {
  Serial.begin(9600);
  pinMode(channelA, INPUT);
  pinMode(channelB, INPUT);


  attachInterrupt(digitalPinToInterrupt(pushButton),button_press,RISING);
  attachInterrupt(digitalPinToInterrupt(channelA,handleChannelA,CHANGE);
}

void loop() {
  if (but_interrupt_flag)
  {
    debounce(pushButton);
    //do bunch of stuff here
    but_interrupt_flag = 0; //this will make sure we don't go into ISR again when doing stuff
  }

}
