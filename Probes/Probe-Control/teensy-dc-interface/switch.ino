
static int samples = 5;
int upperPin = 0, lowerPin = 0;

void setupSwitches() {
  pinMode(UPPER_SWITCH_PIN, INPUT);
  pinMode(LOWER_SWITCH_PIN, INPUT);
}

void readSwitches() {
  int val = digitalRead(UPPER_SWITCH_PIN);
  if (val == 1)   upperPin += 1;
  else            upperPin = 0;

  val = digitalRead(LOWER_SWITCH_PIN);
  if (val == 1)   lowerPin += 1;
  else            lowerPin = 0;
}

bool lowerSwitch() {
  if (lowerPin >= samples) return true;
  else return false;
}

bool upperSwitch() {
  if (upperPin >= samples) return true;
  else return false;
}

