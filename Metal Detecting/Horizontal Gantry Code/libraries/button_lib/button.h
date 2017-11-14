#ifndef button_h
#define button_h

#include "Arduino.h"

class button {
  public:
    button(int buttonPin);
    int getCounter();
    void setCounter(int count);
    void debounce(int buttonPin);
    void checkButtonForState();

  private:
    //switch variables
    volatile int but_interrupt_flag = 0;
    const int debounce_time = 20;  //ms
    int last_switch_time = 0;
    const int debounce_timeout = 1000; // ms
    int counter;
};

#endif
