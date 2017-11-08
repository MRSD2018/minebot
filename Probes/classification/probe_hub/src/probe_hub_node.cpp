#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"

#include <fstream>

#define PI 3.14159265
// #include <probe_hub/probe_hub_node.h>

std::vector<float> motorPosVector;
// float timeBetweenProbes = 5;
std_msgs::Bool probe_cmd;
double lastTime;
float probe_angle = 0.0; // eventually increase to 30deg 
float rad_threshold = 0.1;

// sample data
std::vector<float> motorPosVector_sim;
std::vector<double> sampleTime_sim;
std::vector<float> gantryXPos_sim;

motorPosVector_sim = [.140 .141 .145 .152];
sampleTime_sim = [0.0 5.0 10.0 15.0];
gantryXPos_sim = [0 .012 .024 .036];
int sampleTime_step = 0;

struct point3D { float x, y, z; };

struct point2D { float x, y };

struct circle {
  point2D center;
  float rad;
};

circle mine;

std::vector<point3D> contactPointsVector;

void simulatePointCapture() // send back 
{

}

void probeDataClbk(const std_msgs::Bool& msg)
{
  // float motorPos = msg.data;
  // motorPosVector.push_back(motorPos);
  // for (auto it = motorPosVector.begin(); it!=motorPosVector.end(); it++)
  // {
  //   // ROS_INFO("%f",*it);
  //   // std::cout<<*it<<std::endl;
  // }
  // std::endl;
  updateContactPoints();
  sampleTime_step++;
}

void updateContactPoints() // adds to the vector of contact points, in gantry frame
{
  contactPointsVector.push_back(contactPointToGantryFrame(motorPos));
}

bool isMine() // determines whether collection of points represents a circle (i.e. mine)
{
  if (contactPointsVector.size()>=3) // if fewer 3 points, there is not enough data to estimate circumcenter
  {
    std::vector<point2D> projectedPoints;
    projectedPoints.x = contactPointsVector.x;
    projectedPoints.y = contactPointsVector.y; // simplification for planar case -> update!!
    // TODO: need to project points to a common horizontal plane - probing angle = 0 for now so that this is not a problem
    circle circle = calcCircle(std::vector<point2D>& projectedPoints); // 
    if (circle.rad<rad_threshold) 
    {
      mine = circle; // assign the calculated circle to the mine instance;
      return true;
    }
    else 
    {
      return false;
    }

  }
  else
    return false;
}


point3D contactPointToGantryFrame(float& motorPos)
{
  point3D cp; // cp = contact point
  cp.x = getGantryXPos(); // currently hard coded
  cp.y = motorPos*cos(probe_angle*PI/180.0); // define these params above
  cp.z = 0;//-motorPos*sin(probe_angle*PI/180.0)-0.4;
  return cp;
}

float getGantryXPos() // side to side position of gantry head
{
  return gantryXPos_sim[sampleTime_step];
}

circle calcCircle(const std::vector<point2D>& points)
{
  circle circle;
  point2D cc;
  float sigX = 0;
  float sigY = 0;
  int q = 0;

  int n = points.size();

  for (int i = 0;i<n-2;i++){ // go through all the combinations of points
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
  circle.rad = calcRadius(cc, points);
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

  cc.x = (sqI * dJK.y + sqJ * dKI.y + sqK * dIJ.y) / (2 * det);
  cc.y = -(sqI * dJK.x + sqJ * dKI.x + sqK * dIJ.x) / (2 * det);

  return cc;
}

float calcRadius(point2D& cc, std::vector<point2D>& points)
{
  float rHat = 0;
  float dx, dy;
  int numPoints = points.size();
  for (int i = 0; i<numPoints; i++){
    dx = points[i].x - cc.x;
    dy = points[i].y - cc.y;
    rHat += sqrt(dx^2 + dy^2);
  }
  return rHat / numPoints;
}

void probeCmdLogic(double currentTime)
{
  if(currentTime>sampleTime_sim[sampleTime_step])
  {
    // lastTime = ros::Time::now().toSec();
    probe_cmd.data = true;
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "probe_hub");
  ros::NodeHandle n;
  ros::Publisher begin_probe_pub = n.advertise<std_msgs::Bool>("begin_probe", 1000);
  // ros::Subscriber probe_data_sub = n.subscribe("probe_data", 1000, probeDataClbk);
  ros::Subscriber probe_data_returned_sub = n.subscribe("begin_probe", 1000, probeDataClbk);
  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    double currentTime = ros::Time::now().toSec();
    probeCmdLogic(currentTime);
    begin_probe_pub.publish(probe_cmd);
    probe_cmd.data = false;

    if(isMine)
    {
      ROS_INFO("Object is a mine with radius %f and center x=%f, y=%f",mine.rad,mine.center.x, mine.center.y);
    }
    else
    {
      ROS_INFO("Object is not a mine or not enough points have been sampled yet");
    }

    ros::spinOnce();

    loop_rate.sleep();
  }


  return 0;
}