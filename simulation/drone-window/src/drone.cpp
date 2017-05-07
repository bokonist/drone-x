#include <GL/freeglut.h>
#include <vector>
#include <drone.h>
#include "bits/stdc++.h"
#include <SOIL/SOIL.h>
#include <population.h>
#include <drone-evaluation.h>

using namespace std;

// some defs for code readability
#define MISSILE 2
#define AIMED_MISSILE 3
#define LAYER1 texName[0]
#define LAYER2 texName[1]
#define LAYER3 texName[2]
#define LAYER4 texName[16]
#define MISSILETEXTURE texName[3]
#define MENUSCREEN texName[17]

GLuint dlist[5];
int width, height;
double tx,ty;
static GLuint texName[18];
double movementY,movementX;
unsigned char* image;
float groundX,skyX,treeXfar,treeXnear;
int ii;
int currentgen,currentorganism;

//obstacle obj;
class obstacle obj;

bool firstRun=true;
bool droneAlive=true;

void mixedStepLoop() // to update frame
{
    double now = glutGet(GLUT_ELAPSED_TIME);
    double timeElapsedMs =(now-prev0);
    queuedMilliseconds += timeElapsedMs ;

    //..DRONE ANIMATION at 12fps
    syncDrone+=timeElapsedMs;
    if(syncDrone>=83)  //drone animation
    {   
        dronePhy++;
        if(dronePhy==15)
        dronePhy=4;
        syncDrone-=83;
    }  
    while(queuedMilliseconds >= responseTime) 
    {
        update();
        movePhysics();
        queuedMilliseconds -= responseTime;
        glutPostRedisplay();
    }
   // score+=(timeElapsedMs/1000);
    prev0=now;
}

void initTextures()
{
	vector<string> s;
	s.push_back("res/terrains/layer-1.png");
	s.push_back("res/terrains/layer-2.png");
	s.push_back("res/terrains/layer-3.png");
	s.push_back("res/obstacles/frame1.png");
	s.push_back("res/drones/seq0000.png");
	s.push_back("res/drones/seq0001.png");
	s.push_back("res/drones/seq0002.png");
	s.push_back("res/drones/seq0003.png");
	s.push_back("res/drones/seq0004.png");
	s.push_back("res/drones/seq0005.png");
	s.push_back("res/drones/seq0006.png");
	s.push_back("res/drones/seq0007.png");
	s.push_back("res/drones/seq0008.png");
	s.push_back("res/drones/seq0009.png");
	s.push_back("res/drones/seq0010.png");
	s.push_back("res/drones/seq0011.png");
	s.push_back("res/terrains/layer-4.png");
	s.push_back("res/menu/splash-bg-alt-with-menu.bmp");


	glGenTextures(s.size(),texName);

	for(int ii=0;ii<s.size();ii++) //load all the textures
	{
		glBindTexture(GL_TEXTURE_2D, texName[ii]);  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		image = SOIL_load_image( s[ii].c_str(), &width, &height, 0, SOIL_LOAD_RGBA );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
		SOIL_free_image_data( image );
	}
	s.clear();

	//.. Transparency and shading parameters
	glShadeModel(GL_FLAT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);	
}

void update() // to update background
{

	skyX-=0.0009*resX*3;
	if(skyX<-resX)
		skyX=0;

	treeXfar-=0.0018*resX*3;
	if(treeXfar<-resX)
		treeXfar=0;

	treeXnear-=0.0020*resX*3;
	if(treeXnear<-resX)
		treeXnear=0;

	groundX-=0.0035*resX*3;
	if(groundX<-resX)
		groundX=0;
}

void drawObstacle(int posx,int posy, int type,int objX)
{
	glBindTexture(GL_TEXTURE_2D, MISSILETEXTURE);
	glPushMatrix();
		glTranslatef(posx,posy,0);
		glTranslatef(objX,0,0);
		glBegin(GL_QUADS);  // (50,25)
			glTexCoord2f(0.0, 0.0); glVertex3f(0,0,0);
			glTexCoord2f(1.0, 0.0); glVertex3f(resX*0.059,0,0);
			glTexCoord2f(1.0, -1.0); glVertex3f(resX*0.059,resY*0.052,0);                        
			glTexCoord2f(0.0, -1.0); glVertex3f(0,resY*0.052,0);
		glEnd();
	glPopMatrix();
}
int temp=0;

void resetSimulation() // to reset game
{
	srand(seedValue);
	temp=0;
	groundX=0;
	skyX=0;
	treeXfar=0;
	treeXnear=0;
	score=0;
	droneAlive=true;
	movementX=0;
	movementY=0;
	obstacleList.clear();
	inputKey.clear();
}

