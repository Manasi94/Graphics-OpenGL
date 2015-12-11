
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/glut.h>
#include<unistd.h>

struct tree{
    int left_son; // 1 = true , 0 = false
    int right_son;
    int left_son_index;
    int right_son_index;
	  int height;
	  int x,y;
	  int father;
	  int modifier_t;
	  int status; // first visit = 0 , left visit = 1 , right visit = 2
	  int end;
};

void init(void) 
{ 	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //set display-window background color to white 
    glClearColor(1.0,1.0,1.0,0.0); 
    //set projection paramaters 
    //set clipping area
    glMatrixMode(GL_PROJECTION); //projection matrix for operation
    glLoadIdentity();    //reset projection matrix
    gluOrtho2D(0,640,0,640); //set clipping area's left,right,bottom,top
    //clipping area is mapped to viewport
    //glViewport(0,0,640,480);//screen coordinates xTopLeft,xTopright,width,height
} 

void putpixel(int xC, int yC)
{ 
    glBegin(GL_POINTS); 
    glVertex2i(xC,yC); 
    glEnd(); 
    glFlush(); //executes all OpenGL functions as quickly as possible 
} 
//int xc=100,yc=100;//xc = 320, yc = 240;

// Plot points using circle's 8 part symmetrical property
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

// Bressenham's line drawing algorithm
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
  if(dely<=delx)   //|m|<=1
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

  else        //|m|>1  //dx is smaller than dy
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

// bresenham's circle drawing algorithm
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

