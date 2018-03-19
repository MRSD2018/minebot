//Timing variables
int debounce_time = 100;

void switch_setup () {

}

void debounce(int button) {
  Serial.print("Debouncing "); Serial.println(button);
  if (STATE != 2 && button != state_but) {
    STATE = 0;
    Serial.println("EMERGENCY STOP - LIMIT HIT");
  }
  if (!but_interrupt_flag) {
    unsigned long now = millis ();
    while (digitalReadFast(button) == HIGH || (millis() - now) <= debounce_time)
    {
      if(digitalReadFast(button) == HIGH){now = millis();}
    }
    but_interrupt_flag = 1;
  }
}

