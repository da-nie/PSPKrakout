#include <math.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <psprtc.h>

#include "cmenu.h"

#include "cgame.h"
#include "video.h"
#include "cballmap.h"
#include "cpaddle.h"
#include "cblockmap.h"
#include "cmissilemap.h"
#include "ccharactermap.h"
#include "cfont.h"
#include "basis.h"
#include "csoundcontrol.h"

extern CBallMap cBallMap;
extern CBlockMap cBlockMap;
extern CMissileMap cMissileMap;
extern CCharacterMap cCharacterMap;
extern CPaddle cPaddle;
extern CFont cFont;
extern SGameParameters sGameParameters;
extern CSoundControl cSoundControl;

extern bool done;
//--------------------------------------------------------------------------
CMenu::CMenu(void)
{
}
CMenu::~CMenu()
{
 Release();
}
void CMenu::Create(char *Path)
{
 FindAllLevels(Path);
 cFont.Load(Path);
 cPaddle.Create(Path);
 cBlockMap.Create(Path);
 cBallMap.Create(Path);
 cMissileMap.Create(Path);
 cCharacterMap.Create(Path);
 cSoundControl.Create(Path);
 cGame.Create(Path);

 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssprites\\back.tga",Path);
 cSprite_Back.Load(FileName);

 sprintf(FileName,"%ssprites\\k_name_1.tga",Path);
 cSprite_Name[0].Load(FileName);
 sprintf(FileName,"%ssprites\\k_name_2.tga",Path);
 cSprite_Name[1].Load(FileName);
 sprintf(FileName,"%ssprites\\k_name_3.tga",Path);
 cSprite_Name[2].Load(FileName);
 sprintf(FileName,"%ssprites\\k_name_4.tga",Path);
 cSprite_Name[3].Load(FileName);
 sprintf(FileName,"%ssprites\\k_name_5.tga",Path);
 cSprite_Name[4].Load(FileName);
 sprintf(FileName,"%ssprites\\k_name_6.tga",Path);
 cSprite_Name[5].Load(FileName);
 sprintf(FileName,"%ssprites\\k_name_7.tga",Path);
 cSprite_Name[6].Load(FileName);

 for(int n=0;n<7;n++) cSprite_Name[n].SetAlpha(0,0,0,0);
 delete(FileName);

 if (LoadHiScore(Path)==false) CreateHiScore();
}
void CMenu::Release(void)
{
 cBlockMap.Release();
 cMissileMap.Release();
 cBallMap.Release();
 cPaddle.Release();
 cCharacterMap.Release();
 cFont.Release();
 cGame.Release();
 cSprite_Back.Release();
 for(int n=0;n<7;n++) cSprite_Name[n].Release();
}
//--------------------------------------------------------------------------
void CMenu::SaveHiScore(char *Path)
{
 char *FileName=new char[strlen(Path)+256];
 sprintf(FileName,"%shiscore.bin",Path);
 SceUID SceUID_File;
 SceUID_File=sceIoOpen(FileName,PSP_O_WRONLY|PSP_O_CREAT,0777);//��������� ����
 if (SceUID_File<=0)//������ �������� �����
 {
  delete(FileName);
  return;
 }
 //������� �����
 unsigned char codeword[256];
 strcpy((char*)codeword,"This is BASIS codename for KrakOut Hight Score saving");
 int codewordlength=strlen((char*)codeword);
 int codepos=0;
 int n,m;
 int size=sizeof(struct SHiScore);
 unsigned char *line=new unsigned char[size+2];
 unsigned short *crc=(unsigned short*)(line+size);
 *crc=0;
 for(n=0;n<10;n++)
 {
  //��������� ������� ������� �������� � �����
  memcpy(line,(unsigned char *)&sHiScore[n],size);
  //�������� �
  for(m=0;m<size;m++)
  {
   unsigned char byte=line[m];
   *crc+=byte;
   byte=byte^codeword[codepos];
   *crc+=byte;
   codepos+=m-n;
   codepos++;
   while(codepos<0) codepos+=codewordlength;
   while(codepos>=codewordlength) codepos-=codewordlength;
   byte=byte^codeword[codepos];
   *crc+=byte;
   line[m]=byte;
  }
  //��������� ������
  if (sceIoWrite(SceUID_File,line,size+2)<size+2) break;//������ ������
 }
 delete(line);
 sceIoClose(SceUID_File);
 delete(FileName);
}
bool CMenu::LoadHiScore(char *Path)
{
 char *FileName=new char[strlen(Path)+256];
 sprintf(FileName,"%shiscore.bin",Path);
 SceUID SceUID_File;
 SceUID_File=sceIoOpen(FileName,PSP_O_RDONLY,0777);//��������� ����
 if (SceUID_File<=0)//������ �������� �����
 {
  delete(FileName);
  return(false);
 }
 unsigned char codeword[256];
 strcpy((char*)codeword,"This is BASIS codename for KrakOut Hight Score saving");
 int codewordlength=strlen((char*)codeword);
 int codepos=0;
 int n,m;
 int size=sizeof(struct SHiScore);
 unsigned char *line=new unsigned char[size+2];
 unsigned short *crc=(unsigned short*)(line+size);
 unsigned short crc_l=0;
 bool ok=true;
 for(n=0;n<10;n++)
 {
  //��������� �������
  if (sceIoRead(SceUID_File,line,size+2)<size+2)//������ ������
  {
   ok=false;
   break;
  }
  //�������������� �������
  for(m=0;m<size;m++)
  {
   char code1=codeword[codepos];
   codepos+=m-n;
   codepos++;
   while(codepos<0) codepos+=codewordlength;
   while(codepos>=codewordlength) codepos-=codewordlength;
   char code2=codeword[codepos];
   unsigned char byte=line[m];
   crc_l+=byte;
   byte=byte^code2;
   crc_l+=byte;
   byte=byte^code1;
   crc_l+=byte;
   line[m]=byte;
  }
  //���������� crc
  if ((*crc)!=crc_l)//������ ����������� �����
  {
   ok=false;
   break;
  }
  //��������� ������� �� ������ � ������� ��������
  memcpy((unsigned char *)&sHiScore[n],line,size);
 }
 delete(line);
 sceIoClose(SceUID_File);
 delete(FileName);
 return(ok);
}
void CMenu::CreateHiScore(void)
{
 strcpy(sHiScore[0].Name,"By Da-nie 10.08.2008");sHiScore[0].Score=1000000;
 strcpy(sHiScore[1].Name,"������� �����.......");sHiScore[1].Score=700000;
 strcpy(sHiScore[2].Name,"������ �������......");sHiScore[2].Score=500000;
 strcpy(sHiScore[3].Name,"��������� �������...");sHiScore[3].Score=400000;
 strcpy(sHiScore[4].Name,"������ ���������....");sHiScore[4].Score=200000;
 strcpy(sHiScore[5].Name,"����������� ��������");sHiScore[5].Score=100000;
 strcpy(sHiScore[6].Name,"������ ����.........");sHiScore[6].Score=50000;
 strcpy(sHiScore[7].Name,"�������� ���������..");sHiScore[7].Score=20000;
 strcpy(sHiScore[8].Name,"�������� �������....");sHiScore[8].Score=10000;
 strcpy(sHiScore[9].Name,"�������� �����......");sHiScore[9].Score=1000;
}
//����������� �������
void CMenu::FindAllLevels(char *Path)
{
 char *FileName=new char[strlen(Path)+256];
 sGameParameters.LevelAmount=0;
 while(done==false)
 {
  sprintf(FileName,"%slevels/level_%i.klv",Path,sGameParameters.LevelAmount+1);
  SceUID SceUID_File;
  SceUID_File=sceIoOpen(FileName,PSP_O_RDONLY,0777);//��������� ����
  if (SceUID_File<=0) break;//������ ����� ���
  sceIoClose(SceUID_File);
  sGameParameters.LevelAmount++;
 }
 delete(FileName);
}
void CMenu::DrawHiScore(void)
{
 char string[255];
 int n;
 SceCtrlData pad;
 u64 tick_frequency=sceRtcGetTickResolution();//������� �������� �������
 u64 begin_tick;
 u64 current_tick;
 sceRtcGetCurrentTick(&begin_tick);
 bool enable_control=false;
 while(!done)
 {
  if (enable_control==false)
  {
   sceRtcGetCurrentTick(&current_tick);//������� �������� �������
   double time_span=((double)(current_tick-begin_tick))/(double)tick_frequency;//��������� �����
   if (time_span>0.3) enable_control=true;
  }
  sceCtrlReadBufferPositive(&pad,1);
  if (pad.Buttons && enable_control==true) break;//������ ����� ������� � ������ ������ �������
  //������� ������� ��������
  cSprite_Back.Put(0,0,false);
  //������� ������� ��������
  cFont.PrintAt(136,10,(char*)"������� ��������",true);
  for(n=0;n<10;n++)
  {
   sprintf(string,"%s %i",sHiScore[n].Name,sHiScore[n].Score);
   cFont.PrintAt(70,40+n*20,string,true);
  }
  //������ ����� �� �����
  Video_ViewDisplayPage();
  Video_ChangeDisplayPage();
 }
}
void CMenu::InputHiScore(int score)
{
 int n;
 int posy=-1;
 for(n=0;n<10;n++)
 {
  if (sHiScore[n].Score<score)
  {
   posy=n;
   break;
  }
 }
 if (posy<0) return;//����� �� �������
 //�������� ������� �������� ����
 for(n=9;n>posy;n--)
 {
  sHiScore[n]=sHiScore[n-1];
 }
 sHiScore[posy].Score=score;
 strcpy(sHiScore[posy].Name,"                    ");
 //������ ���
 SceCtrlData pad;
 bool on_exit=false;
 unsigned char symbol=32;
 int posx=0;
 while(done==false && on_exit==false)
 {
  //������� ������ � ������� ��������
  sHiScore[posy].Name[posx]=symbol;
  //������� ������� ��������
  cSprite_Back.Put(0,0,false);
  cFont.PrintAt(136,10,(char*)"������� ��������",true);
  cFont.PrintAt(32,250,(char*)"Select-�����. ������-���� �����.",true);
  char string[255];
  for(n=0;n<10;n++)
  {
   sprintf(string,"%s %i",sHiScore[n].Name,sHiScore[n].Score);
   cFont.PrintAt(70,40+n*20,string,true);
  }
  Video_ViewDisplayPage();
  //��� ������� �� ������� �����, ����� � ����, � ��� �� �������
  while(done==false)
  {
   sceCtrlReadBufferPositive(&pad,1);
   if (pad.Buttons&PSP_CTRL_UP)//������ ������� "�����"
   {
    symbol++;
    if (symbol>(unsigned char)'~') symbol=32;
    break;
   }
   if (pad.Buttons&PSP_CTRL_DOWN)//������ ������� "�����"
   {
    symbol--;
    if (symbol<32) symbol=(unsigned char)'~';
    break;
   }
   if (pad.Buttons&PSP_CTRL_LEFT)//������ ������� "�����"
   {
    sHiScore[posy].Name[posx]=32;
    posx--;
    if (posx<0) posx=0;
    break;
   }
   if (pad.Buttons&PSP_CTRL_RIGHT)//������ ������� "������"
   {
    posx++;
    if (posx>19) posx=19;
    break;
   }
   if (pad.Buttons&PSP_CTRL_SELECT)//������ ������� select
   {
    on_exit=true;
    break;
   }
  }
  //������ ����� �� �����
  Video_ViewDisplayPage();
  Video_ChangeDisplayPage();
  //������ ��������
  sceKernelDelayThread(100000);
 }
 //��������� ������� ������
 for(n=posx+1;n<20;n++) sHiScore[posy].Name[n]='.';
}
void CMenu::Activate(char *Path)
{
 int n;
 sGameParameters.StartLevel=1;
 sGameParameters.ControlMode=0;
 Phase=0;
 int index=0;
 SceCtrlData pad;
 int key_delay=0;
 char string[255];
 while(!done)
 {
  //���������� ����������� �� ����������
  sceCtrlReadBufferPositive(&pad,1);
  bool on_key=false;
  if (pad.Buttons&PSP_CTRL_UP)
  {
   on_key=true;
   if (key_delay==0)
   {
    index--;
    key_delay=15;
   }
  }
  if (pad.Buttons&PSP_CTRL_DOWN)
  {
   on_key=true;
   if (key_delay==0)
   {
    index++;
    key_delay=15;
   }
  }
  if (pad.Buttons&PSP_CTRL_LEFT)
  {
   on_key=true;
   if (key_delay==0)
   {
    if (index==2)//����� ���������� ������
    {
     sGameParameters.StartLevel--;
     if (sGameParameters.StartLevel<1) sGameParameters.StartLevel=sGameParameters.LevelAmount;
     key_delay=10;
     }
    if (index==3)//����� ����������
    {
     sGameParameters.ControlMode--;
     if (sGameParameters.ControlMode<0) sGameParameters.ControlMode=2;
     key_delay=15;
    }
   }
  }
  if (pad.Buttons&PSP_CTRL_RIGHT)
  {
   on_key=true;
   if (key_delay==0)
   {
    if (index==2)//����� ���������� ������
    {
     sGameParameters.StartLevel++;
     if (sGameParameters.StartLevel>sGameParameters.LevelAmount) sGameParameters.StartLevel=1;
     key_delay=10;
    }
    if (index==3)//����� ����������
    {
     sGameParameters.ControlMode++;
     if (sGameParameters.ControlMode>2) sGameParameters.ControlMode=0;
     key_delay=15;
    }
   }
  }
  if (index>6) index=0;
  if (index<0) index=6;
  if (pad.Buttons&PSP_CTRL_CROSS)//������ ����� ����
  {
   on_key=true;
   if (key_delay==0)
   {
    if (index==0)//����� ����
    {
     sGameParameters.Practics=false;
     sGameParameters.Demonstrations=false;
     cGame.StartGame(Path);
     //������ � ������� �������� ���
     InputHiScore(sGameParameters.Score);
     //��������� ������� ��������
     SaveHiScore(Path);
     key_delay=100;
    }
    if (index==1)//����� ��������
    {
     sGameParameters.Practics=true;
     sGameParameters.Demonstrations=false;
     cGame.StartGame(Path);
     key_delay=100;
    }
    if (index==4)
    {
     DrawHiScore();//������ ������� ��������
     key_delay=100;
    }
    if (index==5)//������������
    {
     sGameParameters.Practics=true;
     sGameParameters.Demonstrations=true;
     cGame.StartGame(Path);
     key_delay=100;
    }

    if (index==6) break;//����� �� ����
   }
  }
  if (key_delay>0) key_delay--;
  if (on_key==false) key_delay=0;
  //������� ������� ��������
  cSprite_Back.Put(0,0,false);
  //������� ����� �������� ����
  int x=18;
  int y=0;
  for(n=0;n<7;n++)
  {
   y=7.0*sin(M_PI/180.0*(Phase+x));
   cSprite_Name[n].Put(x,10+y,true);
   x+=cSprite_Name[n].GetWidth()+5;
  }
  Phase+=10;
  Phase%=360;
  //������� ������ ����
  y=90;
  int x_o[7]={5,5,5,5,5,5,5};
  x_o[index]=15;
  cFont.PrintAt(x_o[0],y,(char*)"������ ����",true);y+=20;
  cFont.PrintAt(x_o[1],y,(char*)"������ ��������",true);y+=20;
  sprintf(string,"��������� �������:%i",sGameParameters.StartLevel);
  cFont.PrintAt(x_o[2],y,string,true);y+=20;
  if (sGameParameters.ControlMode==0) cFont.PrintAt(x_o[3],y,(char*)"����������:������ ������� � �������",true);
  if (sGameParameters.ControlMode==1) cFont.PrintAt(x_o[3],y,(char*)"����������:����� ������� � �������",true);
  if (sGameParameters.ControlMode==2) cFont.PrintAt(x_o[3],y,(char*)"����������:�������� � �������",true);
  y+=20;
  cFont.PrintAt(x_o[4],y,(char*)"������� ��������",true);y+=20;
  cFont.PrintAt(x_o[5],y,(char*)"������������",true);y+=20;
  cFont.PrintAt(x_o[6],y,(char*)"����� �� ����",true);y+=20;
  //������ ����� �� �����
  Video_ViewDisplayPage();
  Video_ChangeDisplayPage();
 }
 SaveHiScore(Path);
}
