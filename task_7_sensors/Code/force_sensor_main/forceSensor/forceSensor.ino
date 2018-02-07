#include "ForceSensor.h"

ForceSensor force(A0);

void setup() {
  Serial.begin(9600);
  force.initDataArray();
}

void loop() {
    Serial.println(force.getForce());
}
