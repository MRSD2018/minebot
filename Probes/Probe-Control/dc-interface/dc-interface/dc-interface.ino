
// MIN PWM: 20
// MAX PWM: 200 - 3.5A spike, might need acceleration

#define motorPWM 10
#define motorDIR 9

void setup() {
  // put your setup code here, to run once:
  pinMode(motorPWM, OUTPUT);
  pinMode(motorDIR, OUTPUT);
}

int pwm = 100;

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(motorDIR, LOW);
    analogWrite(motorPWM, pwm);
    delay(2000);
    analogWrite(motorPWM, 0);
    delay(1000);
    digitalWrite(motorDIR, HIGH);
    analogWrite(motorPWM, pwm);
    delay(2000);
    analogWrite(motorPWM, 0);
    delay(1000);

    if (Serial.available() > 0)
    {
      pwm = Serial.readString().toInt();
      Serial.print("PWM: ");
      Serial.println(pwm);
    }
      

}
