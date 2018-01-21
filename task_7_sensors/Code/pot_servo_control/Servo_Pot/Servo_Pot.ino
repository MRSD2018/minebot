#include <Servo.h>

//In
#define Pot A0

//Out
#define PWM 9

//Vairables
int potIn;
int Angle;
Servo myservo;

void setup() {
  // put your setup code here, to run once:
  pinMode(Pot, INPUT);
  pinMode(3, OUTPUT);
  myservo.attach(PWM);
  Serial.begin(9600);
}

void loop() {
  // Read the potentiometer input, map to desired servo angle, and set the servo to that angle:
  potIn = analogRead(Pot);
  Angle = map(potIn, 0, 1023, 0, 180);
  Serial.println(Angle);
  myservo.write(Angle);
  delay(10);
}
