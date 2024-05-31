# SemaFORR + TurtleBot2 Stage
A cognitively-based system for robot navigation

Implemented SemaFORR to work with TurtleBot2 StageROS simulator

## Debugging
One or both solves the missing sdl-image problem
* sudo apt-get install libsdl2-image-dev
* sudo apt-get install libsdl-image1.2-dev

One or both solves the missing sdl-ttf problem
* sudo apt-get install libsdl2-ttf-dev
* sudo apt-get install libsdl-ttf2.0-dev

Solves the missing move-base ros library problem
* sudo apt-get install ros-noetic-move-base

Encountered a problem with missing include
* Go to why and why_plan directories and “mkdir include” in both of them. This should solve the issue.

Issue with Python 2.7 not found
* sudo apt-get install python-dev – this is for python 2.7
* sudo apt-get install python3
