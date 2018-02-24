#ifndef CCHARACTER2MAP_H_INCLUDED
#define CCHARACTER2MAP_H_INCLUDED

#include "cobject.h"

struct SCharacter2
{
 float X;
 float Y;
 int CurrentImage;
 int ChangeImage;
 //��������� ����������� ���������
 int ModeOfMove;//����� �����������
 int TimeOfMove;//����� �����������
 int TimeOfLife;//����� ����� ���������
 int LifeMode;//����� (1-��������;2-�����;3-������,4-�����)
};

class CCharacter2Map
{
 protected:
  CObject cObject_Sprite;//������� ���������
  int Birth_Frame_Start;
  int Birth_Frame_End;
  int Life_Frame_Start;
  int Life_Frame_End;
  int Destruction_Frame_Start;
  int Destruction_Frame_End;
  int Explosion_Frame_Start;
  int Explosion_Frame_End;
  SCharacter2 sCharacter2[5];
  int CharacterAmount;
  float Radius;
  int TimerDoStartCharacter;//����� �� ��������� ��������� (���� ��� ��������)
  int Step;//��� ��������
 public:
  void Create(char *Path);
  void Release(void);
  void InitializeToStartLevel(void);
  void CreateNewCharacter(int x,int y);
  void DestroyCharacter(int character);
  void DeleteCharacter(int character);
  void ExplosionCharacter(int character);
  void AnimateCharacter(void);
  void Processing(void);
  void MoveCharacter(void);
  void PutCharacter(void);
  void DestroyAllCharacter(void);
  void ImpactCharacterToBall(int ball_x,int ball_y,int ball_radius,float &vx,float &vy);
  bool ImpactCharacterToMissile(int xleft,int xright,int yup,int ydown);
};
#endif
