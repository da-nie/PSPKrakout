#include <math.h>
#include "ccharacter1map.h"
#include "cballmap.h"
#include "cpaddle.h"
#include "basis.h"
#include "csoundcontrol.h"

extern CBallMap cBallMap;
extern CPaddle cPaddle;
extern SGameParameters sGameParameters;
extern CSoundControl cSoundControl;

void CCharacter1Map::Create(char *Path)
{
 int frame_size_x;
 int frame_size_y;
 int blend_color_r;
 int blend_color_g;
 int blend_color_b;
 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssprites\\t1.txt",Path);
 if (GetParameters(FileName,(char*)"FRAME_SIZE_X:",&frame_size_x)==false)
 {
  delete(FileName);
  return;
 }
 if (GetParameters(FileName,(char*)"FRAME_SIZE_Y:",&frame_size_y)==false)
 {
  delete(FileName);
  return;
 }
 if (GetParameters(FileName,(char*)"BLEND_COLOR_R:",&blend_color_r)==false)
 {
  delete(FileName);
  return;
 }
 if (GetParameters(FileName,(char*)"BLEND_COLOR_G:",&blend_color_g)==false)
 {
  delete(FileName);
  return;
 }
 if (GetParameters(FileName,(char*)"BLEND_COLOR_B:",&blend_color_b)==false)
 {
  delete(FileName);
  return;
 }
 if (GetParameters(FileName,(char*)"BIRTH_FRAME_START:",&Birth_Frame_Start)==false)
 {
  delete(FileName);
  return;
 }
 if (GetParameters(FileName,(char*)"BIRTH_FRAME_END:",&Birth_Frame_End)==false)
 {
  delete(FileName);
  return;
 }
 if (GetParameters(FileName,(char*)"LIFE_FRAME_START:",&Life_Frame_Start)==false)
 {
  delete(FileName);
  return;
 }
 if (GetParameters(FileName,(char*)"LIFE_FRAME_END:",&Life_Frame_End)==false)
 {
  delete(FileName);
  return;
 }
 if (GetParameters(FileName,(char*)"DESTRUCTION_FRAME_START:",&Destruction_Frame_Start)==false)
 {
  delete(FileName);
  return;
 }
 if (GetParameters(FileName,(char*)"DESTRUCTION_FRAME_END:",&Destruction_Frame_End)==false)
 {
  delete(FileName);
  return;
 }
 sprintf(FileName,"%ssprites\\t1.tga",Path);
 cObject_Sprite.Load(FileName);
 cObject_Sprite.SetSpriteSize(frame_size_x,frame_size_y);
 cObject_Sprite.SetBlendColor(blend_color_r,blend_color_g,blend_color_b);
 Radius=(float)frame_size_x/2;
 if (Radius<frame_size_y/2) Radius=(float)frame_size_y/2;
 delete(FileName);
}
void CCharacter1Map::Release(void)
{
 cObject_Sprite.Delete();
}
void CCharacter1Map::InitializeToStartLevel(void)
{
 CharacterAmount=0;
 TimerDoStartCharacter=(int)(RND(5000)+100);
 Step=0;
}
void CCharacter1Map::CreateNewCharacter(int x,int y)
{
 if (CharacterAmount>=3 || sGameParameters.EnableCharacter[1]==false) return;
 cSoundControl.Play(SOUND_BIRTH);
 sCharacter1[CharacterAmount].X=(float)x;
 sCharacter1[CharacterAmount].Y=(float)y;
 sCharacter1[CharacterAmount].CurrentImage=Birth_Frame_Start-1;
 sCharacter1[CharacterAmount].ChangeImage=1;
 sCharacter1[CharacterAmount].LifeMode=1;

 sCharacter1[CharacterAmount].TimeOfLife=RND(5000)+500;
 sCharacter1[CharacterAmount].TimeOfMove=0;
 sCharacter1[CharacterAmount].ModeOfMove=0;
 CharacterAmount++;
}
void CCharacter1Map::DestroyCharacter(int character)
{
 sCharacter1[character].CurrentImage=Destruction_Frame_Start-1;
 sCharacter1[character].ChangeImage=1;
 sCharacter1[character].LifeMode=3;
}
void CCharacter1Map::DeleteCharacter(int character)
{
 for(int n=character+1;n<CharacterAmount;n++) sCharacter1[n-1]=sCharacter1[n];
 CharacterAmount--;
}
void CCharacter1Map::AnimateCharacter(void)
{
 Step++;
 Step=Step%2;
 if (Step!=0) return;
 for(int n=0;n<CharacterAmount;n++)
 {
  if (sCharacter1[n].LifeMode==0)
  {
   DeleteCharacter(n);
   n--;
   continue;
  }
  sCharacter1[n].CurrentImage+=sCharacter1[n].ChangeImage;
  if (sCharacter1[n].LifeMode==1)
  {
   if (sCharacter1[n].CurrentImage==Birth_Frame_End-1)
   {
    sCharacter1[n].LifeMode=2;
    sCharacter1[n].CurrentImage=Life_Frame_Start-1;
	continue;
   }
  }
  if (sCharacter1[n].LifeMode==2)
  {
   if (sCharacter1[n].CurrentImage==Life_Frame_End-1) sCharacter1[n].CurrentImage=Life_Frame_Start-1;
   continue;
  }
  if (sCharacter1[n].LifeMode==3)
  {
   if (sCharacter1[n].CurrentImage==Destruction_Frame_End-1) sCharacter1[n].LifeMode=0;
   continue;
  }
 }
}
void CCharacter1Map::Processing(void)
{
 if (TimerDoStartCharacter>0)
 {
  TimerDoStartCharacter--;
  if (TimerDoStartCharacter==0)
  {
   CreateNewCharacter(RND(300)+18+Radius,RND(190-Radius)+Radius+53);
   TimerDoStartCharacter=(int)(RND(5000)+500);
  }
 }
 MoveCharacter();
}
void CCharacter1Map::MoveCharacter(void)
{
 float pd_x=cPaddle.GetX();
 float pd_y=cPaddle.GetY();
 float pd_wd2=cPaddle.GetWidth()/2-4;
 float pd_hd2=cPaddle.GetHeight()/2;
 int pd_face[4];

 for(int n=0;n<CharacterAmount;n++)
 {
  if (sCharacter1[n].LifeMode!=2) continue;
  sCharacter1[n].TimeOfLife--;
  if (sCharacter1[n].TimeOfLife==0)//����� ����� ��������� �����������
  {
   DestroyCharacter(n);
   continue;
  }
  float new_x=sCharacter1[n].X;
  float new_y=sCharacter1[n].Y;
  if (sCharacter1[n].ModeOfMove==1) new_x--;
  if (sCharacter1[n].ModeOfMove==2) new_x++;
  if (sCharacter1[n].ModeOfMove==3) new_y--;
  if (sCharacter1[n].ModeOfMove==4) new_y++;
  //�������� ������������ �� ��������
  int face1=CircleIntercectionToLine(0,52,480,52,new_x,new_y,(float)Radius);//�������
  int face2=CircleIntercectionToLine(0,252,480,252,new_x,new_y,(float)Radius);//������
  int face3=CircleIntercectionToLine(17,0,17,272,new_x,new_y,(float)Radius);//�����
  int face4=CircleIntercectionToLine(458,0,458,272,new_x,new_y,(float)Radius);//������
  if (face1+face2+face3+face4!=0) sCharacter1[n].TimeOfMove=0;
  else
  {
   sCharacter1[n].X=new_x;
   sCharacter1[n].Y=new_y;
  }
  //�������� ������������ � ������
  //����� �����
  pd_face[0]=CircleIntercectionToLine(pd_x-pd_wd2,pd_y-pd_hd2+4,pd_x-pd_wd2,pd_y+pd_hd2-4,sCharacter1[n].X,sCharacter1[n].Y,(float)Radius);
  //������ �����
  pd_face[1]=CircleIntercectionToLine(pd_x+pd_wd2,pd_y-pd_hd2+4,pd_x+pd_wd2,pd_y+pd_hd2-4,sCharacter1[n].X,sCharacter1[n].Y,(float)Radius);
  //������� ��������
  pd_face[2]=CircleIntercectionToCircle(pd_x,pd_y-pd_hd2+4,4,sCharacter1[n].X,sCharacter1[n].Y,(float)Radius);
  //������ ��������
  pd_face[3]=CircleIntercectionToCircle(pd_x,pd_y+pd_hd2-4,4,sCharacter1[n].X,sCharacter1[n].Y,(float)Radius);
  if (pd_face[0]+pd_face[1]+pd_face[2]+pd_face[3]!=0)
  {
   cSoundControl.Play(SOUND_DESTROY);
   DestroyCharacter(n);
  }
  if (sCharacter1[n].TimeOfMove>0) sCharacter1[n].TimeOfMove--;
  if (sCharacter1[n].TimeOfMove==0)
  {
   sCharacter1[n].TimeOfMove=RND(200)+10;
   sCharacter1[n].ModeOfMove=RND(5);
  }
 }
}
void CCharacter1Map::PutCharacter(void)
{
 for(int n=0;n<CharacterAmount;n++)
 {
  cObject_Sprite.SetSpriteCoord(0,sCharacter1[n].CurrentImage);
  cObject_Sprite.Put((int)sCharacter1[n].X-(int)Radius,(int)sCharacter1[n].Y-(int)Radius);
 }
}
void CCharacter1Map::DestroyAllCharacter(void)
{
 for(int n=0;n<CharacterAmount;n++)
 {
  if (sCharacter1[n].LifeMode!=2 && sCharacter1[n].LifeMode!=1) continue;
  DestroyCharacter(n);
 }
}
void CCharacter1Map::ImpactCharacterToBall(int ball_x,int ball_y,int ball_radius,float &vx,float &vy)
{
 int n;
 //��������� ���
 for(n=0;n<CharacterAmount;n++)
 {
  if (sCharacter1[n].LifeMode!=2) continue;
  if (CircleIntercectionToCircle(ball_x,ball_y,ball_radius,sCharacter1[n].X,sCharacter1[n].Y,Radius)==true)
  {
   cSoundControl.Play(SOUND_DESTROY);
   DestroyCharacter(n);
   float angle=RNDf(360.0);
   vy=sin(M_PI/180.0*angle);
   vx=cos(M_PI/180.0*angle);
  }
 }
}
bool CCharacter1Map::ImpactCharacterToMissile(int xleft,int xright,int yup,int ydown)
{
 int n;
 for(n=0;n<CharacterAmount;n++)
 {
  if (sCharacter1[n].LifeMode!=2) continue;
  int pd_face1=CircleIntercectionToLine((float)xleft,(float)yup,(float)xright,(float)yup,sCharacter1[n].X,sCharacter1[n].Y,(float)Radius);//�������
  int pd_face2=CircleIntercectionToLine((float)xleft,(float)ydown,(float)xright,(float)ydown,sCharacter1[n].X,sCharacter1[n].Y,(float)Radius);//������
  int pd_face3=CircleIntercectionToLine((float)xleft,(float)yup,(float)xleft,(float)ydown,sCharacter1[n].X,sCharacter1[n].Y,(float)Radius);//�����
  int pd_face4=CircleIntercectionToLine((float)xright,(float)yup,(float)xright,(float)ydown,sCharacter1[n].X,sCharacter1[n].Y,(float)Radius);//������
  if (pd_face1+pd_face2+pd_face3+pd_face4!=0)
  {
   cSoundControl.Play(SOUND_DESTROY);
   DestroyCharacter(n);
  }
 }
 return(false);//������ �� ��������
}
