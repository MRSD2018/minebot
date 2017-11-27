import numpy as np
import cv2
import subprocess
import time


class Contour:
    def __init__(self):
        return


# these are color filters in HSV
# Hue is 0 to 180
# Saturation and Value are 0 to 255
lower_red = np.array([0, 75, 85])
upper_red = np.array([1, 255, 255])

lower_red2 = np.array([165, 75, 85])
upper_red2 = np.array([180, 255, 255])


def analyze_image(frame):
    # start = time.time()
    # change the captured frame from BGR (blue-green-red) to HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # filter on color
    mask = cv2.inRange(hsv, lower_red, upper_red)
    mask2 = cv2.inRange(hsv, lower_red2, upper_red2)
    mask = mask | mask2
    # res = cv2.bitwise_and(frame, frame, mask=mask)

    # opencv function that finds shapes
    try:
        mask, contours, hierarchy = cv2.findContours(mask, cv2.RETR_LIST, cv2.CHAIN_APPROX_NONE)
    except ValueError:
        contours, hierarchy = cv2.findContours(mask, cv2.RETR_LIST, cv2.CHAIN_APPROX_NONE)

    newContours = []
    areas = []

    # this is where the code gets weird
    # start by looking at each contour
    for index, c in enumerate(contours):
        # find the arc length (aka circumference) of the contour and that must be bigger than a particular size
        per = cv2.arcLength(c, True)
        approx = cv2.approxPolyDP(c, 2, True)
        # hull = cv2.convexHull(approx)
        # area = cv2.contourArea(hull)
        area = cv2.contourArea(approx)
        # if cv2.contourArea(hull) < 0.01:
        if area < 0.01:
            continue
        # solidity = cv2.contourArea(approx)/cv2.contourArea(hull)
        print(index, per, area)

        # this is where the filtering happens
        if 30 > area or area > 150:
            continue
        # elif solidity < 0.8:
        #     continue

        # this stuff just stores the contours that we like
        contour = Contour()
        areas.append(area)
        M = cv2.moments(approx)
        cX = int(M["m10"] / M["m00"])
        cY = int(M["m01"] / M["m00"])
        contour.area = area
        contour.contour = approx
        contour.cX = cX
        contour.cY = cY
        newContours.append(contour)
        # print(index, cX, cY, per, cv2.contourArea(approx))

    print(len(newContours))

    # this draws all the good contours
    cv2.drawContours(mask, [c.contour for c in newContours], -1, (128, 255, 0), 1)

    # Display the resulting frame
    if __name__ == "__main__":
        cv2.imshow('frame', frame)
        cv2.imshow('mask', mask)

    # figure out how strong the signal is
    newContours.sort(key=lambda x: x.cX, reverse=False)

    # end = time.time()
    # print "Time to process one frame: ", end - start

    # if there's no lights, then return just that
    if len(newContours) == 0:
        return 0
    # if there's only one light, there's only one light
    elif len(newContours) == 1:
        return 1
    elif len(newContours) == 2:
        # if the two are far apart
        if newContours[1].cX - newContours[0].cX > 300:
            return 15
        else:
            return 2
    else:
        # if the distance between the last two is much bgger than the typical distance between
        typ = np.mean([newContours[i+1].cX - newContours[i].cX for i in range(len(newContours) - 2)])
        ult = newContours[-1].cX - newContours[-2].cX
        if ult > typ + 25:
            return 14 + len(newContours) - 1
        else:
            return len(newContours)


def main():

    '''
    frame = cv2.imread('md_lights/distracted.png')
    analyze_image(frame)
    cv2.waitKey(0)
    '''


    cap = cv2.VideoCapture(0)
    cap.read()
    subprocess.call('v4l2-ctl -d /dev/video0 -c exposure_auto=1', shell=True)
    subprocess.call('v4l2-ctl -d /dev/video0 -c exposure_absolute=10', shell=True)

    while(True):
        # Capture frame-by-frame
        ret, frame = cap.read()

        res = analyze_image(frame)
        # print res

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
