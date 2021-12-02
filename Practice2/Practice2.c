#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define name "cubito.obj"
#define imageHeight 1920
#define imageWidth 1920

int mat[imageHeight][imageWidth] = {0};

typedef struct
{
  float x;
  float y;
  float z;
} vertex;

typedef struct
{
  int a;
  int b;
  int c;
} faces;

typedef struct
{
  int a;
  int b;
} edges;

void count_vertexes_faces(int *nv, int *nf);
void Convert_to_VLF(vertex *vertex_array, faces *faces_array, int nv, int nf);
vertex *Read_OBJ_vertex(int nv);
faces *Read_OBJ_faces(int nf, int nv);
void Draw_Line_Bresenham(int x1, int y1, int x2, int y2);
void setProjectionMatrix(const float *angleOfView, const float *near, const float *far, float M[4][4]);
vertex multPointMatrix(vertex in, float M[4][4]);
void Make_2D(vertex *vertex_array, faces *faces_array, int nf);
void Make_3D(vertex *vertex_array, faces *faces_array, int nf, int nv);
void Store_ppm();
void Translation(int pixels, char *direction);
void setRotationMatirx(const float xang,const float yang, const float zang, float M[3][3]);
vertex multPointRotationMatrix(vertex in, float M[3][3]);
vertex *Rotation(float xang,float yang,  float zang, vertex *vertex_array, faces *faces_array, int nv, int nf);

int main()
{
  int option, nv = 0, nf = 0;
  count_vertexes_faces(&nv, &nf);                                             // count how many vertex and faces the obj have
  vertex *vertex_array = Read_OBJ_vertex(nv);                                 // read the vertexex of the obj
  faces *faces_array = Read_OBJ_faces(nf, nv);                                // read the faces of the obj
  //Convert_to_VLF(vertex_array, faces_array, nv, nf);                          // Convert the obj to a vlf format type
  vertex_array = Rotation(0, 0, 360, vertex_array, faces_array, nv, nf); // To rotate and object uncomment 
  Make_3D(vertex_array, faces_array, nf, nv);                                 // render the obj
  //Translation(500, "up");                                                     // To traslate the object uncomment //to indicate direction use up,down,left or right  
  //Translation(500 , "right");                                                     
  Store_ppm();                                                                // Store the object in the ppm
  return 1;
}

void count_vertexes_faces(int *nv, int *nf)
{
  FILE *input;
  char k[20];
  input = fopen(name, "r");
  if (input == NULL)
  {
    printf("Error:file can´t be opened\n");
    exit(1);
  }
  while (!feof(input))
  {
    fgets(k, 20, input);
    if (k[0] == 'v')
      *nv = *nv + 1;
    if (k[0] == 'f')
      *nf = *nf + 1;
  }
  fclose(input);
  free(input);
}

void Convert_to_VLF(vertex *vertex_array, faces *faces_array, int nv, int nf)
{
  FILE *output;
  output = fopen("VLF.flv", "a");
  int edg = 1;
  fprintf(output, "Vertexes\n");
  for (int i = 0; i < nv; i++)
  {
    fprintf(output, "%d.-%f %f %f\n", i + 1, vertex_array[i].x, vertex_array[i].y, vertex_array[i].z);
  }
  edges *edges_array = ((edges *)malloc(sizeof(edges) * (nf * 3)));
  fprintf(output, "Edges\n");
  for (int i = 0; i < nf; i++)
  {
    fprintf(output, "%d %d %d\n", edg, faces_array[i].a, faces_array[i].b);
    edges_array[edg - 1].a = edg, faces_array[i].a;
    edges_array[edg - 1].b = edg, faces_array[i].b;
    edg++;
    fprintf(output, "%d %d %d\n", edg, faces_array[i].b, faces_array[i].c);
    edges_array[edg - 1].a = edg, faces_array[i].b;
    edges_array[edg - 1].b = edg, faces_array[i].c;
    edg++;
    fprintf(output, "%d %d %d\n", edg, faces_array[i].c, faces_array[i].a);
    edges_array[edg - 1].a = edg, faces_array[i].c;
    edges_array[edg - 1].b = edg, faces_array[i].a;
    edg++;
  }
  fprintf(output, "Faces\n");
  for (int i = 0; i < nf - 1; i++)
  {
    fprintf(output, "%d %d %d %d\n", i + 1, faces_array[i].a, faces_array[i].b, faces_array[i].c);
  }
  fclose(output);
  free(output);
}

