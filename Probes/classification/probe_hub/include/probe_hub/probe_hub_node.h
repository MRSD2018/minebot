#pragma once

#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"

class probe_hub
{
public:
  void foo();
  int bar;
private:
	void probeDataClbk(const std_msgs::Float32& msg);
	void probeCmdLogic(double currentTime);
	std::vector<float> motorPosVector;
	float timeBetweenProbes = 5;
	double lastTime;
};

std_msgs::Bool probe_cmd;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "probe_hub");
  ros::NodeHandle n;
  ros::Publisher begin_probe_pub = n.advertise<std_msgs::Bool>("begin_probe", 1000);
  ros::Subscriber probe_data_sub = n.subscribe("probe_data", 1000, probeDataClbk);
  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    double currentTime = ros::Time::now().toSec();
    probeCmdLogic(currentTime);
    begin_probe_pub.publish(probe_cmd);
    probe_cmd.data = false;

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}