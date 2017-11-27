import cv2
import numpy as np

# this is just an object to
class Contour:
    def __init__(self):
        return

#load video from file
vid = cv2.VideoCapture('hopper.avi')


# these are color filters in HSV
# Hue is 0 to 180
# Saturation and Value are 0 to 255
lower_red = np.array([45, 5, 35])
upper_red = np.array([65, 255, 225])

while True:
    # Capture frame-by-frame
    ret, frame = vid.read()

    # change the captured frame from BGR (blue-green-red) to HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # filter on color
    mask = cv2.inRange(hsv, lower_red, upper_red)
    res = cv2.bitwise_and(frame, frame, mask=mask)

    # opencv function that finds shapes
    mask, contours, hierarchy = cv2.findContours(mask, cv2.RETR_LIST, cv2.CHAIN_APPROX_NONE)

    newContours = []
    areas = []

    # this is where the code gets weird
    # start by looking at each contour
    for index, c in enumerate(contours):
        # find the arc length (aka circumference) of the contour and that must be bigger than a particular size
        per = cv2.arcLength(c, True)
        if per > 150:
            # also weird. This compares the area of the contour to
            # the area if you wrapping a string around the contour tightly (the convex shape)
            # solidity is the ratio of these, and it must be within a range
            # approx tries to simplify the contour and then says it must be less than X points
            # and then the total area must be bigger than a certain size
            # these were all tuned to the specific video we had
            # you can't do arc length or total area filtering if the camera moves really far and really close to the target
            approx = cv2.approxPolyDP(c, 2, True)
            hull = cv2.convexHull(approx)
            area = cv2.contourArea(hull)
            solidity = cv2.contourArea(approx)/cv2.contourArea(hull)
            if 0.1 < solidity < 0.5 and len(approx) < 17 and area > 700:

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

    finalContours = []

    # this displays a circle over the contour saved with the biggest area
    for x, c in enumerate(newContours):
        if areas[x] == max(areas):
            finalContours.append(c)
            cv2.circle(mask, (c.cX, c.cY), 7, (255, 255, 255), -1)

    # print([len(x) for x in newContours])
    # print(areas)

    # this draws all the good contours
    cv2.drawContours(mask, [c.contour for c in newContours], -1, (128, 255, 0), 3)
    # print(len([c for c in contours if cv2.arcLength(c, True) > 150]))
    # cv2.imshow('res', res)
    cv2.imshow("mask", mask)
    cv2.imshow("frame", frame)
    cv2.waitKey(1)
