#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"
#define PI 3.14159265
// #include <probe_hub/probe_hub_node.h>

std::vector<float> motorPosVector;
float timeBetweenProbes = 5;
std_msgs::Bool probe_cmd;
double lastTime;

struct point3D
{
  float x, y, z;
};

struct point2D
{
  float x, y
};

struct circle
{
  point2D center;
  float rad;
};

std::vector<point3D> contactPointsVector;

void probeDataClbk(const std_msgs::Float32& msg)
{
  float motorPos = msg.data;
  motorPosVector.push_back(motorPos);
  // for (auto it = motorPosVector.begin(); it!=motorPosVector.end(); it++)
  // {
  //   // ROS_INFO("%f",*it);
  //   // std::cout<<*it<<std::endl;
  // }
  // std::endl;
  updateContactPoints();
}

void updateContactPoints()
{
  contactPointsVector.push_back(contactPointToGantryFrame(motorPos));
}

void classifyShape()
{
  if (contactPointsVector.size()>=3) // below 3 points, not enough data to estimate circumcenter
  {
    // TODO: need to project points to a common horizontal plane
    circle circle = initCircle(std::vector<point2D>& points);
  }
}

point3D contactPointToGantryFrame(float& motorPos)
{
  point3D cp;
  cp.x = getGantryXPos();
  cp.y = motorPos*cos(30*PI/180.0)+0.3; // define these params above
  cp.z = -motorPos*sin(30*PI/180.0)-0.4;
  return cp;
}

float getGantryXPos() // side to side position of gantry head
{
  return 1; // TODO
}

circle initCircle(const std::vector<point2D>& points)
{
  circle circle;
  point2D cc;
  float sigX = 0;
  float sigY = 0;
  int q = 0;

  int n = points.size();

  for (int i = 0;i<n-2;i++){
    for (int j = i+1;j<n-1;j++){
      for (int k = j+1;k<n;k++){
        // create a vector of three points
        std::vector<point2D> threePoints;
        threePoints.push_back(points[i]);
        threePoints.push_back(points[j]);
        threePoints.push_back(points[k]);
        cc = circumcenter(threePoints);
        sigX += cc.x;
        sigY += cc.y;
        q++;
      }
    }
  }
  // if (q==0)
  //   disp('All points aligned')
  // end
  cc.x = sigX/q;
  cc.y = sigY/q;
  circle.center = cc;
  circle.rad = 1; // insert calculation for this later
  return circle;
}


point2D circumcenter(const std::vector<point2D>& points)
{

  float pIx = points[0].x;
  float pIy = points[0].y;
  float pJx = points[1].x;
  float pJy = points[1].y;
  float pKx = points[2].x;
  float pKy = points[2].y;

  float dIJ.x = pJx - pIx;
  float dIJ.y = pJy - pIy;

  float dJK.x = pKx - pJx;
  float dJK.y = pKy - pJy;

  float dKI.x = pIx - pKx;
  float dKI.y = pIy - pKy;

  float sqI = pIx * pIx + pIy * pIy;
  float sqJ = pJx * pJx + pJy * pJy;
  float sqK = pKx * pKx + pKy * pKy;

  float det = dJK.x * dIJ.y - dIJ.x * dJK.y;

    // if (abs(det) < 1.0e-10)
    //   cc=0;
    // 

  point2D cc;

  cc.x=(sqI * dJK.y + sqJ * dKI.y + sqK * dIJ.y) / (2 * det);
  cc.y = -(sqI * dJK.x + sqJ * dKI.x + sqK * dIJ.x) / (2 * det);

  return cc;
}


void probeCmdLogic(double currentTime)
{
  if((currentTime-lastTime)>timeBetweenProbes)
  {
    lastTime = ros::Time::now().toSec();
    probe_cmd.data = true;
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