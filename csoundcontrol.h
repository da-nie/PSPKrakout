#ifndef CSOUNDCONTROL_H_INCLUDED
#define CSOUNDCONTROL_H_INCLUDED

#include <psprtc.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <psprtc.h>

#include "csound.h"

enum
{
 SOUND_IMPACT_1,
 SOUND_IMPACT_2,
 SOUND_IMPACT_3,
 SOUND_IMPACT_4,
 SOUND_IMPACT_5,
 SOUND_IMPACT_6,
 SOUND_BREAK,

 SOUND_PRIZE_OPEN,
 SOUND_PRIZE_X,
 SOUND_PRIZE_B,
 SOUND_PRIZE_E,
 SOUND_PRIZE_G,
 SOUND_PRIZE_S,
 SOUND_PRIZE_S_END,
 SOUND_PRIZE_PLUS,
 SOUND_PRIZE_MINUS,

 SOUND_GAME_OVER,
 SOUND_LOSE,
 SOUND_MISSILE,
 SOUND_DESTROY,
 SOUND_CHANGE_LEVEL,
 SOUND_DOLLAR,
 SOUND_DOUBLE_BALL,
 SOUND_BIRTH,
 SOUND_LEVEL_UP,
 SOUND_MISSILE_ON,
};

//���������� �������
class CSoundControl
{
 protected:
  int LastID;//������������� ���������� ���������������� �����
  u64 LastID_tick;//�����, ����� ��������� ���� ��� ������������
 public:
  CSoundControl(void);
  void Create(char *Path);//������� �����
  void Play(int ID);//��������� ��������������
  void StopAllSound(void);//���������� ��������������� ���� ������
  bool CheckEndAllSound(void);//��������, ����������� �� ��������������� �� ���� �������
};

#endif
