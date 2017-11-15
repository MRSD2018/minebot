// SERIAL INPUT FUINCTIONS
void serialControl() {

  if (Serial.available() > 0) {
    String input = Serial.readString();

    switch (input.charAt(0))
    {
      case 'p':
        setState(PROBE);
        if (debug) Serial.println("Probe Command Received");
        break;
      case 's':
        setState(IDLE);
        if (debug) Serial.println("Stop Command Recieved");
        break;
      case 'z':
        setState(ZERO);
        if (debug) Serial.println("Zero Command Recieved");
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
