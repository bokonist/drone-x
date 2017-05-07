#include <bits/stdc++.h>
using namespace std;
extern double queuedMilliseconds,prev0,fps,responseTime,score,syncDrone,movementX,movementY;
extern int speed,dronePhy,menu;
extern int resX,resY;
extern bool droneAlive;
extern int currentgen, currentorganism,seedValue;

class obstacle // data structure for obstacles
{
	public:
		double x,y; // coordinates
		double norm_x,norm_y;
		int type; //for future expansion
		int objdisp; // object displacement
		bool evaded; // if this obstable was evaded by the drone
		obstacle()
		{
			x=0;
			y=0;
			norm_x=0;
			norm_y=0;
			type=0;
			objdisp=0;
			evaded=false;
		}
};

extern vector<obstacle> obstacleList;
extern vector<char> inputKey;
void initTextures();
void processSpecialKeys(int , int , int );
void mixedStepLoop();
void draw();
void update();
void movePhysics();
void resetSimulation();