#include <stdio.h>
#include <stdlib.h>

int main(){

    FILE *imagen;
    imagen = fopen("imagen.ppm", "w+");

    char uno[30], dos[10];
    int ancho = 1920, alto=1080;
    int i, j, ban=0;
    //int r, g, b; 

    sprintf(uno,"P3\n%d %d\n255\n",ancho, alto);
    fputs(uno, imagen);

    for(j=0;j<alto;j++){
        for(i=0;i<ancho;i++){
            if(ban==0){
                if(i<640){
                    sprintf(dos,"0 255 0 ");
                    fputs(dos, imagen);
                }
                if(i>=640 && i<1280){
                    sprintf(dos,"255 255 255 ");
                    fputs(dos, imagen);
                }
                if(i>=1280){
                    sprintf(dos,"255 0 0 ");
                    fputs(dos, imagen);
                }
            }
            if(ban==1){
                if(i<640){
                    sprintf(dos,"0 255 0 ");
                    fputs(dos, imagen);
                }
                if(i>=640 && i<1280){
                    sprintf(dos,"175 37 8 ");
                    fputs(dos, imagen);
                }
                if(i>=1280){
                    sprintf(dos,"255 0 0 ");
                    fputs(dos, imagen);
                }
            }
        }
        fputs("\n", imagen);

        if(j>380 && j<700){
            ban=1;
        }
        else{ 
            ban=0;
        }
    }
    fclose(imagen);
    return 0;
}

/*
            r = rand()%256;
            g = rand()%256;
            b = rand()%256;
            sprintf(dos,"%d %d %d ",r, g, b);
            fputs(dos, imagen);
*/