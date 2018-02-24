#ifndef CBALLMAP_H_INCLUDED
#define CBALLMAP_H_INCLUDED

#include "cobject.h"

struct SBall
{
 int Radius;//радиус м€ча
 float X;//координаты центра м€ча
 float Y;
 float Vx;//вектор полЄта м€ча
 float Vy;
 int Mode;//состо€ние м€ча (0-следует за биткой, 1-летит)
 int Image;//номер картинки м€ча
 int WaitTime;//врем€ ожидани€ м€ча битки
 bool NoWaitTime;//true-м€ч прилип из-за приза G
 int MagnetDy;//смещение дл€ прилипшего с€ча относительно центра битки
};
class CBallMap
{
 protected:
  //-----------------------------------------------------------
  CObject cObject_Ball;//спрайты м€ча
  SBall sBall_Array[100];//м€чи
  int CurrentBall;//текущее число м€чей
  int Frame;//число кадров м€ча
  //-----------------------------------------------------------
 public:
  //-----------------------------------------------------------
  void Create(char *Path);//создать объект BALLMAP
  void Release(void);//освободить объект BALLMAP
  //-----------------------------------------------------------
  int GetCurrentBall(void);//получить текущее число м€чей
  void GetBallCoord(int ball,float &x,float &y);//получить координаты м€ча
  void GetBallVector(int ball,float &vx,float &vy);//получить вектор полЄта м€ча
  //-----------------------------------------------------------
  void InitializeToStartLevel(void);//инициализировать поле на начало уровн€
  //-----------------------------------------------------------
  void CreateNewBall(float x,float y,float vx,float vy,int mode);//создать новый м€ч
  void DeleteBall(int ball);//удалить м€ч
  //-----------------------------------------------------------
  void PutAllBall(void);//вывести все м€чи
  //-----------------------------------------------------------
  void ChangeImageFrame(void);//сменить кадр спрайта м€ча
  //-----------------------------------------------------------
  bool Processing(void);//процесс управлени€ м€чами
  bool MoveBall(int ball);//двинуть м€ч
  void StartAllBall(void);//запустить все м€чи в полЄт
  void SetAllBallToPaddle(void);//присоединить м€ч к битке (по координатам)
  int GetNearestBallNumber(float x,float y);//получить ближайший к координатам м€ч
  int GetNearestToVerticalLineBallNumber(float x);//получить ближайший к вертикально линии м€ч (нужно дл€ демонстрации)
  //-----------------------------------------------------------
};
#endif
