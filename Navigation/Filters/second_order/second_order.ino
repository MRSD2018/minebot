float dT_PD = 0.002;
float x = 0.0;  //distance
float xf = 0.0; //filtered distance
float xd = 0.0; //desired distance
float dx = 0.0; //rate of change of distance
float sum_e = 0.0; //past errors
float low_pass_freq = 150.0;  //dependent on hardware
float diff_freq = 150.0;  //dependent on hardware
float x_old[2]={0.0,0.0};
float xf_old[2]={0.0,0.0};
float dx_old[2]={0.0,0.0};
float low_pass_itr = 1.0;
float diff_itr = 1.0;

void low_pass_filter()
{
  float w = 2.0 * 3.1415926536 * low_pass_freq;
  float a = pow(((w * dT_PD)/(2.0 +(w * dT_PD))),2.0);
  float b = 2.0 * a;
  float c = a;
  float d = 2.0 * (2.0 - (w * dT_PD)) / (2.0 + (w * dT_PD));
  float e = pow((- 0.5 * d),2.0);

  xf = (a * x) + (b * x_old[0]) + (c * x_old[1]) + (d * xf_old[0]) + (e * xf_old[1]);

  if (low_pass_itr < 3.5) //because won't have old values
  {
    xf = x;
    low_pass_itr+=1;
  }

  xf_old[1] = xf_old[0];
  xf_old[0] = xf;
  
  x_old[1] = x_old[0];
  x_old[0] = x;
}

void differentiator()
{
  float w = 2.0 * 3.1415926536 * diff_freq;
  float a = 2.0 * dT_PD * pow((w/(2.0 + (dT_PD * w))),2.0);
  float b = - a;
  float c = 2.0 * (2.0 - (dT_PD * w)) / (2.0 + (dT_PD * w));
  float d = pow((- 0.5 * c), 2.0);

  dx = (a * x) + (b * x_old[0]) + (c * dx_old[0]) + (d * dx_old[1]);

  if(diff_itr < 3.5)
  {
    dx = (x - x_old[0])/dT_PD;
    diff_itr+=1;
  }

  dx_old[1] = dx_old[0];
  dx_old[0] = dx;

  x_old[1] = x_old[0];
  x_old[0] = x;
}

