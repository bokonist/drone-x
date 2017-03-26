#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <drone.h>

double queuedMilliseconds,prev0,fps,responseTime,score;
int resX,resY,dronePhy;

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
void mixedStepLoop()
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
    score+=(timeElapsedMs/1000);
    prev0=now;
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
   //glutInitWindowSize(1280, 720);
   //glutInitWindowPosition(100, 100);
   //glutCreateWindow("drone-x");
   //glutFullScreen();
   
   //.... Game mode rendering
   glutGameModeString("1280x720");
   if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
   {
     glutEnterGameMode();
   }
   else 
   {
    printf("The select resolution mode is not available, change GameModeString \n");
    exit(1);
   }
  /*// glutInitWindowSize(1280, 720);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("drone-x");
   glutFullScreen();*/
   initTextures();  // texture initialisation
   glutIgnoreKeyRepeat(1);  // keyboard repeat off
   glutDisplayFunc(draw); //in draw.cpp
   glutIdleFunc(mixedStepLoop);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(processSpecialKeys); //in movement.cpp
   glutMainLoop();
   return 0; 
}
