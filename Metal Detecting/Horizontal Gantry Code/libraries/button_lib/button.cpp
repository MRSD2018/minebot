#include "button.h"

button::button(int buttonPin)
{
	pinMode(buttonPin, INPUT);
}

int button::getCounter()
{
	return counter;
}
(
void button::setCounter(int count)
{
	counter = count;
}

void button::debounce()
{
  int cur_time = millis();
  if ( (cur_time - last_switch_time) > debounce_timeout){
    last_switch_time = cur_time;
  }
}

void button::checkButtonForState(){
  if (but_interrupt_flag){
    stateButton.debounce();
    counter++;
    but_interrupt_flag = 0; //this will make sure we don't go into ISR again when doing stuff
  }
}
