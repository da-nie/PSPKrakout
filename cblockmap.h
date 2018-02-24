#ifndef CBLOCKMAP_H_INCLUDED
#define CBLOCKMAP_H_INCLUDED
#include <stdio.h>
#include "cobject.h"

struct SBlock
{
 int ImageX;//���������� ��������
 int ImageY;
 int Break;//����� ��������� ��� ���������� �����
 bool Obstacle;//true-���� �������� ������������
 int Prize;//��� ����� ��� ��������� �����
 bool PrizeActive;//true-���� �������
 int PrizeTime;//����� ������������� �����
 bool Space;//true-����-������ ������������
 //-----------------------------------------------------------
 int Dust;//������ ����������� �����
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
  unsigned char Dust[10][150];//����� ��� ����������� ������
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
