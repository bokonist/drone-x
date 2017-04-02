#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/drone.h"
#include "neat.h"
#include "population.h"
#include "../include/drone-evaluation.h"

double queuedMilliseconds,prev0,fps,responseTime,score;
int resX,resY,dronePhy;
vector<obstacle> obstacleList;
vector<char> inputKey; // list of moves to be actuated

void reshape(int w, int h)
{
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glOrtho(0,resX,0,resY,-5,10);
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

double syncDrone;

void initNeat()
{
    NEAT::load_neat_params("neat_settings.ne",true);
}

int main(int argc, char** argv)
{
   dronePhy=4;
   //..frame-update setup
   queuedMilliseconds=0;
   prev0=0;
   fps=60;
   responseTime=(1/fps)*1000;

   //..Rendering Resolution
   resX=854;resY=480;

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   
   //.... Window rendering
   glutInitWindowSize(1280, 720);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("drone-x");
   //glutFullScreen();
   
   //.... Game mode rendering
   /*glutGameModeString("1920x1080");
   if(glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
   {
      glutEnterGameMode();
   }
   else 
   {
      printf("The select resolution mode is not available, change GameModeString to correct resolution\n");
      exit(1);
   }*/
   initTextures();  // texture initialisation
   initNeat();
   glutIgnoreKeyRepeat(1);  // keyboard repeat off
   glutDisplayFunc(draw); 
   //glutIdleFunc(mixedStepLoop);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   //glutSpecialFunc(processSpecialKeys);
   glutMainLoop();
   return 0; 
}
