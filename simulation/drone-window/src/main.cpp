#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/drone.h"
#include "neat.h"
#include "population.h"
#include "../include/drone-evaluation.h"

double queuedMilliseconds,prev0,fps,responseTime,score;
int resX,resY,dronePhy,seedValue,menu;
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

void mouseHandle(int btn,int state,int x,int y)
{
   int y1=resY-y;
   if(btn==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
   {
      if(x>=454 && x<=820 && y1>=81 && y1<=112)
      menu=1;
   }
}

double syncDrone;

void initNeat()
{
    NEAT::load_neat_params("neat_settings.ne",true);
}

int main(int argc, char** argv)
{
   if(argc>=2)
      seedValue=atoi(argv[1]);
   else
      seedValue=53; // Default seedvalue
   dronePhy=4;
   //..frame-update setup
   queuedMilliseconds=0;
   prev0=0;
   fps=60;
   //fps=180;
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
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouseHandle);
   glutMainLoop();
   return 0; 
}
