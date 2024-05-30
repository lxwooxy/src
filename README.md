# SemaFORR + TurtleBot2 Stage
A cognitively-based system for robot navigation

Implemented SemaFORR to work with TurtleBot2 StageROS simulator

Issues I have run across:
One or both solve the missing sdl-image problem
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl-image1.2-dev

For me, the second one solved the missing sdl-ttf problem
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libsdl-ttf2.0-dev

Solves the missing move-base ros library problem
sudo apt-get install ros-noetic-move-base

Encountered a problem with missing include
Go to why and why_plan directories and “mkdir include” in both of them. This should solve the issue.

Issue with return type in Graph.cpp in /src/menge_ros/menge_core/src/resources/Graph.cpp
Change line 139 to “return 0;” instead of “return false;”

Issue with return type in HeightField.cpp
Change line 130, 136, 141 to “return 0;” instead of “return false;”

Issue with Python 2.7 not found
sudo apt-get install python-dev – this is for python 2.7
sudo apt-get install python3
