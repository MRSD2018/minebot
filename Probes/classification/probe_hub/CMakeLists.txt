cmake_minimum_required(VERSION 2.8.3)
project(probe_hub)
add_definitions(-std=c++11)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg)

## Declare ROS messages and services

## Generate added messages and services
#generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(probe_hub src/probe_hub_node.cpp)
target_link_libraries(probe_hub ${catkin_LIBRARIES})
#add_dependencies(talker beginner_tutorials_generate_messages_cpp)