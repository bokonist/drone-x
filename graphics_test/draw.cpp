#include<GL/glut.h>
#include<SOIL/SOIL.h>
#include<myheader.h>

int width, height;
static GLuint tex1;
int flag;
int movementY,movementX;

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
unsigned char* image;
void texSelect()
{    
  
 if(flag==0)
 {
   //glBindTexture(GL_TEXTURE_2D, tex1);
   image = SOIL_load_image( "img/sky.png", &width, &height, 0, SOIL_LOAD_RGBA );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
   SOIL_free_image_data( image );

 }

if(flag==1)
   {
   image = SOIL_load_image( "img/drone250.png", &width, &height, 0, SOIL_LOAD_RGBA );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
   SOIL_free_image_data( image );
   
   }
 if(flag==2)
 {
   image = SOIL_load_image( "img/cloud.png", &width, &height, 0, SOIL_LOAD_RGBA );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
   SOIL_free_image_data( image );
   
 }  
   
}

int objX,objY;
float worldX;
float angleRotate;

void update()
{
 objX-=5;
 objY--; 
 worldX-=0.5;

if(objX==-2700)
   objX=0;

 if(worldX==-2000)
   worldX=0;

angleRotate+=0.5;
if(angleRotate==360)
angleRotate=0;
}


void character()
{
   flag=1;
   texSelect();

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


void drawObstacle(int posx,int posy)
{
flag=2;
texSelect();

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
   
   flag=0;
   texSelect();

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

   drawObstacle(2000,400);
   drawObstacle(2400,600);
   drawObstacle(2200,800);
   drawObstacle(2600,220);

   glutSwapBuffers();
   glDisable(GL_TEXTURE_2D);
}

