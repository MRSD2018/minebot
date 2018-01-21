#!/usr/bin/env python

import rospy
import cv2
import subprocess
from std_msgs.msg import Int8
from webcam import analyze_image
import time

DEVICE = 0


def main():
    rospy.init_node('md_vision')
    pub = rospy.Publisher('md_signal', Int8, queue_size=10)

    cap = cv2.VideoCapture(DEVICE)
    # cap.set(cv2.CV_CAP_PROP_BUFFERSIZE, 0)
    cap.read()
    subprocess.call('v4l2-ctl -d /dev/video'+str(DEVICE)+' -c exposure_auto=1', shell=True)
    subprocess.call('v4l2-ctl -d /dev/video'+str(DEVICE)+' -c exposure_absolute=10', shell=True)
    # subprocess.call('v4l2-ctl -d /dev/video0 -c exposure_auto=1', shell=True)
    # subprocess.call('v4l2-ctl -d /dev/video0 -c exposure_absolute=10', shell=True)

    start = time.time()
    r = rospy.Rate(10)  # 10hz
    while not rospy.is_shutdown():
        print "Time since last frame: ", time.time() - start
        start = time.time()
        # Capture frame-by-frame
        ret, frame = cap.read()

        if (time.time() - start) < 0.010:
            continue

        res = analyze_image(frame)
        pub.publish(Int8(res))
        # print res

        if cv2.waitKey(100) & 0xFF == ord('q'):
            break

        r.sleep()

    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
