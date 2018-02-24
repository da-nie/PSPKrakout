#ifndef CMISSILEMAP_H_INCLUDED
#define CMISSILEMAP_H_INCLUDED

#include "cobject.h"

struct SMissile
{
 float X;
 float Y;
 int Image;
 int Height;
 int Width;
};
class CMissileMap
{
 protected:
  //-----------------------------------------------------------
  CObject cObject_Missile;
  //-----------------------------------------------------------
  SMissile sMissile_Array[100];
  int CurrentMissile;
  int Frame;
  //-----------------------------------------------------------
 public:
  //-----------------------------------------------------------
  void Create(char *Path);
  void Release(void);
  //-----------------------------------------------------------
  void InitializeToStartLevel(void);
  void CreateNewMissile(float x,float y);
  void DeleteMissile(int missile);
  void PutAllMissile(void);
  void Processing(void);
  void ChangeImageFrame(void);
 //-----------------------------------------------------------
};
#endif
