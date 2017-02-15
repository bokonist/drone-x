#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include<myheader.h>

double queuedMilliseconds,prev0,fps,responseTime;


void reshape(int w, int h)
{
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glOrtho(0,1920,0,1080,1,10);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   //..gameLoop setup
   queuedMilliseconds=0;
   prev0=0;
   fps=60;
   responseTime=(1/fps)*1000;

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  //glutInitWindowSize(1920, 1080);
   //glutInitWindowPosition(100, 100);
   glutCreateWindow("drone-x");
   glutFullScreen();
   init();
   glutDisplayFunc(draw);
   glutIdleFunc(mixedStepLoop);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(processSpecialKeys);
   glutMainLoop();
   return 0; 
}

