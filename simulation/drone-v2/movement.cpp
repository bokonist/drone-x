#include <GL/glut.h>
#include <vector>
#include <drone.h>
using namespace std;

vector<char> inputKey; 
int cntU,cntD;
void movePhysics()
{
	int ii;
	for(ii=0;ii<inputKey.size();ii++)	
	{
		// ...UP KEY ANIMATION  7*4=28
		if(inputKey[ii]=='U')
		{
			if( movementY <= ( resY-( (resY/2) + (resY*0.20) ) ) )  //Up Boundary check
			movementY+=0.0047*resX;
			cntU++;
		}					
		if(cntU==7)
		{	
			cntU=0;	
			inputKey.erase(inputKey.begin()+ii);
		}	
		if(cntU>0)
			break;
		
		//...DOWN KEY ANIMATION  7*4=28
		if(inputKey[ii]=='D')
		{
			if( movementY >= -( resY/2 - (resY*9)/100 ) ) // Down boundary check
			movementY-=0.0047*resX;
			cntD++;
		}	
		if(cntD==7)
		{
			cntD=0;
			inputKey.erase(inputKey.begin()+ii);
		}
		if(cntD>0)
			break;
	}
}

void processSpecialKeys(int key, int xx, int yy)
{

	switch (key) 
	{
		case GLUT_KEY_DOWN :
				//if(movementY!=-440)
				inputKey.push_back('D');
			break;
		case GLUT_KEY_UP :
				//if(movementY!=440)
				inputKey.push_back('U');
			break;
	}
}
