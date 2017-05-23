# drone-x - NeuroEvolution self learning drone
[![GitHub release](https://img.shields.io/github/release/bokonist/drone-x.svg)](https://github.com/bokonist/drone-x/releases)

## Contents

- [About](#about)
- [Requirements](#requirements)
- [Installation](#installation)
- [Frequently Asked Questions](#frequently-asked-questions)

## About
[![Drone-x Demo](http://i.imgur.com/3N7J26U.jpg)](https://www.youtube.com/watch?v=gq39JeI6yxo "Drone-demo")

A self learning AI drone to evade obstacles. The simulation is made using Opengl & neural network using C++

## Requirements
### Runtime & Development

- C++ Compiler >= GCC 4.8.1
- Freeglut-dev >= Freeglut 3.0.0
- SOIL image loader library >= SOIL 1.16

## Installation
To download the repo & Run the simulation:

	git clone https://github.com/bokonist/drone-x
	cd drone-x/simulation/drone-window
	make
	./drone

To uninstall & delete generation files:

	make clean
	rm -r gen*
	rm drone

## Frequently Asked Questions
### Which part of the source code is the graphics simulation part?
src/drone.cpp , main.cpp & include/drone.h are the Opengl part of the code. These parts can be changed to change the simulation speed, obstacle type, menu system.

### Experiencing stutter and performance lose?
Check to make sure you have a supported graphics card & install its latest driver. Run with vblank_mode=1 to enable v-sync. If you still experience stutter, then comment out the gamemode part in main.cpp and run the game in gamemode instead of window.

### Is it possible to port this to windows / getting error for to_string() function?
Yes, the freeglut & soil are cross platform libraries, when compiling make sure the latest version of gcc is installed using Mingw-w64 for the latest version of g++ compiler otherwise you may get an error for to_string() function or alternatively you can add a patch for it yourself.

### How fast can the simulation run/ How to speed up simulation?
In main.cpp change fps parameter value & run the game with the flag vblank_mode=0 to disable vsync. maximum framerate depends on your system configuration. Alternatively you can change the the obstacle speed parameter in drone.cpp to achieve the same effect.

### Why is my system getting loud/ single core cpu usage 100%?
This is a single threaded application for ease of understanding. The graphics part and neural code is run on an infinite loop, therefore the cpu usage will be maximum 100% for a single core even if that loop is not doing anything.




