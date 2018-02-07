#define button 2

const int debounce_time = 20;  //ms
volatile bool button_interrupt_flag = 0;

void setup() {
  Serial.begin(9600);
  pinMode(but_0, INPUT);
  attachInterrupt(digitalPinToInterrupt(but_0), button_0_press, RISING);
}

void loop() {
  // put your main code here, to run repeatedly://button 0 FSM state switching
  if (button_interrupt_flag)
  {
    debounce(but_0);
    
    //enter code here, what happens after button is pressed everytime?
    serial.println("button pressed and debounced.");
    
    button_interrupt_flag = 0;
  }
}

void debounce(int button)
{
  unsigned long now = millis ();
  while (digitalRead(button) == HIGH || (millis() - now) <= debounce_time)
  {
    if(digitalRead(button) == HIGH){now = millis();}
  }
}
