# eecs-capstone-2122
Mobile 3D printer project for OSU EECS Capstone 2021-2022

## Required libraries for main program

### cglm
* version 0.8.4
* https://github.com/recp/cglm
* Need to add path of folder containing "cglm-0.8.4\include\cglm\clgm.h" to (Project->Properties->C/C++->General->Additional Include Directories) 
* Used for vector and matrix math for the IK algorithms

### libusb
* version 1.0.25
* https://libusb.info/
* Considerably more involved library installation. Read the library's README for instructions.
* I have only gotten it to work with the dll version.
