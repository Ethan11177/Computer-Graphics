#include<math.h>
#include<stdlib.h>
#include<stdio.h>

#define imageWidth 1920
#define imageHeight 1920
int mat[imageWidth][imageHeight] = {0};
void Store_ppm();
void main()
{
int x[4],y[4],i,put_x,put_y;
printf("\n****** Bezier Curve ***********");
printf("\n Please enter x and y coordinates ");
for(i=0;i<4;i++)                 
{
scanf("%d,%d",&x[i],&y[i]);
put_x=x[i];
put_y=y[i];
mat[put_x][put_y]=1;               // Control Points
}

for(int t=0.0;t<=1.0;t=t+0.1)             
{
put_x = pow(1-t,3)*x[0] + 3*t*pow(1-t,2)*x[1] + 3*t*t*(1-t)*x[2] + pow(t,3)*x[3]; 
put_y =  pow(1-t,3)*y[0] + 3*t*pow(1-t,2)*y[1] + 3*t*t*(1-t)*y[2] + pow(t,3)*y[3];
mat[put_x][put_y]=1;
}
Store_ppm();
}

void Store_ppm()
{
    FILE *output;
    int x = 0;
    output = fopen("image.ppm", "a");
    fprintf(output, "P3\n");
    fprintf(output, "%d %d\n", imageHeight, imageWidth);
    fprintf(output, "255\n");
    for (int i = 0; i < imageHeight; i++)
    {
        for (int j = 0; j < imageWidth; j++)
        {
            if (mat[i][j] == 1)
            {
                fputs("255 255 255 ", output);
                x++;
            }
            else
                fputs("0 0 0 ", output);
        }
        fprintf(output, "\n");
    }
    printf("Image correctly rendered\n");
    fclose(output);
    free(output);
}