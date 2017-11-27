#!/usr/bin/env python

import matplotlib.pyplot as plt
import rospy
from std_msgs.msg import Int16
from std_msgs.msg import Int8
from drawnow import drawnow

cur_pos = 0
cur_det = 0

positions = []
detections = []


def update_pos(data):
    global cur_pos
    cur_pos = data.data
    if cur_pos < 1.0:
        global positions
        global detections
        positions = []
        detections = []


def update_detection(data):
    global cur_pos
    global cur_det
    cur_det = data.data
    positions.append(cur_pos)
    detections.append(cur_det)
    drawnow(draw_cur_pos)
    # draw_cur_pos()


def draw_cur_pos():
    ax = plt.subplot(111)
    ax.set_xlim(0, 800)
    ax.set_ylim(0, 30)
    ax.plot(cur_pos, cur_det, 'kx')
    ax.plot(positions, detections, 'k')


def main():
    global positions
    global detections

    positions = []
    detections = []

    rospy.spin()

if __name__ == "__main__":
    rospy.init_node('plotter')
    sub = rospy.Subscriber('md_signal', Int8, update_detection)
    sub2 = rospy.Subscriber('gantry_pos', Int16, update_pos)
    main()
