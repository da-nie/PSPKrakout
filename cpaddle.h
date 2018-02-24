#ifndef CPADDLE_H_INCLUDED
#define CPADDLE_H_INCLUDED

#include "cobject.h"

class CPaddle
{
 protected:
  //-----------------------------------------------------------
  float Y;//координаты центра битки
  float X;
  int Height;//высота битки
  int Width;//ширина битки
  bool Double;//true-битка удвоена
  bool Missile;//true-ракета есть
  float Speed;//скорость перемещения битки
  bool Move;//true-производится движение битки
  bool Magnet;//true-мяч прилипает к битке
  //-----------------------------------------------------------
  CObject cObject_PaddleNormal;//спрайты нормальной и удвоенной битки
  CObject cObject_PaddleDouble;
  //-----------------------------------------------------------
 public:
  //-----------------------------------------------------------
  void Create(char *Path);
  void Release(void);
  //-----------------------------------------------------------
  int GetHeight(void);
  int GetWidth(void);
  int GetX(void);
  int GetY(void);
  bool GetMagnet(void);
  bool GetMissile(void);
  //float GetSpeed(void);
  //-----------------------------------------------------------
  void InitializeToStartLevel(void);
  void PutPaddle(void);
  void MoveUp(void);
  void MoveDown(void);
  void Processing(void);
  void DoubleMode(void);
  void NormalMode(void);
  void MissileModeOn(void);
  void MissileModeOff(void);
  void MagnetModeOn(void);
  void MagnetModeOff(void);
  void CorrectUpYPos(int dy);
  void CorrectDownYPos(int dy);
  //-----------------------------------------------------------
};
#endif
