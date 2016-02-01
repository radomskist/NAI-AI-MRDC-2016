# NAI-ComputerVision

##Requires:
Open Kinect 2:
https://github.com/OpenKinect/libfreenect2
http://openkinect.org/wiki/Main_Page


SDL 2:

https://www.libsdl.org/download-2.0.php

https://www.libsdl.org/


GLEW:

http://glew.sourceforge.net/install.html

http://glew.sourceforge.net/


GLM:

https://github.com/g-truc/glm

http://glm.g-truc.net/0.9.7/index.html


OpenCV (3.1):

http://opencv.org/downloads.html

http://opencv.org/




##Special folders
Create a depends folder in the main directory to have libraries such as libusb and freenect2 in them.

Create a include folder in the main directory and copy the freenect2, libusb, GLM, and glew headers into their respective folders.


###Arguments
-ct = webcam test, uses webcame display as window

-kdep = kinect depth display, if there is a kinect connected it'll use this by default.

-krgb = kinect rgb display.

-mem = memory map, uses the memory map. Used when all other options fail.

###Memory map controls:
WASD, move north, south, west and east respectively.

Arrow keys = rotate camera

f = toggle path clipping

esc = close program
