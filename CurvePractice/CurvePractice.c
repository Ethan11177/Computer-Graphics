#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define TAM  500
#define TAM2 40000000
#define numerox 1920  
#define numeroy 1080 

int mat[numerox][numeroy] = {0};

typedef struct{

    int x;
    int y;

}cordinates;

void principal();
void lineasdda(double xi, double xf, double yi, double yf);
void curvegenerator();
void curvegenerator_2(cordinates* a, int n);
void Draw_Segment_Console();
void Draw_Line_Bresenham(int x1,int y1,int x2,int y2);

int main(){

    principal();

}

void principal(){

    curvegenerator();        
    Draw_Segment_Console();
}

void curvegenerator(){

    cordinates *cores = ((cordinates*)malloc(sizeof(cordinates)*4));

   srand(time(NULL));

   int n = 0;

    for (int i = 0; i < 4; i++)
    {


        //if((rand()%(2))==1)
            cores[i].x = rand() % 10;
        //else
            //cores[i].x = (-1)*(rand() % 10);

        //if((rand()%(2))==1)
            cores[i].y = rand() % 10;
        //else
            //cores[i].y = (-1)*(rand() % 10);

        printf("%d, %d\n", cores[i].x, cores[i].y);
    }

    n = rand()%(1+99);

    printf("%d\n\n", n);

    curvegenerator_2(cores, n);

}

void curvegenerator_2(cordinates *a, int n){

    double t = 1, p = 0;
    double x2 = 0, y2 = 0;
    double x1 = a[0].x, y1 = a[0].y;  

    t = t/n;
    p = t;

    for (int i = 0; i < n; i++)
    {
        x2 = (a[0].x*pow(t,3)) + (a[1].x*pow(t,2)) + (a[2].x*pow(t,1)) + (a[3].x*pow(t,0));
        y2 = (a[0].y*pow(t,3)) + (a[1].y*pow(t,2)) + (a[2].y*pow(t,1)) + (a[3].y*pow(t,0));
        printf("%1f\n", t);
        printf("%f,%f-%f,%f\n\n",x1,y1,x2,y2);
        Draw_Line_Bresenham(x1*5, y1*5, x2*5, y2*5);
        x1 = x2;
        y1 = y2;
        t = p * i;
    }
    

}


void Draw_Segment_Console()
{
  for(int i=0;i<100;i++)
  {
    for(int j=0;j<100;j++)
    {
      printf("%d",mat[i][j]);
    }
    printf("\n");
  }
}


void Draw_Line_Bresenham(int x1,int y1,int x2,int y2)
{
  int x,y,dx,dy,dx1,dy1,px,py,xe,ye;
  dx=x2-x1;
  dy=y2-y1;
  dx1=fabs(dx);
  dy1=fabs(dy);
  px=2*dy1-dx1;
  py=2*dx1-dy1;
  if(dy1<=dx1)
  {
    if(dx>=0)
    {
    x=x1;
    y=y1;
    xe=x2;
    }
    else
    {
    x=x2;
    y=y2;
    xe=x1;
    }
    mat[x][y]=1;
    for(int i=0;x<xe;i++)
    {
    x=x+1;
    if(px<0)
    {
      px=px+2*dy1;
    }
    else
    {
      if((dx<0 && dy<0) || (dx>0 && dy>0))
      {
      y=y+1;
      }
      else
      {
      y=y-1;
      }
      px=px+2*(dy1-dx1);
    }
    mat[x][y]=1;
    }
  }
  else
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
    mat[x][y]=1;
    for(int i=0;y<ye;i++)
    {
    y=y+1;
    if(py<=0)
    {
      py=py+2*dx1;
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
      py=py+2*(dx1-dy1);
    }
    mat[x][y]=1;
    }
  }
}