vertex *Read_OBJ_vertex(int nv)
{
  FILE *input;
  float x, y, z;
  char k[1];
  int xmin = 0, ymin = 0, zmin = 0;
  vertex *vertex_array = ((vertex *)malloc(sizeof(vertex) * nv));
  input = fopen(name, "r");
  if (input == NULL)
  {
    printf("Error:file cant be opened\n");
    exit(1);
  }
  for (int i = 0; i < nv; i++)
  {
    fscanf(input, "%s %f %f %f", k, &x, &y, &z);
    if (k[0] == 'v')
    {
      if (x < xmin)
        xmin = x;
      if (y < ymin)
        ymin = y;
      if (z < zmin)
        zmin = z;
      vertex_array[i].x = x;
      vertex_array[i].y = y;
      vertex_array[i].z = z;
    }
  }
  fclose(input);
  free(input);
  return vertex_array;
}

faces *Read_OBJ_faces(int nf, int nv)
{
  FILE *input;
  char k[100];
  int i, x = 0;
  float a, b, c;
  faces *faces_array = ((faces *)malloc(sizeof(faces) * nf));
  input = fopen(name, "r");
  if (input == NULL)
  {
    printf("Error:file cant be opened\n");
    exit(1);
  }
  for (i = 0; i < nv + nf; i++)
  {
    fscanf(input, "%s %f %f %f", k, &a, &b, &c);
    if (k[0] == 'f')
    {
      faces_array[x].a = a;
      faces_array[x].b = b;
      faces_array[x].c = c;
      x++;
    }
  }
  fclose(input);
  free(input);
  return faces_array;
}

void Draw_Line_Bresenham(int x1, int y1, int x2, int y2)
{
  int x, y, dx, dy, dx1, dy1, px, py, xe, ye;
  dx = x2 - x1;
  dy = y2 - y1;
  dx1 = fabs(dx);
  dy1 = fabs(dy);
  px = 2 * dy1 - dx1;
  py = 2 * dx1 - dy1;
  if (dy1 <= dx1)
  {
    if (dx >= 0)
    {
      x = x1;
      y = y1;
      xe = x2;
    }
    else
    {
      x = x2;
      y = y2;
      xe = x1;
    }
    mat[x][y] = 1;
    for (int i = 0; x < xe; i++)
    {
      x = x + 1;
      if (px < 0)
      {
        px = px + 2 * dy1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
        {
          y = y + 1;
        }
        else
        {
          y = y - 1;
        }
        px = px + 2 * (dy1 - dx1);
      }
      mat[x][y] = 1;
    }
  }
  else
  {
    if (dy >= 0)
    {
      x = x1;
      y = y1;
      ye = y2;
    }
    else
    {
      x = x2;
      y = y2;
      ye = y1;
    }
    mat[x][y] = 1;
    for (int i = 0; y < ye; i++)
    {
      y = y + 1;
      if (py <= 0)
      {
        py = py + 2 * dx1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
        {
          x = x + 1;
        }
        else
        {
          x = x - 1;
        }
        py = py + 2 * (dx1 - dy1);
      }
      mat[x][y] = 1;
    }
  }
}

void setProjectionMatrix(const float *angleOfView, const float *near, const float *far, float M[4][4])
{
  float scale = 1 / tan(*angleOfView * 0.5 * M_PI / 180);
  M[0][0] = scale;
  M[1][1] = scale;
  M[2][2] = -*far / (*far - *near);
  M[3][2] = (-*far * *near) / (*far - *near);
  M[2][3] = -1;
  M[3][3] = 0;
}

vertex multPointMatrix(vertex in, float M[4][4])
{
  vertex out;
  out.x = in.x * M[0][0] + in.y * M[1][0] + in.z * M[2][0] + M[3][0];
  out.y = in.x * M[0][1] + in.y * M[1][1] + in.z * M[2][1] + M[3][1];
  out.z = in.x * M[0][2] + in.y * M[1][2] + in.z * M[2][2] + M[3][2];
  float w = in.x * M[0][3] + in.y * M[1][3] + in.z * M[2][3] + M[3][3];

  if (w != 1)
  {
    out.x /= w;
    out.y /= w;
    out.z /= w;
  }
  return out;
}

void Make_2D(vertex *vertex_array, faces *faces_array, int nf)
{
  int a, b, c;
  for (int i = 1; i < nf; i++)
  {
    a = faces_array[i].a;
    b = faces_array[i].b;
    c = faces_array[i].c;
    Draw_Line_Bresenham(vertex_array[a - 1].x, vertex_array[a - 1].y, vertex_array[b - 1].x, vertex_array[b - 1].y);
    Draw_Line_Bresenham(vertex_array[b - 1].x, vertex_array[b - 1].y, vertex_array[c - 1].x, vertex_array[c - 1].y);
    Draw_Line_Bresenham(vertex_array[c - 1].x, vertex_array[c - 1].y, vertex_array[a - 1].x, vertex_array[a - 1].y);
  }
}

