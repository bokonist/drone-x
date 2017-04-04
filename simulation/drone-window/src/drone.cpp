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

GLuint dlist[5];
int width, height,tx,ty;
static GLuint texName[17];
double movementY,movementX;
unsigned char* image;
float groundX,skyX,treeXfar,treeXnear;
int ii;

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
void moveDroneTo(double ratio=0, double droneNormalY=0)
{
	double destination=0;
	destination = resY*ratio;
	double currentPos= droneNormalY*resY;
	double displacement=0.48;
	while(!(ceil(destination*100) == ceil(currentPos*100)))
	{
		if(ratio > droneNormalY)
		{
			inputKey.push_back('U');
			currentPos += displacement ;
		}
		else
		{
			inputKey.push_back('D');
			currentPos -= displacement ;	
		}
	}
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

	skyX-=0.0009*resX;
	if(skyX<-resX)
		skyX=0;

	treeXfar-=0.0018*resX;
	if(treeXfar<-resX)
		treeXfar=0;

	treeXnear-=0.0020*resX;
	if(treeXnear<-resX)
		treeXnear=0;

	groundX-=0.0035*resX;
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
			//printf("\t\tHIT:%d %d %d %d \n",abs((movementX)-(obstacleList[ii].x+obstacleList[ii].objdisp)),abs((movementY+resY/2)-(obstacleList[ii].y)),(movementY+resY/2),(obstacleList[ii].y));
				cout<<int(score)<<endl;
				droneAlive=false;
			 	//start from beginning	
			}
		}
	}
	//.... Boundary check kill drone.
	
	if( !( movementY <= ( resY-( (resY/2) + (resY*0.20) ) ) )  || !( movementY >= -( resY/2 - (resY*9)/100 ) ) )
		droneAlive=false;
	
}

void initialiseList()
{
	dlist[0]=glGenLists(1);
	dlist[1]=glGenLists(1);
	dlist[2]=glGenLists(1);
	dlist[3]=glGenLists(1);
	dlist[4]=glGenLists(1);
}

void staticGround()
{
	glNewList(dlist[0], GL_COMPILE);
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
	glNewList(dlist[1], GL_COMPILE);
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
	glNewList(dlist[2], GL_COMPILE);
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
	glNewList(dlist[4], GL_COMPILE);
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
	glNewList(dlist[3], GL_COMPILE);
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


int opt1;
void draw()
{
	
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(opt1==0)
	{
		initialiseList();
		staticDrone();
		staticSky();
		staticTreeFar();
		staticTreeNear();
		staticGround();
		opt1=1;
	}
	else
	{
		//...WORLD	

		//SKY
		glPushMatrix();
		glTranslatef(skyX,0,0);
		glCallList(dlist[1]);
		glPopMatrix();

		//TREE FAR
		glPushMatrix();
		glTranslatef(treeXfar,0,0);
		glCallList(dlist[2]);
		glPopMatrix();

		//TREE NEAR
		glPushMatrix();
		glTranslatef(treeXnear,0,0);
		glCallList(dlist[4]);
		glPopMatrix();

		//GROUND	
		glPushMatrix();
		glTranslatef(groundX,0,0);
		glCallList(dlist[0]);
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
		string str="SCORE: "+to_string(tempscore);
		const char * c = str.c_str();
		renderStrokeFont(resX*0.092,resY*0.95,3,c);

		// OBSTACLES
		glColor3f(1,1,1);
		for(ii=0;ii<obstacleList.size();ii++)
		{
			/*todo : add the code to update the evaded variable, if the drone is in the path of the obstable*/
			obstacleList[ii].objdisp-=0.0047*resX;
			if ( obstacleList[ii].objdisp<= -( resX+(resX*0.32) ) )
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
			else
			{
				if(obstacleList[ii].type==AIMED_MISSILE)
				glColor3ub(245,75,75);
				//..OBSTACLES	
				drawObstacle(obstacleList[ii].x, obstacleList[ii].y, obstacleList[ii].type,obstacleList[ii].objdisp);
				glColor3f(1,1,1);
			}
		}
		int randno=0;
		if(temp==120) // generate new obstable
		{
			
			temp=0;
			randno= rand();
			if((randno %10) < 4) // 40% chance of launched missile being aimed at the drone. this is to discourage the drone standing still and getting high scores.
			{
				tx= resX+(randno %(31*resX/100));
				ty= movementY+resY/2 + 20;
				obj.type=AIMED_MISSILE;
			}
			else
			{
				tx= resX+(randno %(31*resX/100));
				ty= (resY*10/100)+ randno %(83*resY/100);
				obj.type= MISSILE;	
			}
			obj.x=tx;
			obj.y=ty;
			obj.norm_x = tx/resX;
			obj.norm_y = ty/resY;
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
		glCallList(dlist[3]);
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
	/*
	int ii;
	for(ii=0;ii<inputKey.size();ii++)	
	{
		// ...UP KEY ANIMATION  7*4=28
		if(inputKey[ii]=='U')
		{
			if( movementY <= ( resY-( (resY/2) + (resY*0.20) ) ) )  //Up Boundary check
			movementY+=0.0047*resX;
			countUp++;
		}					
		if(countUp==7)
		{	
			countUp=0;	
			inputKey.erase(inputKey.begin()+ii);
		}	
		if(countUp>0)
			break;
		
		//...DOWN KEY ANIMATION  7*4=28
		if(inputKey[ii]=='D')
		{
			if( movementY >= -( resY/2 - (resY*9)/100 ) ) // Down boundary check
			movementY-=0.0047*resX;
			countDown++;
		}	
		if(countDown==7)
		{
			countDown=0;
			inputKey.erase(inputKey.begin()+ii);
		}
		if(countDown>0)
			break;
	}
	*/
	int ii;
	double s=0.001;
	int multiplier=1;
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
/*
void processSpecialKeys(int key, int xx, int yy)
{

	switch (key) 
	{
		case GLUT_KEY_DOWN :
				//if(movementY!=-440)
				inputKey.push_back('D');
			break;
		case GLUT_KEY_UP :
				//if(movementY!=440)
				inputKey.push_back('U');
			break;
	}
}
*/
