#ifndef CSOUND_H_INCLUDED
#define CSOUND_H_INCLUDED

#include <pspaudiolib.h>
#include <pspaudio.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <psprtc.h>

#include "ccontainer.h"

//������ ���������� ����� (44100, 16 ���)
struct SSound
{
 unsigned int *Data;//������ ����� (��� ���� ������� �����!)
 int Length;//����� ����� � 16-�� ��������� ������ ��� ������ ������
 int ID;//������������� �����

 SSound *Next;//��������� �� ��������� �������
 SSound *Prior;//��������� �� ���������� �������
};

//��������������� � ������ ������ ����
struct SPlaySound
{
 SSound *sSoundPtr;//��������� �� ������ �����
 int Offset;//������� ��������� ������������
 int Channel;//��������������� �����

 SPlaySound *Next;//��������� �� ��������� �������
 SPlaySound *Prior;//��������� �� ���������� �������
};

class CSound
{
 protected:
  CContainerList<SSound> cContainerList_SSound;//��������� ����������� ������
  CContainerList<SPlaySound> cContainerList_SPlaySound;//��������� ��������������� ������
  pspAudioCallback_t CallBackFunction;//������� ��������� ������
 public:
  CSound();
  ~CSound();
  bool LoadNewSound(char *FileName,int ID);//��������� �������� ����
  void CallBack(void* buf,unsigned int length,void *userdata);//������� ��������� ������ ������
  void SetCallBackFunction(pspAudioCallback_t callback);//������� ������� ��������� ������ ������������
  bool Play(int ID);//��������� �� ��������������� ���� � ��������������� ID
  void StopAllSound(void);//���������� ��������������� ���� ������
  bool CheckEndAllSound(void);//��������, ����������� �� ��������������� �� ���� �������
};
#endif
