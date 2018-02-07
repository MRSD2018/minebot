<<<<<<< HEAD
extern int clock_pin; // output to clock
extern int CSn_pin; // output to chip select
extern int input_pin; // read AS5045
extern int input_stream; // one bit read from pin
extern long packed_data; // two bytes concatenated from inputstream
extern long angle; // holds processed angle value
extern float angle_Float;
extern float true_angle;
extern float calib_angle;
extern float offset_angle; //17.02
extern float resting_angle;
extern float rotary_angle;
extern float starting_angle;
extern long printing_angle;
extern long angle_mask; // 0x111111111111000000: mask to obtain first 12 digits with position info
extern long status_mask; // 0x000000000000111111; mask to obtain last 6 digits containing status info
extern long status_bits; // holds status/error information
extern int DECn; // bit holding decreasing magnet field error data
extern int INCn; // bit holding increasing magnet field error data
extern int OCF; // bit holding startup-valid bit
extern int COF; // bit holding cordic DSP processing error data
extern int LIN; // bit holding magnet field displacement error data
extern int debug; // SET THIS TO 0 TO DISABLE PRINTING OF ERROR CODES

int parse = 0;

void setup() {
  Serial.begin(9600);
  setup_rotary_encoder();
  calibrate_rotary_encoder();
}

void loop(){
  delay(200);
//  if(Serial.available()>0)
//  {  
//    parse = Serial.parseInt();
//    if(parse == 0){calibrate_rotary_encoder();}
//    if(parse == 1){rotary_data();}
//  }
  float answer = rotary_data();
  Serial.println(answer);
}

=======
extern int clock_pin; // output to clock
extern int CSn_pin; // output to chip select
extern int input_pin; // read AS5045
extern int input_stream; // one bit read from pin
extern long packed_data; // two bytes concatenated from inputstream
extern long angle; // holds processed angle value
extern float angle_Float;
extern float true_angle;
extern float calib_angle;
extern float offset_angle; //17.02
extern float resting_angle;
extern float rotary_angle;
extern float starting_angle;
extern long printing_angle;
extern long angle_mask; // 0x111111111111000000: mask to obtain first 12 digits with position info
extern long status_mask; // 0x000000000000111111; mask to obtain last 6 digits containing status info
extern long status_bits; // holds status/error information
extern int DECn; // bit holding decreasing magnet field error data
extern int INCn; // bit holding increasing magnet field error data
extern int OCF; // bit holding startup-valid bit
extern int COF; // bit holding cordic DSP processing error data
extern int LIN; // bit holding magnet field displacement error data
extern int debug; // SET THIS TO 0 TO DISABLE PRINTING OF ERROR CODES

int parse = 0;

void setup() {
  Serial.begin(115200);
  setup_rotary_encoder();
  calibrate_rotary_encoder();
}

void loop(){
  delay(200);
//  if(Serial.available()>0)
//  {  
//    parse = Serial.parseInt();
//    if(parse == 0){calibrate_rotary_encoder();}
//    if(parse == 1){rotary_data();}
//  }
  float answer = rotary_data();
  Serial.println(answer);
}

>>>>>>> 2702f454d51990978334a05f56b994e10fce6f8c