void Make_3D(vertex *vertex_array, faces *faces_array, int nf, int nv)
{
  float M[4][4] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, Camera[4][4] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
  float angleOfView = 90;
  float near = 0.1;
  float far = 100;
  int x = 0, y = 0;
  vertex vertCamera, projectedVert;
  Camera[3][1] = -10;
  Camera[3][2] = -20;
  vertex *vertex_array_3D = ((vertex *)malloc(sizeof(vertex) * nv));
  setProjectionMatrix(&angleOfView, &near, &far, M);
  for (int i = 0; i < nv; i++)
  {
    vertCamera = multPointMatrix(vertex_array[i], Camera);
    projectedVert = multPointMatrix(vertCamera, M);
    if (projectedVert.x < -1 || projectedVert.x > 1 || projectedVert.y < -1 || projectedVert.y > 1)
      continue;
    x = (imageHeight - (projectedVert.x + 1) * 0.8 * (imageHeight));
    y = (imageWidth - (projectedVert.y + 1) * 0.8 * (imageWidth));
    vertex_array_3D[i].x = y;
    vertex_array_3D[i].y = x;
  }
  Make_2D(vertex_array_3D, faces_array, nf);
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

void Translation(int pixels, char *direction)
{

  if (strcmp(direction, "up") == 0)
  {
    int aux[imageWidth];
    for (int i = 0; i < pixels; i++)
    {
      memcpy(aux, mat[0], sizeof(int) * imageWidth);
      for (int j = 0; j < imageHeight; j++)
        memcpy(mat[j], mat[j + 1], sizeof(int) * imageWidth);

      memcpy(mat[imageHeight - 1], aux, sizeof(int) * imageWidth);
    }
  }

  if (strcmp(direction, "down") == 0)
  {
    int aux[imageWidth];
    for (int i = 0; i < pixels; i++)
    {
      memcpy(aux, mat[imageHeight - 1], sizeof(int) * imageWidth);
      for (int j = imageHeight - 1; j >= 0; j--)
        memcpy(mat[j], mat[j - 1], sizeof(int) * imageWidth);

      memcpy(mat[0], aux, sizeof(int) * imageWidth);
    }
  }

  if (strcmp(direction, "left") == 0)
  {
    int aux[imageHeight];
    for (int i = 0; i < pixels; i++)
    {
      for (int k = 0; k < imageHeight; k++)
        aux[k] = mat[k][0];

      for (int j = 0; j < imageWidth; j++)
        for (int t = 0; t < imageHeight; t++)
          mat[t][j] = mat[t][j + 1];

      for (int k = 0; k < imageHeight; k++)
        mat[k][imageWidth - 1] = aux[k];
    }
  }
  if (strcmp(direction, "right") == 0)
  {
    int aux[imageHeight];
    for (int i = 0; i < pixels; i++)
    {
      for (int k = 0; k < imageHeight; k++)
        aux[k] = mat[k][imageWidth - 1];

      for (int j = imageWidth - 1; j >= 0; j--)
        for (int t = 0; t < imageHeight; t++)
          mat[t][j] = mat[t][j - 1];

      for (int k = 0; k < imageHeight; k++)
        mat[k][0] = aux[k];
    }
  }
}

void setRotationMatirx(const float xang,const float yang, const float zang, float M[3][3])
{
  M[0][0] = cos(yang);
  M[0][1] = sin(yang) * sin(zang);
  M[0][2] = sin(yang) * cos(zang);
  M[1][0] = sin(yang) * sin(xang);
  M[1][1] = (cos(zang) * cos(xang)) - (cos(yang) * sin(xang) * sin(zang));
  M[1][2] = -cos(xang) * sin(zang) - (cos(yang) * cos(xang) * sin(zang));
  M[2][0] = -sin(yang) * cos(xang);
  M[2][1] = (cos(zang) * sin(xang)) + (cos(yang) * sin(xang) * sin(zang));
  M[2][2] = -sin(xang) * sin(zang) - (cos(yang) * cos(xang) * cos(zang));
}

vertex multPointRotationMatrix(vertex in, float M[3][3])
{
  vertex out;
  out.x = in.x * M[0][0] + in.y * M[1][0] + in.z * M[2][0];
  out.y = in.x * M[0][1] + in.y * M[1][1] + in.z * M[2][1];
  out.z = in.x * M[0][2] + in.y * M[1][2] + in.z * M[2][2];

  return out;
}

vertex *Rotation( float xang,float yang, float zang, vertex *vertex_array, faces *faces_array, int nv, int nf)
{
  float M[3][3];
  setRotationMatirx(yang, xang, zang, M);
  for (int i = 0; i < nv; i++)
  {
    vertex_array[i] = multPointRotationMatrix(vertex_array[i], M);
  }
  return vertex_array;
}
