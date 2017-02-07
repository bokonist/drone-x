#include <bits/stdc++.h>
#include <GL/glut.h>
#include <unistd.h>
using namespace std;
#define sign(x) ((x>0)?1:((x<0)?-1:0))

struct lis
{
  int data;
  lis *next;
};
class vertex
{
  public:
  int data,x,y;
  float color;
};


vertex V[20],Q[20];

int front=0;
int rear=0;
lis *adj[20], *node, *temp;

int N,wx,hy;
int flag=0;
int vt=0;

string str;
GLvoid *font_style = GLUT_BITMAP_HELVETICA_18;

void display();

void drawstr(GLuint x, GLuint y, const char* format, int length)
{
    glRasterPos2i(x, y);  
    for(int i=0; i<length; ++i)
    {
        glutBitmapCharacter(font_style, *(format+i) );
    }
}


float pixels[3];

GLubyte fillCol[3] = {255,255,255};  
GLubyte borderCol[3] = {0,0,0};
GLubyte fillColt[3];
GLubyte strcol[3]={230,100,80};

void getPixel(int x, int y, GLubyte pixels[3])  
{  
    glReadPixels(x,y,1.0,1.0,GL_RGB,GL_UNSIGNED_BYTE,pixels);  
} 

void setPixel(GLint x, GLint y)
{
  glBegin(GL_POINTS);
   glColor3f(0,0,0);
   glVertex2i(x,y);
  glEnd();
}

void setPixelcol(int pointx, int pointy, GLubyte f[3])  
{  
       glBegin(GL_POINTS);  
           glColor3ubv(f);  
           glVertex2i(pointx,pointy);  
       glEnd();    
} 

void boundaryFill4(int x,int y,GLubyte fillColor[3],GLubyte borderColor[3])  
{  
    GLubyte interiorColor[3]; 
    GLubyte strintcol[3];   
    getPixel(x,y,interiorColor);  
    if(interiorColor[0]==strcol[0])
       {
          getPixel(x+1,y+1,strintcol);
          if(strintcol[0]!=fillColor[0])
          {
            boundaryFill4(x+1,y,fillColor,borderColor); 
          }
       }
    if((interiorColor[0]!=borderColor[0] && (interiorColor[1])!=borderColor[1] && (interiorColor[2])!=borderColor[2]) && (interiorColor[0]!=fillColor[0] && (interiorColor[1])!=fillColor[1] && (interiorColor[2])!=fillColor[2]))  
    {  
            setPixelcol(x,y,fillColor);
            if((interiorColor[0]!=strcol[0]) && (interiorColor[1]!=strcol[1]) && (interiorColor[2]!=strcol[2]) )
            { 
             boundaryFill4(x+1,y,fillColor,borderColor);  
             boundaryFill4(x-1,y,fillColor,borderColor);  
             boundaryFill4(x,y+1,fillColor,borderColor);  
             boundaryFill4(x,y-1,fillColor,borderColor);  
           } 
    }  
} 


void drawCircle(float c,int x1,int y1)
{
  int xCenter=x1,yCenter=y1,r=30;
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

 if(c==1)
  {
    fillColt[0]= 255;
    fillColt[1]= 255;
    fillColt[2]= 255;
 }
 if(c==0.5)
  {
    fillColt[0]= 128;
    fillColt[1]= 128;
    fillColt[2]= 128;
 }
  if(c==0)
  {
    fillColt[0]= 0;
    fillColt[1]= 0;
    fillColt[2]= 0;
 }

 glColor3ubv(strcol);
 drawstr(xCenter-5,yCenter-5, str.c_str(), str.length());
 boundaryFill4(xCenter+15,yCenter+15,fillColt,borderCol);
  
}



void drawEdge(int xt,int yt)
{
    int x1,x2,y1,y2;
    x1=V[xt].x;
    x2=V[yt].x;
    y1=V[xt].y;
    y2=V[yt].y;

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


void BFS()
{
  for(int i=0;i<N;i++)
   {
    
   if(V[i].color==1)
    {
     V[i].color=0.5;
     display();
     sleep(1);
     cout<<V[i].data<<" is grey \n";
     Q[rear++]=V[i];
     vertex vtemp;
     lis *temp;
     while(front!=rear)
      {
        vtemp=Q[front++];
        temp=adj[vtemp.data-1]->next;
        while(temp!=NULL)
        {
          if(V[temp->data-1].color==1)
          {
            V[temp->data-1].color=0.5;
            display();
            sleep(1);
            cout<<V[temp->data-1].data<<" is grey\n";
            Q[rear++]=V[temp->data-1]; 
          } 
          temp=temp->next;
        }
       V[vtemp.data-1].color=0;
       display();
       sleep(1); 
       cout<<V[vtemp.data-1].data<<" is black\n";
      }
    }
  }
}



void mouse(int button, int state, int x, int y)  
{  
    if(vt!=N)
    {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)   
        { 
            V[vt].x=x;
            V[vt].y= (700-y);
            cout<<V[vt].x<<'\t'<<V[vt].y<<'\n';
            vt++;
            display();
        }
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)   
    {
        cout<<"\nRunning BFS\n";
        BFS();
    }
}

void display()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f, 0.8f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i=0;i<vt;i++)
    {
        str=to_string(V[i].data);
        drawCircle(V[i].color,V[i].x,V[i].y);
    }
    if(vt==N)
    {
        for(int i=0;i<N;i++)
        {
            temp=adj[i]->next;
            while(temp!=NULL)
            {
                drawEdge(i,temp->data-1);
                temp=temp->next;
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


int main(int argc,char *argv[])
{
    int dis,j,posx,posy,e1,e2;
    wx=700,hy=700;

    cout<<"Enter the no of vertices: ";
    cin>>N;

    for(int i=0;i<N;i++)
    { 
        V[i].data=i+1;
        V[i].color=1;
        adj[i]=new lis;
        adj[i]->data=0;
        adj[i]->next=NULL;
        cout<<"\nEnter the vertices "<<i+1<<" is linked to: ";
        cin>>j;
        if(j!=0)
        {
            temp=adj[i];
            while(j!=0)
            {
                node=new lis;
                node->data=j;
                node->next=NULL;
                cin>>j;
                temp->next=node;
                temp=temp->next;
            }
        }
    }


/*    cout<<"\nAdjacency List: \n";
    for(int i=0;i<N;i++)
    {
        cout<<"\n"<<i+1<<": ";
        temp=adj[i]->next;
        while(temp!=NULL)
        {
            cout<<temp->data<<" ";
            temp=temp->next;
        }
        cout<<"\n";
    }*/

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("BFS");
    glutMouseFunc(mouse);
    glutDisplayFunc(display);
    glutReshapeFunc(handleResize);
    glutMainLoop();
}
