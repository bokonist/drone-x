#include<GL/glut.h>
#include<myheader.h>

void processSpecialKeys(int key, int xx, int yy)
{
switch (key) {
case GLUT_KEY_DOWN :
movementY-=10;
break;
case GLUT_KEY_UP :
movementY+=10;
break;
case GLUT_KEY_RIGHT :
movementX+=10;
break;
case GLUT_KEY_LEFT :
movementX-=10;
break;

}

}

