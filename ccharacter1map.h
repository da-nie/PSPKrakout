#ifndef CCHARACTER1MAP_H_INCLUDED
#define CCHARACTER1MAP_H_INCLUDED

#include "cobject.h"

struct SCharacter1
{
 float X;
 float Y;
 int CurrentImage;
 int ChangeImage;
 //параметры перемещения персонажа
 int ModeOfMove;//режим перемещения
 int TimeOfMove;//время перемещения
 int TimeOfLife;//время жизни персонажа
 int LifeMode;//режим (1-рождение;2-жизнь;3-гибель)
};

class CCharacter1Map
{
 protected:
  CObject cObject_Sprite;//спрайты персонажа
  int Birth_Frame_Start;
  int Birth_Frame_End;
  int Life_Frame_Start;
  int Life_Frame_End;
  int Destruction_Frame_Start;
  int Destruction_Frame_End;
  SCharacter1 sCharacter1[5];
  int CharacterAmount;
  float Radius;
  int TimerDoStartCharacter;//время до появления персонажа (если это возможно)
  int Step;//шаг анимации
 public:
  void Create(char *Path);
  void Release(void);
  void InitializeToStartLevel(void);
  void CreateNewCharacter(int x,int y);
  void DestroyCharacter(int character);
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