void hitDetection()
{
	for(ii=0;ii<obstacleList.size();ii++)
	{	
		// difference in distance between drone initial X and obstacle size	         	
		if(( abs( (movementY+resY/2+resY*0.20/2)-(obstacleList[ii].y+resY*0.052/2) ) <= (resY*0.052/2+resY*0.20/2-30)))
		{
			obstacleList[ii].evaded=true;
			// Difference in distance between drone centre Y and obstacle centre Y , -30 because drone png has excessive transparent part																					
			if(((abs( (movementX)-( obstacleList[ii].x+obstacleList[ii].objdisp) ) )<= (resX*0.059+resX*0.088-50) ) )
			{
				cout<<int(score)<<endl;
				droneAlive=false;
			 	//start from beginning	
			}
		}
	}
	//.... Boundary check kill drone.
	
	//if( !( movementY <= ( resY-( (resY/2) + (resY*0.20) ) ) )  || !( movementY >= -( resY/2 - (resY*9)/100 ) ) )
	//	droneAlive=false;
	
}
int baseIndex;
void initialiseList()
{
	baseIndex=glGenLists(1);
}

void staticGround()
{
	glNewList(baseIndex+0, GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, LAYER3);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(0,0,0);
			glTexCoord2f(1.0, 0.0); glVertex3f(resX*2,0,0);
			glTexCoord2f(1.0, -1.0); glVertex3f(resX*2,(resY*10)/100,0);
			glTexCoord2f(0.0, -1.0); glVertex3f(0,(resY*10)/100,0);
		glEnd();
	glEndList();
}

void staticSky()
{	
	glNewList(baseIndex+1, GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, LAYER1);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(0,0,0);
			glTexCoord2f(1.0, 0.0); glVertex3f(resX*2,0,0);
			glTexCoord2f(1.0, -1.0); glVertex3f(resX*2,resY,0);
			glTexCoord2f(0.0, -1.0); glVertex3f(0,resY,0);
		glEnd();
	glEndList();
}

void staticTreeFar()
{
	glNewList(baseIndex+2, GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, LAYER2);	
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(0,0,0);
			glTexCoord2f(1.0, 0.0); glVertex3f(resX*2,0,0);
			glTexCoord2f(1.0, -1.0); glVertex3f(resX*2,resY,0);
			glTexCoord2f(0.0, -1.0); glVertex3f(0,resY,0);
		glEnd();
	glEndList();
}


void staticTreeNear()
{
	glNewList(baseIndex+4, GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, LAYER4);	
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(0,0,0);
			glTexCoord2f(1.0, 0.0); glVertex3f(resX*2,0,0);
			glTexCoord2f(1.0, -1.0); glVertex3f(resX*2,resY,0);
			glTexCoord2f(0.0, -1.0); glVertex3f(0,resY,0);
		glEnd();
	glEndList();
}

void staticDrone()
{
	glNewList(baseIndex+3, GL_COMPILE);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(0,resY/2,0);
			glTexCoord2f(1.0, 0.0); glVertex3f(resX*0.088,resY/2,0);
			glTexCoord2f(1.0, -1.0); glVertex3f(resX*0.088,(resY/2)+(resY*0.20),0); 
			glTexCoord2f(0.0, -1.0); glVertex3f(0,(resY/2)+(resY*0.20),0);
			glEnd();
	glEndList();
}

GLvoid *font_style = GLUT_STROKE_ROMAN;

void renderStrokeFont(int x,int y,int z,const char* temp)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	glLineWidth(2);
	glScalef(0.2,0.2,0.2);
	const char *c;
	for (c=temp; *c != '\0'; c++) 
		glutStrokeCharacter(font_style, *c);
	glPopMatrix();
}


int runonce;

