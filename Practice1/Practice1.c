//Name: Vaquera Aguilera Ethan Emiliano
//Group: 3CV14
//School. ESCOM
//Teacher: SAUCEDO DELGADO RAFAEL NORMAN

#include <stdio.h>
#include <stdlib.h>
#include <math.h>



int main(int argc, char *argv[]) {


	
	int x1=2, y1=6, x2=12, y2=2;
   	float ax,ay,x,y,luz;
   	int i;

   	if(abs(x2-x1)>=abs(y2-y1))
   	luz=abs(x2-x1);
   	else
   	luz=abs(y2-y1);
   	
	ax=(x2-x1)/luz;
   	ay=(y2-y1)/luz;



   	x=(float)x1;
   	y=(float)y1;



   	i=1;

   	while(i<=luz)
   	{


        x=x+ax;
        printf("%f,", round(x));
        y=y+ay;
        printf("%f\n", round(y));
        i=i+1;
   	}
	return 0;
}
