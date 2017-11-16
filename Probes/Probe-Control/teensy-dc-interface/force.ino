// LOAD CELL FUNCTIONS

void setupLoadCell() {
  // Determined with Matlab Script
  loadCell.set_scale(67590.7657); // 1/gain
  loadCell.set_offset(-0.8384);
}

float loadValue = 0.0f;
float dloadValuedt = 0.0f;
long int lastTime = 0;


void readForce() { // ISR

  newValue = true; // For logging

  long int time = millis();
  double dt = (time - lastTime);

  if (dt > 10) { // reject false readings from DATA pin

    float newValue = loadCell.get_value();
    dloadValuedt = (newValue - loadValue) / (dt) * 1000;
    loadValue = newValue;

    lastTime = time;
  }
}

float getRawForce() {
  return loadValue; // kg
}

float getForce() {
  return getRawForce() - tare;
}

float getForceDerivative() {
  return dloadValuedt; // kg/s
}

float getNormalizedForceDerivative() {
  return dloadValuedt/speedAdjustment; // kg/s
}
