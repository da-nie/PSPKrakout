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

//данные единичного звука (44100, 16 бит)
struct SSound
{
 unsigned int *Data;//данные звука (дл€ двух каналов сразу!)
 int Length;//длина звука в 16-ти разр€дных словах дл€ одного канала
 int ID;//идентификатор звука

 SSound *Next;//указатель на следующий элемент
 SSound *Prior;//указатель на предыдущий элемент
};

//воспроизводимый в данный момент звук
struct SPlaySound
{
 SSound *sSoundPtr;//указатель на данные звука
 int Offset;//текущее положение проигрывани€
 int Channel;//задействованный канал

 SPlaySound *Next;//указатель на следующий элемент
 SPlaySound *Prior;//указатель на предыдущий элемент
};

class CSound
{
 protected:
  CContainerList<SSound> cContainerList_SSound;//контейнер загруженных звуков
  CContainerList<SPlaySound> cContainerList_SPlaySound;//контейнер воспроизводимых звуков
  pspAudioCallback_t CallBackFunction;//функци€ обратного вызова
 public:
  CSound();
  ~CSound();
  bool LoadNewSound(char *FileName,int ID);//загрузить звуковой файл
  void CallBack(void* buf,unsigned int length,void *userdata);//функци€ обратного вызова канала
  void SetCallBackFunction(pspAudioCallback_t callback);//указать функцию обратного вызова аудиоканалов
  bool Play(int ID);//запустить на воспроизведение звук с идентификатором ID
  void StopAllSound(void);//остановить воспроизведение всех звуков
  bool CheckEndAllSound(void);//вы€снить, закончилось ли воспроизведение по всем каналам
};
#endif
