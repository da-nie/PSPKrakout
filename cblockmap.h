#ifndef CBLOCKMAP_H_INCLUDED
#define CBLOCKMAP_H_INCLUDED
#include <stdio.h>
#include "cobject.h"

struct SBlock
{
 int ImageX;//координаты картинки
 int ImageY;
 int Break;//число попаданий для разрушения блока
 bool Obstacle;//true-блок является препятствием
 int Prize;//тип приза для призового блока
 bool PrizeActive;//true-приз активен
 int PrizeTime;//время существования приза
 bool Space;//true-блок-пустое пространство
 //-----------------------------------------------------------
 int Dust;//стадии растворения блока
};
class CBlockMap
{
 protected:
  //-----------------------------------------------------------
  SBlock sBlock_Array[12][26];
  CObject cObject_Block;
  CObject cObject_BlockPrize;
  CObject cObject_UpWall;
  CObject cObject_DownWall;
  CObject cObject_LeftWall;
  unsigned char Dust[10][150];//маски для растворения блоков
  //-----------------------------------------------------------
 public:
  //-----------------------------------------------------------
  void Create(char *Path);
  void Release(void);
  //-----------------------------------------------------------
  SBlock GetBlock(int x,int y);
  //-----------------------------------------------------------
  bool LoadMap(char *Path);
  void PutMap(void);
  void ActivateBlock(int x,int y);
  void ChangeImageFrame(void);
  void ExplosionBlock(int x,int y);
  void DestroyBlock(int x,int y);
  int LevelOver(void);
 //-----------------------------------------------------------
};
#endif
