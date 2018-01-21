#define UPPER_SWITCH_PIN 33
#define LOWER_SWITCH_PIN 18 //31

void setup() {
  pinMode(UPPER_SWITCH_PIN, INPUT);
  pinMode(LOWER_SWITCH_PIN, INPUT);
  pinMode(13, OUTPUT);
  Serial.begin(38400);
}

void loop() {

  Serial.println("TEST");

  if (digitalRead(LOWER_SWITCH_PIN) == 1)
    digitalWrite(13, HIGH);  
  else
    digitalWrite(13, LOW);

//  Serial.print(digitalRead(LOWER_SWITCH_PIN));
//  Serial.print(" ");
//  Serial.print(digitalRead(UPPER_SWITCH_PIN));
//  Serial.println(" ");
 
}
