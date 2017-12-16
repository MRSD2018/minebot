#!/usr/bin/env python

import rospy
from std_msgs.msg import Int8, Int16MultiArray
from operator import itemgetter


prev_pos = (0, 0)
prev_forward = True
pos_history = []
detection_history = []


def close(one, two):
    if abs(one - two) < 0.0001:
        return True
    return False


def update_detection(data):
    global detection_history
    pos_history.append(prev_pos)
    detection_history.append(data.data)


def update_pos(data):
    global prev_pos
    global prev_forward
    global detection_history

    if close(data.data[3], prev_pos[0]) and close(data.data[4], prev_pos[1]):
        return

    # do for lateral only, not long
    new_forward = data.data[3] > prev_pos[0]

    prev_pos = (data.data[3], data.data[4])

    if new_forward != prev_forward:
        prev_forward = new_forward
        check_history()
        detection_history = []


def check_history():
    global detection_history
    if not detection_history: return
    index, element = max(enumerate(detection_history), key=itemgetter(1))
    if element > 10:
        print rospy.get_time(), pos_history[index], element


def main():
    rospy.init_node('md_analysis')

    sub = rospy.Subscriber('md_signal', Int8, update_detection)
    sub2 = rospy.Subscriber('gantryStat', Int16MultiArray, update_pos)

    rospy.spin()

if __name__ == "__main__":
    main()