//Function for drawing tree
void tree(void) 
{ 
  glColor3f(1.0,0.0,0.0); 
	glPointSize(2.0);
	//Var
  //Variable declaration
  int i,j,n,h,place,root,modifier_sum,max_height,current;
  bool is_leaf;
  int fl = 0;
  int first_visit=0,left_visit=1,right_visit=2;
  //Input
  
  //Input a well informed binary tree 
  printf("Enter the no of nodes : ");
	scanf("%d",&n);//total number of nodes
  //Input maximum height of the tree
	printf("\nEnter the maximum height : ");
	scanf("%d",&max_height);
  struct tree t[n+1];
  //loop for details of each node
	for(i=0;i<n;i++)
	{	printf("enter details of %dth node father, left son, right son and height: ",i);
		scanf("%d %d %d %d",&t[i].father,&t[i].left_son,&t[i].right_son,&t[i].height);
		if(t[i].left_son == 1)
		{
            		printf("Enter left son: ");
                	scanf("%d",(&t[i].left_son_index));
            	}
        	if(t[i].right_son == 1)
        	{
            		printf("Enter right son: ");
                	scanf("%d",(&t[i].right_son_index));
            	}

        	t[i].x = 0;
        	t[i].y = 0;
		t[i].end=0;
	}

	t[i].end = -1000;

   //begin
	 int modifier[max_height+1];
	 int next_pos[max_height+1];

	 for(i=0;i<=max_height;i++)
	 {
   	next_pos[i] = 1;
   	modifier[i] = 0;
   }
    
    root = 0;
    t[root].status = first_visit;
    current = root;
    modifier_sum = 0;
    while(t[current].end != -1000)
    {
     	switch(t[current].status)
       	{
       	 case 0:
         //modifier_sum = modifier_sum + t[current].modifier;
         	t[current].status = left_visit; 
         	if(t[current].left_son)
         	{
              current = t[current].left_son_index;
              t[current].status = first_visit;
          }
          //printf("current %d status %d",current,t[current].status);
          break;
          case 1:
           	t[current].status = right_visit;
           	if(t[current].right_son)
           	{
           		current = t[current].right_son_index;
           		t[current].status = first_visit;
           	}
                 //printf("current %d status %d",current,t[current].status);
          break;
          case 2:
            h = t[current].height;
           	is_leaf = ((!(t[current].left_son)) && (!(t[current].right_son)));
           	if(is_leaf)
           	{
           		place = next_pos[h];
            }
           	else if(!(t[current].left_son))
           	{
           		place = t[t[current].right_son_index].x - 1;
            }
            else if(!(t[current].right_son))
            {
           		place = t[t[current].left_son_index].x + 1;
            }
             	else
             	{
            	 place = ((t[t[current].left_son_index].x) + (t[t[current].right_son_index].x) ) / 2;
              }
             	modifier[h] = modifier[h] > (next_pos[h] - place) ? modifier[h] : (next_pos[h] - place);
             	if(is_leaf)
             	{
             		t[current].x = place;
             	}
             	else
             	{
             	 t[current].x = place + modifier[h];
             	}
             	next_pos[h] = t[current].x + 2;
             	t[current].modifier_t = modifier[h];
             	current = t[current].father;
             	if(current == -1)
             	{
             		fl = 1;
              }
               // printf("current %d status %d",current,t[current].status);
            break;
        	}
        	if(fl == 1)
        	{
            		break;
       		}
    	}//while ends

  
	for(i=0;i<n;i++)
	{
		printf("current %d end %d ",i,t[i].end);
	}	
  root = 0;
	t[root].status = first_visit;
	current = root;
	modifier_sum = 0;
	fl=0;
	while(t[current].end != -1000)
	{
   	switch(t[current].status)
   	{
   		case 0:
        t[current].x = t[current].x + modifier_sum;
        modifier_sum = modifier_sum + t[current].modifier_t;
        t[current].y = (2*(t[current].height)) + 1;
        printf("Current : %d Y : %d\n",current,t[current].y);
        t[current].status = left_visit;
        if(t[current].left_son)
        {
        	printf("line 128");
        	current = t[current].left_son_index;
        	t[current].status = first_visit;
        }
      //       printf("current %d status %d",current,t[current].status);
      break;
      case 1:
      	t[current].status = right_visit;
      	if(t[current].right_son)
      	{
      		current = t[current].right_son_index;
      		t[current].status = first_visit;
      	}
      //     printf("current %d status %d",current,t[current].status);
      break;
      case 2:
      	modifier_sum = modifier_sum - t[current].modifier_t;
      	current = t[current].father;
      	if(current == -1)
      	{
      		fl = 1;
      	}
      //   printf("current %d status %d",current,t[current].status);
      break;
    }	
    if(fl == 1)
    {
    	break;
  	}
  }//while ends

  for(i=0;i<n;i++)
  {
    printf("%d\t%d\n",t[i].x,t[i].y);
    printf("Draw Circle at : %d %d\n",t[i].x,t[i].y);
    midpoint_circle(t[i].x*30+150,640-t[i].y*30,10);
    sleep(2);
    if(t[i].left_son)
    {
      printf("Draw Line bw : father %d %d and left son %d %d\n",t[i].x,t[i].y,t[t[i].left_son_index].x,t[t[i].left_son_index].y);
      line(t[i].x*30+150,640-t[i].y*30,t[t[i].left_son_index].x*30+150,640-t[t[i].left_son_index].y*30);
      sleep(2);
    }
    if(t[i].right_son)
    {
      printf("Draw Line bw : father %d %d and right son %d %d\n",t[i].x,t[i].y,t[t[i].right_son_index].x,t[t[i].right_son_index].y);
      line(t[i].x*30+150,640-t[i].y*30,t[t[i].right_son_index].x*30+150,640-t[t[i].right_son_index].y*30);
      sleep(2);//Pause for 2 units of time
    }
  }
glFlush();
} 

//Main function
int main(int argc,char **argv)
{
	//GLUT initialize 
  glutInit(&argc,argv); 
  //display mode initialize
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
  //display-window size width & height setting
  glutInitWindowSize(640,640); 
  //display-window upper-left position setting
  glutInitWindowPosition(50,50); 
  //create display-window with a title 
  glutCreateWindow("Algo 3"); 
  //initialze OpenGL 
  init(); 
  //function tree displayed on the window 
  glutDisplayFunc(tree); 
  //display  
  glutMainLoop(); 
	exit(0);
  return 0;
}