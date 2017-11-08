#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"

std::vector<float> motorPosVector;
float timeBetweenProbes = 2;
std_msgs::Bool probe_cmd;
double lastTime;

void probeDataClbk(const std_msgs::Float32& msg)
{
      ROS_INFO("here");
      // ROS_INFO("%f",msg.data);

  // float motorPos = msg.data;
  // motorPosVector.push_back(motorPos);
  // for (auto it = motorPosVector.begin(); it!=motorPosVector.end(); it++)
  // {
  //   ROS_INFO("%f",*it);
  //   // std::cout<<*it<<std::endl;
  // }
  // std::endl;
}

void probeCmdLogic(double currentTime)
{
    if((currentTime-lastTime)>timeBetweenProbes)
    {
      lastTime = ros::Time::now().toSec();
      probe_cmd.data = false;
    }
}

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