
void rotaryEncoderSetup() {
  setup_rotary_encoder();
  calibrate_rotary_encoder();
}

void rotary_encoder(){
  wheelEncoderDist= int(rotary_data());
;
}

