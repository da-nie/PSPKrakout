#include "basis.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

//--------------------------------------------------------------------------
SGameParameters sGameParameters;

//случайное число
int RND(int Max)
{
 return((int)RNDf(Max));
}
float RNDf(float Max)
{
 return((Max*(float)rand())/(float)RAND_MAX);
}
//нормировать
void Normalize(float &vx,float &vy)
{
 float norma=sqrt(vx*vx+vy*vy);
 if (norma==0) norma=1;
 vx/=norma;
 vy/=norma;
}
//вычислить отражение
void Refraction(float vx,float vy,float nx,float ny,float &rx,float &ry)
{
 float cosa=nx*vx+ny*vy;
 rx=vx-2*cosa*nx;
 ry=vy-2*cosa*ny;
}

//проверка пересечения двух окружностей
int CircleIntercectionToCircle(float xc1,float yc1,float rc1,float xc2,float yc2,float rc2)
{
 float length=(xc2-xc1)*(xc2-xc1)+(yc2-yc1)*(yc2-yc1);
 if (length<=(rc2+rc1)*(rc2+rc1)) return(1);
 return(0);
}

int CircleIntercectionToLine(float x1,float y1,float x2,float y2,float xc,float yc,float r)
{
 //проверка пересечения окружности с отрезком.
 float xl1=x1-xc;
 float yl1=y1-yc;
 float xl2=x2-xc;
 float yl2=y2-yc;
 float a=xl2-xl1;
 float b=yl2-yl1;
 float kd=a*a+b*b;
 float ld=2*a*xl1+2*b*yl1;
 float md=xl1*xl1+yl1*yl1-r*r;
 float d=ld*ld-4*kd*md;
 if (d<0) return(0);
 float sd=(float)sqrt(d);
 float l1=(-ld+sd)/(2*kd);
 float l2=(-ld-sd)/(2*kd);
 float xp1=xl1+l1*a;
 float yp1=yl1+l1*b;
 float xp2=xl1+l2*a;
 float yp2=yl1+l2*b;
 //проверим, находится ли точка пересечения с прямой в отрезке
 if (((xp1>=xl1 && xp1<=xl2) || (xp1>=xl2 && xp1<=xl1)) &&
    ((yp1>=yl1 && yp1<=yl2) || (yp1>=yl2 && yp1<=yl1))) return(1);
 if (((xp2>=xl1 && xp2<=xl2) || (xp2>=xl2 && xp2<=xl1)) &&
    ((yp2>=yl1 && yp2<=yl2) || (yp2>=yl2 && yp2<=yl1))) return(1);
 return(0);
}
bool FindStringInFile(FILE *File,char *SearchString)
{
 //функция ищет строку в файле
 unsigned char byte;
 int pos=0;
 while(1)
 {
  if (fread(&byte,1,1,File)==0) break;
  if (byte==SearchString[pos]) pos++;
                          else pos=0;
  if (pos==(int)(strlen(SearchString))) return(true);
 }
 return(false);
}
bool GetParameters(char *FileName,char *ParametersName,int *parameters)
{
 //функция читает значение пареметра из файла
 FILE *file=fopen(FileName,"r+b");
 if (file==NULL) return(false);
 if (FindStringInFile(file,ParametersName))
 {
  fscanf(file,"%i",parameters);
  fclose(file);
  return(1);
 }
 fclose(file);
 return(true);
}
