#include <stdio.h>
#include <string.h>

#include "csound.h"
#include "ccontainer.h"

CSound cSound;

CSound::CSound()
{

}
CSound::~CSound()
{
 //удаляем все воспроизводимые звуки
 StopAllSound();
 //удаляем все загруженные звуки
 SSound *sSoundPtr=cContainerList_SSound.GetFirstElement();
 while(sSoundPtr!=NULL)
 {
  if (sSoundPtr->Data!=NULL) delete(sSoundPtr->Data);
  sSoundPtr=sSoundPtr->Next;
 }
 cContainerList_SSound.Release();
}

bool CSound::LoadNewSound(char *FileName,int ID)
{
 SceUID SceUID_File;
 SceUID_File=sceIoOpen(FileName,PSP_O_RDONLY,0777);//открываем файл
 if (SceUID_File<0) return(false);//ошибка
 long length=0;
 if (sceIoRead(SceUID_File,&length,sizeof(long))<(int)sizeof(long))//ошибка - мало данных
 {
  sceIoClose(SceUID_File);
  return(false);
 }
 //читаем файл
 unsigned char *data=new unsigned char[length];
 //считываем файл целиком
 if (sceIoRead(SceUID_File,data,length)<length)//ошибка - мало данных
 {
  sceIoClose(SceUID_File);
  delete(data);
  return(false);
 }
 sceIoClose(SceUID_File);
 length/=2;//переводим в 16-ти разрядные слова
 SSound sSound;
 sSound.Data=new unsigned int[length];
 sSound.ID=ID;
 //конвертируем файл
 unsigned short value;
 unsigned short *ptr=(unsigned short*)data;
 unsigned short *d_ptr=(unsigned short*)sSound.Data;
 for(int offset=0;offset<length;offset++)
 {
  value=*ptr;ptr++;
  //размещаем в два канала
  *d_ptr=value;d_ptr++;
  *d_ptr=value;d_ptr++;
 }
 delete(data);
 sSound.Length=length;
 sSound.Prior=NULL;
 sSound.Next=NULL;
 cContainerList_SSound.AddElement(sSound);
 return(true);
}
void CSound::CallBack(void* buf,unsigned int length,void *userdata)
{
 //найдём звук с требуемыми данными
 int channel=(int)userdata;
 SPlaySound *sPlaySoundPtr=cContainerList_SPlaySound.GetFirstElement();
 while(sPlaySoundPtr!=NULL)
 {
  if (sPlaySoundPtr->Channel==channel) break;
  sPlaySoundPtr=sPlaySoundPtr->Next;
 }
 if (sPlaySoundPtr==NULL)//не нашли
 {
  memset((char*)buf,0,length*4);
  sceAudioChRelease(channel);//освобождаем канал, он почему-то бесхозный
  return;
 }
 //передаём в канал данные
 int s_amount=length;
 SSound *sSoundPtr=sPlaySoundPtr->sSoundPtr;//узнаём, с каким звуком мы связаны
 //вычисляем, сколько данных нужно отправить в канал
 if (s_amount+sPlaySoundPtr->Offset>sSoundPtr->Length) s_amount=sSoundPtr->Length-sPlaySoundPtr->Offset;
 if (s_amount<=0)//воспроизведение закончено
 {
  memset((char*)buf,0,length*4);
  sceAudioChRelease(channel);//освобождаем канал
  cContainerList_SPlaySound.DeleteElement(sPlaySoundPtr);//удаляем элемент из списка воспроизведения
  return;
 }
 //копируем данные в канал
 memcpy((char*)buf,(char*)&sSoundPtr->Data[sPlaySoundPtr->Offset],s_amount*4);
 sPlaySoundPtr->Offset+=s_amount;
}
void CSound::SetCallBackFunction(pspAudioCallback_t callback)
{
 CallBackFunction=callback;
}
bool CSound::Play(int ID)
{
 //ищем звуковые данные
 SSound *sSoundPtr=cContainerList_SSound.GetFirstElement();
 while(sSoundPtr!=NULL)
 {
  if (sSoundPtr->ID==ID) break;
  sSoundPtr=sSoundPtr->Next;
 }
 if (sSoundPtr==NULL) return(false);//не нашли
 //запускаем на воспроизведение
 SPlaySound sPlaySound;
 sPlaySound.sSoundPtr=sSoundPtr;
 sPlaySound.Offset=0;
 int channel=sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL,PSP_AUDIO_SAMPLE_ALIGN(44100),PSP_AUDIO_FORMAT_STEREO);
 if (channel<0) return(false);//ошибка создания канала
 sPlaySound.Channel=channel;
 sPlaySound.Prior=NULL;
 sPlaySound.Next=NULL;
 cContainerList_SPlaySound.AddElement(sPlaySound);//добавляем в список воспроизводимых звуков
 //устанавливаем размер буфера канала
 sceAudioSetChannelDataLen(channel,16384);
 //привязываем к каналу функцию обратного вызова
 pspAudioSetChannelCallback(channel,CallBackFunction,(int*)channel);
 //всё, воспроизведение начинается
 return(true);
}
void CSound::StopAllSound(void)
{
 SPlaySound *sPlaySoundPtr=cContainerList_SPlaySound.GetFirstElement();
 while(sPlaySoundPtr!=NULL)
 {
  sceAudioChRelease(sPlaySoundPtr->Channel);//освобождаем канал
  sPlaySoundPtr=sPlaySoundPtr->Next;
 }
 cContainerList_SPlaySound.Release();
}
bool CSound::CheckEndAllSound(void)
{
 SPlaySound *sPlaySoundPtr=cContainerList_SPlaySound.GetFirstElement();
 if (sPlaySoundPtr==NULL) return(true);
 return(false);
}
