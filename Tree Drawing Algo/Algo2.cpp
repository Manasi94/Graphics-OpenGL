#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/glut.h>
#include<unistd.h>

struct tree{
	int no_sons;
	int height;
	int x,y;
	int father;
	int status;
	int sons[10];
	int end;
};

//Initialize
void init(void) 
{ 	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
       //set display-window background color to white 
       glClearColor(1.0,1.0,1.0,0.0); //set projection paramaters //set clipping area
       glMatrixMode(GL_PROJECTION); //projection matrix for operation
       glLoadIdentity();    //reset projection matrix
       gluOrtho2D(0,640,0,640); //set clipping area's left,right //clipping area is mapped to viewport 
	                            //glViewport(0,0,640,480);//screen coordinates xTopLeft,xTopright,width,height
} 

void putpixel(int xC, int yC)
{ 
       glBegin(GL_POINTS); 
              glVertex2i(xC,yC); 
       glEnd(); 
       glFlush(); //executes all OpenGL functions as quickly as possible 
} 

// Plot using circle's 8 way symmetry
void plot_point(int x, int y,int xc,int yc)
{
    glBegin(GL_POINTS);
	glVertex2i(xc+x, yc+y);
	glVertex2i(xc+x, yc-y);
	glVertex2i(xc+y, yc+x);
	glVertex2i(xc+y, yc-x);
	glVertex2i(xc-x, yc-y);
	glVertex2i(xc-y, yc-x);
	glVertex2i(xc-x, yc+y);
	glVertex2i(xc-y, yc+x);
    glEnd();
}

// midpoint circle drawing algorithm
void midpoint_circle(int xc,int yc,int r)
{
	int x=0,y=r;
	float pk=1-r;
	int deltaE=3;
	int deltaSE=-2*r+5;
 
	plot_point(x,y,xc,yc);
	while(y>x)
	{
		if(pk<0)
		{
			pk+=deltaE;
			deltaE+=2;
			deltaSE+=2;
		}
		else
		{
			pk+=deltaSE;
			deltaE+=2;
			deltaSE+=4;
			y--;
		}
		x++;
		plot_point(x,y,xc,yc);
	}
  glFlush();
}

//Line drawing function
void line(int x1,int y1,int x2,int y2)
{
	int x,y,dx,dy,delx,dely,px,py,xe,ye,i,deltaE,deltaNE,deltaN,deltaEN;
	dx=x2-x1;
	dy=y2-y1;
	delx=fabs(dx);
	dely=fabs(dy);
	px=2*dely-delx;
	deltaE=2*dely;
	deltaNE=2*(dely-delx);

	py=2*delx-dely;
	deltaN=2*delx;
	deltaEN=2*(delx-dely);
	if(dely<=delx)   //modulus of m <=1
	{
		if(dx>=0)   //for selection of endpoint which have positive slope
		{
			x=x1;
			y=y1;
			xe=x2;
		}
		else                //negative slope
		{
			x=x2;
			y=y2;
			xe=x1;
		}
		putpixel(x,y);
		for(i=0;x<xe;i++)
		{
			x=x+1;
			if(px<0)
			{
				px+=deltaE;
			}
			else
			{
				if((dx<0 && dy<0) || (dx>0 && dy>0))  //positive slope with (a to b) or (b to a) and incrementing y 
				{
					y=y+1;
				}
				else      //negative slope 
				{
					y=y-1;
				}
			px+=deltaNE;
			}
			
			putpixel(x,y);
		}	
	}

	else        //modulus of m >1  //dx is smaller than dy
	{
		if(dy>=0)   
		{
			x=x1;
			y=y1;
			ye=y2;
		}
		else
		{
			x=x2;
			y=y2;
			ye=y1;
		}
		putpixel(x,y);
		for(i=0;y<ye;i++)
		{
			y=y+1;
			if(py<=0)
			{
				py+=deltaN;
			}
			else
			{
				if((dx<0 && dy<0) || (dx>0 && dy>0))  
				{
					x=x+1;
				}
				else
				{
					x=x-1;
				}
			py+=deltaEN;
			}
		
		putpixel(x,y);
		}
	}
}


//function for naive tree drawing algo 
void tree(void) 
{  
    glColor3f(1.0,0.0,0.0); 
	glPointSize(2.0);
	int root,n,i,max_height,j;
	printf("Enter the no of nodes : ");
	scanf("%d",&n);
	printf("\nEnter the maximum height : ");
	scanf("%d",&max_height);

	struct tree t[n+1];
	for(i=0;i<n;i++)
	{	
		printf("enter details of %dth node father, number of sons, height: ",i);
		scanf("%d %d %d",&t[i].father,&t[i].no_sons,&t[i].height);
		if(t[i].no_sons > 0)
		{
            printf("Enter sons: ");
            for(j=1;j<=t[i].no_sons;j++)
			{
		    	scanf("%d",&(t[i].sons[j]));
            }
        }
    	t[i].sons[0] = 1000;
        t[i].x = 0;
        t[i].y = 0;
		t[i].end=0;
	}
	
	t[i].end = -1000;
	for(i=0;i<n;i++)
	{
	    printf("Node:%d\tFather:%d\tNo_Sons:%d\tX:%d\tY:%d\n",i,t[i].father,t[i].no_sons,t[i].x,t[i].y);
	}

	int next_x[max_height+1];
	root = 0;
	t[0].status = 0;
	int current = root;
	for(i=0;i<=max_height;i++)
	{
		next_x[i] = 1;
	}

	while(t[current].end != -1000)
	{
	    if(t[current].status == 0)
		{
			t[current].x = next_x[t[current].height];
			t[current].y = (2*(t[current].height)) + 1;
			next_x[t[current].height] = next_x[t[current].height] + 2;
			for(i=1;i<=t[current].no_sons;i++)
			{
                t[(t[current].sons[i])].status = 0;
			}
			t[current].status = 1;
		}
		else if((t[current].status >= 1) && (t[current].status <= t[current].no_sons))
		{
			t[current].status = t[current].status + 1;
			current = t[current].sons[(t[current].status - 1)];
		}
		else
		{
			current = t[current].father;
			if(current == -1)
			{
                break;
			}
		}
	}

	for(i=0;i<n;i++)
	{
    	printf("Draw Circle at : %d %d\n",t[i].x,t[i].y);
        midpoint_circle(t[i].x*30+100,640-t[i].y*30,10);
        sleep(2);// wait for 2 units
        for(j=1;j<=t[i].no_sons;j++)
        {
        	printf("Draw Line bw : father %d %d and son %d %d\n",t[i].x,t[i].y,t[t[i].sons[j]].x,t[t[i].sons[j]].y);
        	line(t[i].x*30+100,640-t[i].y*30,t[t[i].sons[j]].x*30+100,640-t[t[i].sons[j]].y*30);
     	   sleep(2);
    	}	
	}
	glFlush();
} 

///Main function
int main(int argc,char **argv)
{
	//initialize GLUT 
    glutInit(&argc,argv); //display mode initialized 
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //display-window W, H set
    glutInitWindowSize(640,640); //set display-window upper-left position 
    glutInitWindowPosition(50,50); //create display-window
    glutCreateWindow("Algo2"); 
    init();//initialze OpenGL //call graphics to be displayed on the window 
    glutDisplayFunc(tree); //call the tree drawing function 
    glutMainLoop(); 
	exit(0);
	return 0;
}



