volatile bool but_interrupt_flag = 0;
const int debounce_time = 20; //ms

//setup
void setupButton()
{
  pinMode(pushButton, INPUT);
  attachInterrupt(digitalPinToInterrupt(pushButton),buttonPress,RISING);
}

void checkButtonForState()
{
  if (but_interrupt_flag)
  {
    debounce(pushButton);
    STATE++;
    if(STATE == 2) startStepper();
    else stopStepper();
    if (STATE>3){STATE = 0;}
    but_interrupt_flag = 0; //this will make sure we don't go into ISR again when doing stuff
  }
}

//ISR
void buttonPress(){if (but_interrupt_flag == 0){but_interrupt_flag = 1;}}

//debouncing
void debounce(int button)
{
  unsigned long now = millis ();
  while (digitalReadFast(button) == HIGH || (millis() - now) <= debounce_time)
  {
    if(digitalReadFast(button) == HIGH){now = millis();}
  }
}
