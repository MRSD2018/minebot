// Code for interfacing with Load Cell

#include "HX711.h"

#define DOUT  3
#define CLK  2

HX711 loadCell(DOUT, CLK);

void setup() {
  Serial.begin(9600);
  
  // Determine with Matlab Script
  loadCell.set_scale(67590.7657); // 1/gain
  loadCell.set_offset(-0.8384);
}

void loop() {
  Serial.print(loadCell.get_value()); // kg
  Serial.println();
}
