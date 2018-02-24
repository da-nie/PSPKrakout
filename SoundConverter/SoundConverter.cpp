#include <windows.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
void FindWAV(char *FileName);
void ConvertWAV(char *FileName);
//------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevstance,LPSTR lpstrCmdLine,int nCmdShow)
{
 //ищем файлы wav по каталогам
 char *Path=new char[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,Path);
 FindWAV(Path);
 delete(Path);
 MessageBox(NULL,"Конвертация завершена","Сообщение",MB_OK);
 return(0);
}
void FindWAV(char *StartDirectory)
{
 SetCurrentDirectory(StartDirectory);
 WIN32_FIND_DATA wfd;
 //сначала ищем файлы
 HANDLE Handle_Find=FindFirstFile("*.wav",&wfd);
 if (Handle_Find!=INVALID_HANDLE_VALUE)
 {
  while(1)
  {
   //это файл
   if (wfd.cFileName[0]!='.' && !(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
   {
    //отправляем на обработку
    ConvertWAV(wfd.cFileName);
   }
   if (FindNextFile(Handle_Find,&wfd)==FALSE) break;
  }
  FindClose(Handle_Find);
 }
 //а потом директории
 Handle_Find=FindFirstFile("*.*",&wfd);
 if (Handle_Find!=INVALID_HANDLE_VALUE)
 {
  while(1)
  {
   //это директория
   if (wfd.cFileName[0]!='.' && wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
   {
    char *dir_name=new char[strlen(StartDirectory)+strlen(wfd.cFileName)+10];
    sprintf(dir_name,"%s\\%s",StartDirectory,wfd.cFileName);
    FindWAV(dir_name);
    delete(dir_name);
   } 
   if (FindNextFile(Handle_Find,&wfd)==FALSE) break;
  }
  FindClose(Handle_Find);
 }
}
void ConvertWAV(char *FileName)
{
 //загружаем WAV-файл
 HMMIO hwav;
 MMCKINFO parent,child;
 WAVEFORMATEX wfmtx;
 parent.ckid=(FOURCC)0;
 parent.cksize=0;
 parent.fccType=(FOURCC)0;
 parent.dwDataOffset=0;
 parent.dwFlags=0;
 child=parent;
 if ((hwav=mmioOpen(FileName,NULL,MMIO_READ|MMIO_ALLOCBUF))==NULL) return;
 parent.fccType=mmioFOURCC('W','A','V','E');
 if (mmioDescend(hwav,&parent,NULL,MMIO_FINDRIFF))
 {
  mmioClose(hwav,0);
  return;
 }
 child.ckid=mmioFOURCC('f','m','t',' ');
 if (mmioDescend(hwav,&child,&parent,0))
 {
  mmioClose(hwav,0);
  return;
 }
 if (mmioRead(hwav,(char *)&wfmtx,sizeof(wfmtx))!=sizeof(wfmtx))
 {
  mmioClose(hwav,0);
  return;
 }
 if (wfmtx.wFormatTag!=WAVE_FORMAT_PCM)
 {
  mmioClose(hwav,0);
  return;
 }
 if (mmioAscend(hwav,&child,0))
 {
  mmioClose(hwav,0);
  return;
 }
 child.ckid=mmioFOURCC('d','a','t','a');
 if (mmioDescend(hwav,&child,&parent,MMIO_FINDCHUNK))
 {
  mmioClose(hwav,0);
  return;
 }
 //проверяем, что файл нужного типа
 if (wfmtx.nChannels!=1) return;//только моно
 if (wfmtx.nSamplesPerSec!=44100) return;//частота тоже нужна 44100
 if (wfmtx.nBlockAlign!=2) return;//только 16 бит
 unsigned char *sound=new unsigned char[child.cksize];
 //а теперь загружаем данные звука
 mmioRead(hwav,(char *)sound,child.cksize);
 mmioClose(hwav,0); 
 //сохраняем файл
 int length=strlen(FileName);
 //отматываем до точки
 for(int n=length;n>0;n--)
 {
  if (FileName[n-1]=='.')
  {
   FileName[n]=0;//обрубаем строку
   break;
  }
 }
 char *ExportFileName=new char[strlen(FileName)+255];
 sprintf(ExportFileName,"%sksd",FileName);
 HANDLE hFile=CreateFile(ExportFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
 delete(ExportFileName);
 if (hFile==INVALID_HANDLE_VALUE)
 {
  delete(sound);
  return;
 }
 DWORD written;
 length=child.cksize;//размер в байтах
 WriteFile(hFile,&length,sizeof(int),&written,NULL);
 //сами данные
 WriteFile(hFile,sound,length,&written,NULL); 
 CloseHandle(hFile);
 delete(sound);
}