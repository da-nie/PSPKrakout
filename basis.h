#ifndef BASIS_H_INCLUDED
#define BASIS_H_INCLUDED

#include <stdio.h>

struct SGameParameters
{
 int Life;//жизни
 int Score;//очки
 int Level;//уровень
 int BallSpeed;//скорость мячей
 int BallSpeedPrizeTime;//время работы призов ускорения и замедления
 bool RightWall;//true-существует правая стенка
 int RightWallTime;//время существования правой стенки
 int StartLevel;//начальный уровень для практики
 bool EnableCharacter[10];//true-персонаж доступен на уровне
 bool Practics;//true-режим практики
 bool Demonstrations;//true-режим демонстрации
 int ControlMode;//режим управления
 int LevelAmount;//количество доступных уровней
 int SelectBlock_X;//выбранный блок для автоматического управления биткой
 int SelectBlock_Y;
};
//--------------------------------------------------------------------------
int RND(int Max);
float RNDf(float Max);
void Normalize(float &vx,float &vy);
void Refraction(float vx,float vy,float nx,float ny,float &rx,float &ry);
int CircleIntercectionToCircle(float xc1,float yc1,float rc1,float xc2,float yc2,float rc2);
int CircleIntercectionToLine(float x1,float y1,float x2,float y2,float xc,float yc,float r);
bool FindStringInFile(FILE *File,char *SearchString);
bool GetParameters(char *FileName,char *ParametersName,int *parameters);
#endif
