#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <drone.h>
#include <bits/stdc++.h>
#include <stdlib.h>

using namespace std;

GLuint dlist[5];
int width, height,tx,ty;
static GLuint texName[17];
int movementY,movementX;
unsigned char* image;

struct obstacle
{
	int x,y;
	int type;
	int objdisp;
};

vector<obstacle> obstacleList;
obstacle obj;

#define SPACE 0
#define DRONE 1
#define CLOUD 2
#define LAYER1 texName[0]
#define LAYER2 texName[1]
#define LAYER3 texName[2]
#define LAYER4 texName[16]
#define CLOUDTEXTURE texName[3]

void init1Texture(const string s,int ii)
{

	glBindTexture(GL_TEXTURE_2D, texName[ii]);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	image = SOIL_load_image( s.c_str(), &width, &height, 0, SOIL_LOAD_RGBA );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
	SOIL_free_image_data( image );

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

for(int ii=0;ii<s.size();ii++)
{
	init1Texture(s[ii],ii);
}
	s.clear();
	//.. Transparency and shading parameters
	glShadeModel(GL_FLAT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);	
}

float groundX,skyX,treeXfar,treeXnear;
int ii;

void update()
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
	glBindTexture(GL_TEXTURE_2D, CLOUDTEXTURE);
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

void resetFunc()
{
	temp=0;
	groundX=0;
	skyX=0;
	treeXfar=0;
	treeXnear=0;
	score=0;
	movementX=0;
	movementY=0;
	obstacleList.clear();
}

void hitDetection()
{
	for(ii=0;ii<obstacleList.size();ii++)
	{	// difference in distance between drone initial X and obstacle size					 Difference in distance between drone centre Y and obstacle centre Y , -30 because drone png has excessive transparent part																					
		if(((abs( (movementX)-( obstacleList[ii].x+obstacleList[ii].objdisp) ) )<= (resX*0.059+resX*0.088-50) ) && ( abs( (movementY+resY/2+resY*0.20/2)-(obstacleList[ii].y+resY*0.052/2) ) <= (resY*0.052/2+resY*0.20/2-30) ) )
		{
		//printf("\t\tHIT:%d %d %d %d \n",abs((movementX)-(obstacleList[ii].x+obstacleList[ii].objdisp)),abs((movementY+resY/2)-(obstacleList[ii].y)),(movementY+resY/2),(obstacleList[ii].y));
		resetFunc();	
		}
	}

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
			obstacleList[ii].objdisp-=0.0047*resX;
			if ( obstacleList[ii].objdisp== -( resX+(resX*0.32) ) )
			{
				obstacleList.erase(obstacleList.begin()+ii);
			}
			else
			{
				//..OBSTACLES	
				drawObstacle(obstacleList[ii].x, obstacleList[ii].y, obstacleList[ii].type,obstacleList[ii].objdisp);
			}
		}

		if(temp==75)
		{
			temp=0;
			tx= resX+(rand()%(31*resX/100));
			ty= (resY*10/100)+rand()%(83*resY/100);
			obj.x=tx;
			obj.y=ty;
			obj.type= CLOUD;
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
	glutSwapBuffers();
}
