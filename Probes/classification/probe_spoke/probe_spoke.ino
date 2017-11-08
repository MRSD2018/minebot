#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>

ros::NodeHandle nh;

std_msgs::Float32 probe_data;
void probeCmdClbk(const std_msgs::Bool& msg);
ros::Publisher probe_data_pub("probe_data", &probe_data);
ros::Subscriber<std_msgs::Bool> probe_cmd_sub("begin_probe", probeCmdClbk);

void probeCmdClbk(const std_msgs::Bool& msg)
{
  //  Serial.println("here1");
  nh.loginfo("Program info");
  probe_data.data = float(analogRead(A1));
  probe_data_pub.publish(&probe_data);
}

void setup()
{
  pinMode(A1, INPUT);
  nh.initNode();
  nh.advertise(probe_data_pub);
  nh.subscribe(probe_cmd_sub);
}

void loop()
{
  nh.spinOnce();
}

// rosrun rosserial_python serial_node.py /dev/ttyACM0
