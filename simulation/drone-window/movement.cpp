#include <GL/glut.h>
#include <drone.h>

void processSpecialKeys(int key, int xx, int yy)
{
	//glutIgnoreKeyRepeat(GLUT_KEY_DOWN);
	int speed=40;
	switch (key) 
	{
		case GLUT_KEY_DOWN :
			for (int i = 0; i < speed; ++i)
				movementY--;
			//movementY-=speed;
			break;
		case GLUT_KEY_UP :
			for (int i = 0; i < speed; ++i)
				movementY++;
			break;
		case GLUT_KEY_RIGHT :
			for (int i = 0; i < speed; ++i)
				movementX++;
			break;
		case GLUT_KEY_LEFT :
			for (int i = 0; i < speed; ++i)
				movementX--;
			break;
	}
}