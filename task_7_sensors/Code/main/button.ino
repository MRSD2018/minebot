//ADD THESE AS DECLARATIONS
/*
#define pushButton 2
extern volatile bool but_interrupt_flag = 0;
extern const int debounce_time = 20; //ms
*/

//ADD THIS INTO setup()
/*
attachInterrupt(digitalPinToInterrupt(pushButton),button_press,RISING);
*/

//ADD THESE INTO loop()
/*
if (but_interrupt_flag)
{
  debounce(pushButton);
  //do bunch of stuff here
  but_interrupt_flag = 0; //this will make sure we don't go into ISR again when doing stuff
}
*/

/*---------------------------------------------------------------------------------------------*/

//ISR
void button_press(){if (but_interrupt_flag == 0){but_interrupt_flag = 1;}}

void debounce(int button)
{
  unsigned long now = millis ();
  while (digitalReadFast(button) == HIGH || (millis() - now) <= debounce_time)
  {
    if(digitalReadFast(button) == HIGH){now = millis();}
  }
}
