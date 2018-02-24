#ifndef COBJECT_H_INCLUDED
#define COBJECT_H_INCLUDED

#include "csprite.h"

class CObject
{
 public:
  //-----------------------------------------------------------
  CSprite cSprite;//������ ��������
  int HeightImage;//������� ������� (������� ��������)
  int WidthImage;
  int Height;//������� ���� ��������
  int Width;
  int ImageXPos;//���������� �������
  int ImageYPos;
  bool Blend;//true-������ ���������� ������������
  //-----------------------------------------------------------
 public:
  CObject(void);
  ~CObject(void);
  bool Load(char *FileName);
  void Put(int x,int y);
  void Put(int x,int y,unsigned char *Mask);
  void SetSpriteSize(int width,int height);
  void SetSpriteCoord(int y,int x);
  void SetBlendColor(unsigned char r,unsigned char g,unsigned char b);
  void Delete(void);
  //-----------------------------------------------------------
};
#endif // COBJECT_H_INCLUDED
