// Code for interfacing with Load Cell
// Author: David Robinson

#include "HX711.h"

#define DOUT  30
#define CLK   29

HX711 loadCell(DOUT, CLK);

void setup() {
  
  // Determine with Matlab Script
  loadCell.set_scale(67590.7657); // 1/gain
  loadCell.set_offset(-0.8384);

  Serial.begin(9600);
}

void loop() {
  Serial.print("VAL: ");
  Serial.print(loadCell.get_value()); // kg
  Serial.println();
}
