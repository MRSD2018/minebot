#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16
import math

period = 4.0  # seconds range
freq = 1 / period
range = 800.0  # peak mm range
sim_rate = 60.0


def main():
    rospy.init_node('gantry_sim')
    pub = rospy.Publisher('gantry_pos', Int16, queue_size=10)
    time = 0.0

    r = rospy.Rate(sim_rate)  # 30 hz
    while not rospy.is_shutdown():
        val = range / 2 + range / 2 * math.sin(2 * math.pi * freq * time)
        print val
        pub.publish(Int16(int(val)))
        time += 1 / sim_rate
        # print res

        r.sleep()


if __name__ == "__main__":
    main()
