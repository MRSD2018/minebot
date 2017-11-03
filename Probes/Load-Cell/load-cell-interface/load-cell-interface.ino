// Code for interfacing with Load Cell
// Author: David Robinson

#include "HX711.h"

#define DOUT  3
#define CLK  2

#define UPPER_SWITCH_PIN 5
#define LOWER_SWITCH_PIN 4

#define STEP_PUL 9
#define STEP_DIR 8

HX711 loadCell(DOUT, CLK);

void setup() {

  pinMode(STEP_PUL, OUTPUT);
  pinMode(STEP_DIR, OUTPUT);
  
  digitalWrite(STEP_PUL, LOW);
  digitalWrite(STEP_DIR, LOW);
 
  pinMode(UPPER_SWITCH_PIN,INPUT);
  pinMode(LOWER_SWITCH_PIN,INPUT);
  
  Serial.begin(9600);
  
  // Determine with Matlab Script
  loadCell.set_scale(67590.7657); // 1/gain
  loadCell.set_offset(-0.8384);
}

void loop() {

  digitalWrite(STEP_PUL, HIGH);
  delayMicroseconds(100);
  digitalWrite(STEP_PUL, LOW);
  delayMicroseconds(100);

//  Serial.print(digitalRead(UPPER_SWITCH_PIN));
//  Serial.print("\t");
//  Serial.print(digitalRead(LOWER_SWITCH_PIN));

//  Serial.print(loadCell.get_value()); // kg
//  Serial.println();
  
}
