#include <bits/stdc++.h>
using namespace std;
#define nl cout<<"\n";
#define WHITE 0
#define GRAY 1
#define BLACK 2
int *color, *dist, *start, *finish;
map<int, vector<int> > adjlist;
int vt=0;
size_t n=0 , e=0;
pair< int , int> *coordinates;

void BFS(int src)
{
	vector<int> neighbours = adjlist[src];
	cout<<"exploring neighbors of "<<src; nl
	queue<int> q;
	for( int i : neighbours)
	{
		if(color[i] == WHITE)
		{
			color[i]=GRAY;
			cout<<"greying node "<<i; nl
			display();
			q.push(i);
		}
	}
	while(!q.empty())
	{
		BFS(q.front());
		q.pop();		
	}
	cout<<"blacking. completely visited "<<src; nl
	color[src]= BLACK;
	display();
}

void drawCircle(int color,pair <int , int> center)
{
	int xCenter=center.first, yCenter=center.second, r=30;
	int x=0,y=r;
	int p = 3/2 - r;
	glColor3f( 0 ,0, 0);
	while(x<=y)
	{
		setPixel(xCenter+x,yCenter+y);
		setPixel(xCenter+y,yCenter+x);
		setPixel(xCenter-x,yCenter+y);
		setPixel(xCenter+y,yCenter-x);
		setPixel(xCenter-x,yCenter-y);
		setPixel(xCenter-y,yCenter-x);
		setPixel(xCenter+x,yCenter-y);
		setPixel(xCenter-y,yCenter+x);
		if (p<0)
			p += (2*x)+3;
		else 
		{ 
			p += (2*(x-y))+5;
			y -= 1;
		}
		x++;
	}
	if(color== WHITE)
	{
		fillColt[0]= 255;
		fillColt[1]= 255;
		fillColt[2]= 255;
	}
	if(color == GRAY)
	{
		fillColt[0]= 128;
		fillColt[1]= 128;
		fillColt[2]= 128;
	}
	if(color == BLACK)
	{
		fillColt[0]= 0;
		fillColt[1]= 0;
		fillColt[2]= 0;
	}
	glColor3ubv(strcol);
	drawstr(xCenter-5,yCenter-5, str.c_str(), str.length());
	boundaryFill4(xCenter+15,yCenter+15,fillColt,borderCol); 
}
void drawEdge(pair<int int> p, pair<int int> q)
{
    int x1,x2,y1,y2;
    x1= p.first;
    x2= q.first;
    y1= p.second;
    y2= q.second;

    int dx, dy, x, y, d, s1, s2, swap=0, temp;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    s1 = sign(x2-x1);
    s2 = sign(y2-y1);

    if(dy > dx)
    {
        temp = dx;
        dx = dy; 
        dy = temp;
        swap = 1;
    }

    d = 2 * dy - dx;
    x = x1;
    y = y1;

    int i;

    for(i = 1; i < dx; i++)
    {
        setPixel(x,y);

        while(d>=0)
        {
            d=d-2*dx;
            if(swap)
                x+=s1;
            else
                y+=s2;
        }

        d=d+2*dy;
        if(swap)
            y+=s2;
        else
            x+=s1;
    }
}

void mouse(int button, int state, int x, int y)  
{  
    if(vt!=N)
    {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)   
        { 
            coordinates[vt].first = x;
            coordinates[vt].second = (700-y);
            cout<< coordinates[vt].first <<'\t'<<  coordinates[vt].second <<'\n';
            vt++;
            display();
        }
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)   
    {
        cout<<"\nRunning BFS\n";
        for(int i=0;i<n;i++)
		{
			if(color[i]==WHITE)
			{
				cout<<"greying node "<<i; nl
				color[i]=GRAY;
				display();
				BFS(i);
			}
		}
    }
}
void display()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f, 0.8f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i=0;i<vt;i++)
    {
        str=to_string(i);
        drawCircle(color[i],coordinates[i]);
    }
    if(vt==n)
    {
        for(int i=0;i<n;i++)
        {
        	vector<int> neighbours = adjlist[i];
        	for( int j : neighbours)
        	{
        		drawEdge(coordinates[i],coordinates[j]);
        	}
        }
    }
    glutSwapBuffers();
}

void handleResize(int w, int h)
{
    glMatrixMode(GL_PROJECTION);   // Transformations being applied to Projection Matrix
    glLoadIdentity();   // Always call after changing matrix
    glViewport(0,0,w,h);
    gluOrtho2D(0.0,700.0,0.0,700.0); // Here object is being moved along with window, wx & hy window size. Here clipping range is zNear=1,zFar=-200
    glMatrixMode(GL_MODELVIEW); // Transformations on ModelView Matrix(default)
}

int main()
{
	cout<<"Enter the number of nodes in the graph"; nl
	cin>>n;
	color =  new int[n];
	coordinates= new pair< int ,int > [n];
	memset(color, WHITE,n*sizeof(int));
	cout<<"Enter the number of edges in the graph"; nl
	cin>>e;
	cout<<"Enter the edges of the graph (u --> v) pairs"; nl
	size_t u,v;
	for(int i=0;i<e;i++)
	{
		cin>>u>>v;
		adjlist[u].push_back(v);
	}
	/**/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Breadth First Search");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutReshapeFunc(handleResize);
	glutMainLoop();
	return 0;
}
