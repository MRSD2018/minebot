int clock_pin = 27; // output to clock
int CSn_pin = 29; // output to chip select
int input_pin = 28; // read AS5045
int input_stream = 0; // one bit read from pin
long packed_data = 0; // two bytes concatenated from inputstream
long angle = 0; // holds processed angle value
float angle_float = 0.0;
float true_angle = 0.0;
float calib_angle = 0.0;
float offset_angle = 0.0; //18 22
float resting_angle = 0.0;
float rotary_angle = 0.0;
float starting_angle = 0.0;
long printing_angle = 0;
long angle_mask = 262080; // 0x111111111111000000: mask to obtain first 12 digits with position info
int debug;

const float diam = 100.5;
float true_rad;
int cnt = 0;
float prev_rad;
float true_dist;

long angle_temp = 0;

void setup_rotary_encoder()
{
  pinMode(clock_pin, OUTPUT); // SCK
  pinMode(CSn_pin, OUTPUT); // CSn -- has to toggle high and low to signal chip to start data transfer
  pinMode(input_pin, INPUT); // SDA
  debug = 0;
}

void calibrate_rotary_encoder()
{
  resting_angle = 0;
  resting_angle = rotary_data();
  Serial.print("Angle calibrated, new 0 at ");
  Serial.println(resting_angle);
}

float rotary_data()
{
  digitalWrite(CSn_pin, HIGH); // CSn high
  digitalWrite(clock_pin, HIGH); // CLK high
  delayMicroseconds(5); // wait for 1 second for no particular reason
  digitalWrite(CSn_pin, LOW); // CSn low: start of transfer
  delayMicroseconds(5); // delay for chip -- 1000x as long as it needs to be
  digitalWrite(clock_pin, LOW); // CLK goes low: start clocking
  delayMicroseconds(5); // hold low for 10 ms
  for (int x=0; x < 18; x++) // clock signal, 18 transitions, output to clock pin
  { 
    digitalWrite(clock_pin, HIGH); // clock goes high
    delayMicroseconds(5); // wait 5 microsec
    input_stream = digitalRead(input_pin); // read one bit of data from pin
    //Serial.print(inputstream, DEC); // useful if you want to see the actual bits
    packed_data = ((packed_data << 1) + input_stream); // left-shift summing variable, add pin value
    digitalWrite(clock_pin, LOW);
    delayMicroseconds(5); // end of one clock cycle
  } // end of entire clock cycle

  angle = packed_data & angle_mask; // mask rightmost 6 digits of packeddata to zero, into angle.
  angle = (angle >> 6); // shift 18-digit angle right 6 digits to form 12-digit value
  angle_float = angle * 0.08789; // angle * (360/4096) == actual degrees

  //arduino's C++ can't handle remainder of floats, here's a workaround
  angle_temp = round((angle_float - resting_angle + 360.0) * 10000);
  angle_temp = angle_temp % (360*10000);
  true_angle = angle_temp / 10000.0;

  //Convert angle to rad.
  true_rad = 3.1415*true_angle / 180;

  //Count rotations 
  if (true_rad - prev_rad > 4) {
    cnt = cnt-1;
  }
  if (true_rad - prev_rad < -4) {
    cnt = cnt+1;
  }

  Serial.println(cnt);

  true_dist = diam*(cnt*3.1415 + true_rad/(2));
   
  prev_rad = true_rad;
  
  return true_dist;
}
