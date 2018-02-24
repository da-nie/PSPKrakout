#ifndef CMENU_H_INCLUDED
#define CMENU_H_INCLUDED

#include "cobject.h"
#include "cgame.h"

struct SHiScore
{
 char Name[21];//���
 int Score;//���������� �����
};

class CMenu
{
 protected:
  CSprite cSprite_Back;//������� ����
  CSprite cSprite_Name[7];//������� �������� ����
  SHiScore sHiScore[10];//������� ��������
  int Phase;//���� �������� ���� ��������
  //-----------------------------------------------------------
  CGame cGame;
 public:
  CMenu(void);
  ~CMenu();
  //-----------------------------------------------------------
  void Create(char *Path);
  void Release(void);
  //-----------------------------------------------------------
  void SaveHiScore(char *Path);//���������� ������� ��������
  bool LoadHiScore(char *Path);//�������� ������� ��������
  void CreateHiScore(void);//������� ������� ��������
  void FindAllLevels(char *Path);//����� ��� ������
  void DrawHiScore(void);//������� ������� ��������
  void InputHiScore(int score);//���� ����� � ������� ��������
  //-----------------------------------------------------------
  void Activate(char *Path);//��������� �� ����������
  //-----------------------------------------------------------
};

#endif