void draw()
{
	
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(menu==0||runonce==0)
	{
		if(runonce==0)
		{
		srand(seedValue);
		initialiseList();
		staticDrone();
		staticSky();
		staticTreeFar();
		staticTreeNear();
		staticGround();
		runonce++;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else
		{
		glBindTexture(GL_TEXTURE_2D, MENUSCREEN);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(0,0,0);
			glTexCoord2f(1.0, 0.0); glVertex3f(resX,0,0);
			glTexCoord2f(1.0, -1.0); glVertex3f(resX,resY,0);
			glTexCoord2f(0.0, -1.0); glVertex3f(0,resY,0);
		glEnd();
		}
	}
	else
	{
		//...WORLD	

		//SKY
		glPushMatrix();
		glTranslatef(skyX,0,0);
		glCallList(baseIndex+1);
		glPopMatrix();

		//TREE FAR
		glPushMatrix();
		glTranslatef(treeXfar,0,0);
		glCallList(baseIndex+2);
		glPopMatrix();

		//TREE NEAR
		glPushMatrix();
		glTranslatef(treeXnear,0,0);
		glCallList(baseIndex+4);
		glPopMatrix();

		//GROUND	
		glPushMatrix();
		glTranslatef(groundX,0,0);
		glCallList(baseIndex+0);
		glPopMatrix();

		//TOP-SCORE BAR
		glPushMatrix();
		glBegin(GL_QUADS);
		glColor3f(0.5,0.5,0.5);
		glVertex3f(0,resY*0.94,3);
		glVertex3f(resX,resY*0.94,3);
		glVertex3f(resX,resY,3);
		glVertex3f(0,resY,3);
		glEnd();
		glPopMatrix();

		//...SCORE
		glColor3f(1,1,1);
		int tempscore;
		tempscore=score;
		string str="SCORE: "+to_string(tempscore) + "   Generation : " + to_string(currentgen) + "   Organism : " + to_string(currentorganism);
		const char * c = str.c_str();
		renderStrokeFont(resX*0.092,resY*0.95,3,c);

		// OBSTACLES
		glColor3f(1,1,1);
		for(ii=0;ii<obstacleList.size();ii++)
		{
			/*todo : add the code to update the evaded variable, if the drone is in the path of the obstable*/
			if ( obstacleList[ii].objdisp<= -( resX+(resX*0.32) ) ) // moved out of screen
			{
				if(obstacleList[ii].evaded)
				{
					if(obstacleList[ii].type== AIMED_MISSILE)
						score+=10;
					else
						score+=5;
				}
				else
				score+=1;
				obstacleList.erase(obstacleList.begin()+ii);
			}
			
			obstacleList[ii].objdisp-=0.0047*resX*3;
			if(obstacleList[ii].type==AIMED_MISSILE)
			glColor3ub(245,75,75);
			//..OBSTACLES	
			drawObstacle(obstacleList[ii].x, obstacleList[ii].y, obstacleList[ii].type,obstacleList[ii].objdisp);
			glColor3f(1,1,1);
			
		}
		int randno=0;
		if(temp==90) // generate new obstable
		{
			
			temp=0;
			randno= rand();
			//... Aimed missile disabled for now.
			/*
			if((randno %10) < 4) // 40% chance of launched missile being aimed at the drone. this is to discourage the drone standing still and getting high scores.
			{
				tx= resX+(randno %(31*resX/100));
				ty= movementY+resY/2 + 20;
				obj.type=AIMED_MISSILE;
			} 
			*/
			//else
			//{
				tx= resX+(randno %(31*resX/100));
				ty= (resY*10/100)+ randno %(83*resY/100);
				obj.type= MISSILE;	
			//}
			obj.x=tx;
			obj.y=ty;
			obj.norm_x = tx/resX;
			obj.norm_y = ty/resY;
			cout<<obj.norm_x<< " "<<obj.norm_y<<endl;
			obj.objdisp=0;
			obstacleList.push_back(obj);
		}
		else
		{
			temp++;
		}
		//..DRONE
		glPushMatrix();
		glTranslatef(movementX,movementY,0);
		glBindTexture(GL_TEXTURE_2D, texName[dronePhy]);
		glCallList(baseIndex+3);
		glPopMatrix();
		hitDetection();
	}	
	if(firstRun)
	{
		firstRun=false;
		NEAT::Population *p=0;
    	p = drone_test(1000); //run 1000 generations on drone
    }
	glutSwapBuffers();
}



//int countUp,countDown;
void movePhysics() // for smooth movements
{

	int ii;
	double s=0.001;
	int multiplier=3;
	for(ii=0;ii<inputKey.size();ii++)	
	{
		if(inputKey[ii]=='U')
		{
			if( movementY <= ( resY-( (resY/2) + (resY*0.20) ) ) )  //Up Boundary check
			movementY+=s*multiplier;
			inputKey.erase(inputKey.begin()+ii);
		}
		else if(inputKey[ii]=='D')
		{
			if( movementY >= -( resY/2 - (resY*9)/100 ) ) // Down boundary check
			movementY-=s*multiplier;
			inputKey.erase(inputKey.begin()+ii);
		}					

	}	
	
}
