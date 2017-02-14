#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <SOIL/SOIL.h>

static GLuint texName;
static int wh = 1080;
static int ww = 1920;
GLvoid *font_style = GLUT_BITMAP_HELVETICA_18;

void drawstr(GLuint x, GLuint y, const char* format, int length)
{

  glRasterPos2i(x, y);  
  for(int i=0; i<length; ++i)
    glutBitmapCharacter(font_style, *(format+i) );
}

void init(void)
{    
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   unsigned char* image = SOIL_load_image( "./res/splash-bg-alt.jpg", &ww, &wh, 0, SOIL_LOAD_RGB );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, ww, wh, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
   SOIL_free_image_data( image );
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   glBindTexture(GL_TEXTURE_2D, texName);
   glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0); glVertex3f(0,0,0);
      glTexCoord2f(1.0, 0.0); glVertex3f(1920,0,0);
      glTexCoord2f(1.0, -1.0); glVertex3f(1920,1080,0);                        
      glTexCoord2f(0.0, -1.0); glVertex3f(0,1080,0);
   glEnd();
   drawstr(940,700,"drone-x",7);
   glFlush();
   glDisable(GL_TEXTURE_2D);
}

void reshape(int w, int h)
{ 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glOrtho(0,1920,0,1080,-1,1);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard (unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(ww, wh);
   glutCreateWindow("drone-x");
   glutFullScreen();
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0; 
}