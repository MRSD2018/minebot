
void serialControl() {

  if (Serial.available() > 0) {
    String input = Serial.readString();

    switch (input.charAt(0))
    {
      case 'p':
        setState(PROBE);
        break;
      case 's':
        setState(IDLE);
        break;
      case 'z':
        setState(ZERO);
        break;
      case 'c':
        setState(CALIB);
        break;
      case 'd':
        debug = !debug;
        break;
      default:
        if (debug) errorMessage(input);
        break;
    }
  }
}

void errorMessage(String input) {
  Serial.println("\"" + input + "\" is an invalid input.");
}
