// LOAD CELL FUNCTIONS
void setupLoadCell() {
  // Determined with Matlab Script
  loadCell.set_scale(67590.7657); // 1/gain
  loadCell.set_offset(-0.8384);
}

float loadValue = 0.0f;
float getRawForce() {
  if (loadCell.is_ready()) {
    loadValue = loadCell.get_value();
  }

  return loadValue; // kgs
}

float getForce() {
  return getRawForce() - tare;
}
