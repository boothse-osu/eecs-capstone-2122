# Required libraries for main program

I would normally not put the libraries up on github, except that this project is to be picked up by users who struggled with the linker. This will be a lot easier for them.

The following instructions were already written and are still here in case the linking somehow breaks and needs to be fixed. Otherwise the configuration for the visual studio project should be set up automatically when pulling down the project.

Guide to adding additional include directories: https://docs.microsoft.com/en-us/cpp/build/reference/i-additional-include-directories?view=msvc-170

Explanation of filepaths (due to users struggling with them): https://en.wikipedia.org/wiki/Path_(computing)

## cglm
* version 0.8.4
* https://github.com/recp/cglm
* Need to add the following filepath to (Project->Properties->C/C++->General->Additional Include Directories): "..\Libraries\cglm-0.8.4\include\cglm" 
* Used for vector and matrix math for the IK algorithms

## CSerialPort
* https://github.com/ABasharEter/CSerialPort
* The following filepath needs to be added to (Project->Properties->C/C++->General->Additional Include Directories): "..\Libraries\CSerialPort-master\CSerialPort"
* Also, the CSerial.c needs to be renamed to CSerial.cpp for reasons I don't completely understand
* Essentially quick functions for doing serial communication so I didn't have to learn the windows API

