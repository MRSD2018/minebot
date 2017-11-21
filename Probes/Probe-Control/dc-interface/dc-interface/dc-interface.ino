
//#include <Encoder.h>

// MIN PWM: 20
// MAX PWM: 200 - 3.5A spike, might need acceleration

//#define motorPWM 35
//#define motorDIR 34
//Encoder enc(19, 18);

#define UPPER_SWITCH_PIN 33
#define LOWER_SWITCH_PIN 31

void setup() {
  // put your setup code here, to run once:
//  pinMode(motorPWM, OUTPUT);
//  pinMode(motorDIR, OUTPUT);
  pinMode(UPPER_SWITCH_PIN, INPUT);
  pinMode(LOWER_SWITCH_PIN, INPUT);
}

int pwm = 20;

void loop() {
  // put your main code here, to run repeatedly:
//  if (digitalRead(UPPER_SWITCH_PIN) == 0)
//    digitalWrite(motorDIR, HIGH);
//  else 
//    digitalWrite(motorDIR, LOW);  
//
//  if (digitalRead(LOWER_SWITCH_PIN) == 0)  
//    analogWrite(motorPWM, pwm);
//  else
//    analogWrite(motorPWM, 0);
  
//  Serial.print(enc.read());
//  Serial.print(" ");
  Serial.print(digitalRead(LOWER_SWITCH_PIN));
  Serial.print(" ");
  Serial.print(digitalRead(UPPER_SWITCH_PIN));
  Serial.println(" ");
  
//  if (Serial.available() > 0)
//  {
//    pwm = Serial.readString().toInt();
//    Serial.print("PWM: ");
//    Serial.println(pwm);
//  }
}
