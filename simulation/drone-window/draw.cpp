#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <drone.h>

int width, height;
static GLuint tex1;
int movementY,movementX;
unsigned char* image;

#define SPACE 0
#define DRONE 1
#define CLOUD 2

void init()
{
	glShadeModel(GL_FLAT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void texSelect(int textureNumber)
{    
	if(textureNumber==SPACE)
	{
		//glBindTexture(GL_TEXTURE_2D, tex1);
		image = SOIL_load_image( "res/terrains/space-terrain.png", &width, &height, 0, SOIL_LOAD_RGBA );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
		SOIL_free_image_data( image );
	}
	if(textureNumber==DRONE)
	{
		image = SOIL_load_image( "res/drones/black-drone.png", &width, &height, 0, SOIL_LOAD_RGBA );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
		SOIL_free_image_data( image );
	}
	if(textureNumber==CLOUD)
	{
		image = SOIL_load_image( "res/obstacles/cloud.png", &width, &height, 0, SOIL_LOAD_RGBA );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
		SOIL_free_image_data( image );
	}  
}

int objX,objY;
int worldX;
int angleRotate;

void update()
{
	objX-=5;
	objY--; 
	worldX-=8;

	if(objX==-2700)
		objX=0;

	if(worldX<-2000)
		worldX=0;

	angleRotate+=30;
	if(angleRotate>360)
		angleRotate=0;
}


void character()
{
	texSelect(DRONE);
	glPushMatrix();
		glTranslatef(40,500,0);
		glTranslatef(movementX,movementY,0);
		glRotatef(-angleRotate,0,0,1);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(-40,-40,-1);
			glTexCoord2f(1.0, 0.0); glVertex3f(40,-40,-1);
			glTexCoord2f(1.0, -1.0); glVertex3f(40,40,-1);                        
			glTexCoord2f(0.0, -1.0); glVertex3f(-40,40,-1);
		glEnd();
	glPopMatrix();
}

void drawObstacle(int posx,int posy, int type)
{
	texSelect(type);
	glPushMatrix();
		glTranslatef(posx,posy,0);
		glTranslatef(objX,0,0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(0,0,-1);
			glTexCoord2f(1.0, 0.0); glVertex3f(200,0,-1);
			glTexCoord2f(1.0, -1.0); glVertex3f(200,100,-1);                                
			glTexCoord2f(0.0, -1.0); glVertex3f(0,100,-1);
		glEnd();
	glPopMatrix();
}

void draw()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_QUADS);
		// glColor3f(0.5,0.5,0.5);
		glVertex3f(0,1080,-1);
		glVertex3f(1920,1080,-1);
		glVertex3f(1920,1040,-1);
		glVertex3f(0,1040,-1);
	glEnd();


	glEnable(GL_TEXTURE_2D);
	// glBindTexture(GL_TEXTURE_2D, tex1);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	texSelect(0);
	glPushMatrix();
		glTranslatef(worldX,0,0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(0,0,-1.1);
			glTexCoord2f(1.0, 0.0); glVertex3f(4000,0,-1.1);
			glTexCoord2f(1.0, -1.0); glVertex3f(4000,1080,-1.1);                        
			glTexCoord2f(0.0, -1.0); glVertex3f(0,1080,-1.1);
		glEnd();
	glPopMatrix();
	character();

	if(rand()%2==0)
	{
		drawObstacle(1920+rand()%(1920-2600),rand()%800, CLOUD);
	}

	glutSwapBuffers();
	glDisable(GL_TEXTURE_2D);
}