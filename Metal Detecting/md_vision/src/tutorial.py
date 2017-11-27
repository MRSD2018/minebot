import numpy as np
from matplotlib import pyplot as plt
import cv2
import time
import os

print os.listdir(os.getcwd()+"/SampleImages")
img = cv2.imread('SampleImages/10ft2.jpg')
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

for val in hsv:
    for subVal in val:
        subVal[0] += 90 if subVal[0] + 90 < 255 else 90 - 255


# define range of blue color in HSV
lower_red = np.array([95, 150, 150])
upper_red = np.array([105, 255, 255])

# Threshold the HSV image to get only blue colors
mask = cv2.inRange(hsv, lower_red, upper_red)

'''
# define range of red color in HSV
lower_red = np.array([85, 150, 150])
upper_red = np.array([95, 255, 255])

# Threshold the HSV image to get only blue colors
mask2 = cv2.inRange(hsv, lower_red, upper_red)
'''

# Bitwise-AND mask and original image
res = cv2.bitwise_and(img, img, mask=mask)

cv2.imshow('frame', img)
# cv2.imshow('mask', mask)
cv2.imshow('res', res)

cv2.waitKey(0)
cv2.destroyAllWindows()