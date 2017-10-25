// Code for interfacing with Load Cell

#include "HX711.h"

#define DOUT  3
#define CLK  2

HX711 loadCell(DOUT, CLK);

void setup() {
  Serial.begin(9600);
  loadCell.set_scale(1); // may need to set calibration adaptively ~ 7000
  loadCell.set_offset(0);
  loadCell.tare(); //Reset the scale to 0
}

void loop() {
  Serial.print("X_");
  Serial.print(loadCell.get_units(1));
  Serial.println();
}
