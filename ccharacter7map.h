#ifndef CCHARACTER7MAP_H_INCLUDED
#define CCHARACTER7MAP_H_INCLUDED

#include "cobject.h"

struct SCharacter7
{
 float X[6];
 float Y[6];
 int CurrentImage[6];
 int ChangeImage[6];
 //��������� ����������� ���������
 int ModeOfMove[6];//����� �����������
 int TimeOfMove[6];//����� �����������
 int TimeOfLife;//����� ����� ���������
 int LifeMode[6];//����� (1-��������;2-�����;3-������)
};

class CCharacter7Map
{
 protected:
  CObject cObject_Sprite;//������� ���������
  int Birth_Frame_Start;
  int Birth_Frame_End;
  int Life_Frame_Start;
  int Life_Frame_End;
  int Destruction_Frame_Start;
  int Destruction_Frame_End;
  SCharacter7 sCharacter7[5];
  int CharacterAmount;
  float Radius;
  int TimerDoStartCharacter;//����� �� ��������� ��������� (���� ��� ��������)
  int Step;//��� ��������
 public:
  void Create(char *Path);
  void Release(void);
  void InitializeToStartLevel(void);
  void CreateNewCharacter(int x,int y);
  void DestroyCharacter(int character,int segment);
  void DeleteCharacter(int character);
  void AnimateCharacter(void);
  void Processing(void);
  void MoveCharacter(void);
  void PutCharacter(void);
  void DestroyAllCharacter(void);
  void ImpactCharacterToBall(int ball_x,int ball_y,int ball_radius,float &vx,float &vy);
  bool ImpactCharacterToMissile(int xleft,int xright,int yup,int ydown);
};
#endif
