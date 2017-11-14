//#include "button.h"

//Pins
#define Speed 30
#define channelAPin 34
#define channelBPin 33
#define stateButton 28

//LEDs
#define LED1 5
#define LED2 6
#define LED3 7
#define LED4 8
#define Enable 11

//items

//button
extern volatile bool but_interrupt_flag;
extern const int debounce_time; //ms

int STATE=0;

void setup() {
  Serial.begin(9600);

  //items
  setupStateButton();

}

void loop() {
  Serial.println(STATE);

}
