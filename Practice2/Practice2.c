#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define TAM  500
#define TAM2 40000000
#define numerox 1920  
#define numeroy 1080 
char archivo[] = "cubito.obj"; 

FILE *onion;
FILE *entrada;

struct lectura{

    double x;
    double y;
    double z;
    
};  

struct lecturaf{

    int f1;
    int f2;
    int f3;
    int f4;
    
}; 

int cont = 0, cont2=0;
int mat[numerox][numeroy] = {0};
struct lectura lector[TAM2];
struct lecturaf lectorf[TAM2];

void dibujadoppm();
void lineasdda(double xi, double xf, double yi, double yf, double zi, double zf);
int leer_archivo();
void leer_archivo_f();
void imprimir();
void dibujar1();

int main()
{
    system("cls");
	dibujadoppm();
    leer_archivo();
    leer_archivo_f();
    imprimir();
    dibujar1();
}

void dibujar1(){

    int f1, f2, f3, f4, i=0;

    while(i<cont2){

        f1 =  lectorf[i].f1;
        f2 =  lectorf[i].f2;
        f3 =  lectorf[i].f3;
        f4 =  lectorf[i].f4;

        /*printf("%s %d %d %d %d\n", aux3, f1+1, f2+1, f3+1, f4+1);
        printf("v %f %f %f\n", lector[f1].x, lector[f1].y, lector[f1].z);
        printf("v %f %f %f\n", lector[f2].x, lector[f2].y, lector[f2].z);
        printf("v %f %f %f\n", lector[f3].x, lector[f3].y, lector[f3].z);
        printf("v %f %f %f\n", lector[f4].x, lector[f4].y, lector[f4].z);*/

        lineasdda(lector[f1].x*60, lector[f2].x*60, lector[f1].y*60, lector[f2].y*60, lector[f1].z*60, lector[f2].z*60);
        lineasdda(lector[f2].x*60, lector[f3].x*60, lector[f2].y*60, lector[f3].y*60, lector[f2].z*60, lector[f3].z*60);
        lineasdda(lector[f3].x*60, lector[f4].x*60, lector[f3].y*60, lector[f4].y*60, lector[f3].z*60, lector[f4].z*60);
        lineasdda(lector[f4].x*60, lector[f1].x*60, lector[f4].y*60, lector[f1].y*60, lector[f4].z*60, lector[f1].z*60);
        i++;

    }

    dibujadoppm();
    //imprimir();
    
    printf("Proceso finalizado operaciones de lectura: %d", cont+cont2);

}


void imprimir()
{

    int i=0;

    while(i<=cont)
    {
        printf("v %f %f %f\n", lector[i].x, lector[i].y, lector[i].z);
        i++;
    }

    i=0;

    while(i<cont2)
    {
        printf("f %d %d %d %d \n", lectorf[i].f1, lectorf[i].f2, lectorf[i].f3, lectorf[i].f4);
        i++;
    }

}

int leer_archivo()
{
    int i=0,j=0;
    entrada = fopen(archivo, "rb");
    char aux3[TAM];
    double x1,y1,z1;
    x1 = y1 = z1 = 0;

    while(feof(entrada)==0){

        fscanf(entrada, "%s", aux3);
        //fgets(aux2, 200, entrada);
        fscanf(entrada, "%lf %lf %lf", &x1, &y1, &z1);

        if(i>3)
        {

            lector[j].x = x1;
            lector[j].y = y1;
            lector[j].z = z1;
            //printf("%lf %lf %lf\n", x1, y1, z1);
            

            if(strcmp(aux3,"f")==0){
                
                j=j-1;
                cont = j;
                break;
                
            }
            //printf("%f %f %f %d\n", lector[j].x, lector[j].y, lector[j].z, cont);
            j++;
        }     
        i++;
    }
    //system("pause");

    //printf("%d", i);
    fclose(entrada);

    return 0;

}

void leer_archivo_f(){

    int i=0,j=0;
    entrada = fopen(archivo, "rb");
    char aux3[TAM];
    int f1,f2,f3,f4;
    f1 = f2 = f3 = f4 = 0;

    while(feof(entrada)==0){

        fscanf(entrada, "%s", aux3);
        //fgets(aux2, 200, entrada);
        fscanf(entrada, "%d %d %d %d", &f1, &f2, &f3, &f4);

        if(strcmp(aux3, "f")==0)
        {

            lectorf[j].f1 = f1;
            lectorf[j].f2 = f2;
            lectorf[j].f3 = f3;
            lectorf[j].f4 = f4;
            
            //printf("%d %d %d %d contador: %d\n", lectorf[j].f1, lectorf[j].f2, lectorf[j].f3, lectorf[j].f4, j+1);
            j++;
            cont2=j-1;
        }     
        i++;
    }
    //system("pause");

    //printf("%d", i);
    fclose(entrada);

}

void dibujadoppm(){

	onion = fopen("imagen.ppm", "w+");

    int i=0, j=0;
    char uno[30];

    sprintf(uno,"P3\n%d %d\n255\n",numerox, numeroy);
    fputs(uno, onion);

    for(i=0; i<=numerox; i++){
        for(j=0; j<=numeroy; j++){
            char dos[] = "";
            sprintf(dos, "%d %d %d ", mat[i][j], mat[i][j], mat[i][j]);
            fputs(dos, onion);
        }
    }

    fclose(onion);
	
}

void lineasdda(double xi, double xf, double yi, double yf, double zi, double zf){

    float dx = 0, dy = 0, incx = 0, incy = 0, p = 0;
    int x = 0, y = 0;

    dx = xf - xi;
    dy = yf - yi;

    if (fabs(dx) >= fabs(dy))
        p = fabs(dx);
    else 
        p = fabs(dy);
    incx = (dx/p);
    incy = (dy/p);

    x = xi;
    y = yi;

    for (int i = 0; i <= p; i++)
    {
        //printf("(%d, %d)\n",x, y);
        mat[x][y]= 255;
        x+=incx;
        y+=incy;
    }
 
}
