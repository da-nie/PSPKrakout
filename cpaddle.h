#ifndef CPADDLE_H_INCLUDED
#define CPADDLE_H_INCLUDED

#include "cobject.h"

class CPaddle
{
 protected:
  //-----------------------------------------------------------
  float Y;//���������� ������ �����
  float X;
  int Height;//������ �����
  int Width;//������ �����
  bool Double;//true-����� �������
  bool Missile;//true-������ ����
  float Speed;//�������� ����������� �����
  bool Move;//true-������������ �������� �����
  bool Magnet;//true-��� ��������� � �����
  //-----------------------------------------------------------
  CObject cObject_PaddleNormal;//������� ���������� � ��������� �����
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
