#include <math.h>
#include "ccharacter9map.h"
#include "cballmap.h"
#include "cpaddle.h"
#include "basis.h"
#include "csoundcontrol.h"

extern CBallMap cBallMap;
extern CPaddle cPaddle;
extern SGameParameters sGameParameters;
extern CSoundControl cSoundControl;

void CCharacter9Map::Create(char *Path)
{
 int frame_size_x;
 int frame_size_y;
 int blend_color_r;
 int blend_color_g;
 int blend_color_b;
 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssprites\\t9.txt",Path);
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
 sprintf(FileName,"%ssprites\\t9.tga",Path);
 cObject_Sprite.Load(FileName);
 cObject_Sprite.SetSpriteSize(frame_size_x,frame_size_y);
 cObject_Sprite.SetBlendColor(blend_color_r,blend_color_g,blend_color_b);
 Radius=(float)frame_size_x/2;
 if (Radius<frame_size_y/2) Radius=(float)frame_size_y/2;
 delete(FileName);
}
void CCharacter9Map::Release(void)
{
 cObject_Sprite.Delete();
}
void CCharacter9Map::InitializeToStartLevel(void)
{
 CharacterAmount=0;
 TimerDoStartCharacter=RND(5000)+100;
 Step=0;
}
void CCharacter9Map::CreateNewCharacter(int x,int y)
{
 if (CharacterAmount>=3 || sGameParameters.EnableCharacter[9]==false) return;
 cSoundControl.Play(SOUND_BIRTH);
 sCharacter9[CharacterAmount].X=(float)x;
 sCharacter9[CharacterAmount].Y=(float)y;
 sCharacter9[CharacterAmount].CurrentImage=Birth_Frame_Start-1;
 sCharacter9[CharacterAmount].ChangeImage=1;
 sCharacter9[CharacterAmount].LifeMode=1;

 sCharacter9[CharacterAmount].TimeOfLife=RND(5000)+500;
 sCharacter9[CharacterAmount].TimeOfMove=0;
 sCharacter9[CharacterAmount].ModeOfMove=0;
 CharacterAmount++;
}
void CCharacter9Map::DestroyCharacter(int character)
{
 sCharacter9[character].CurrentImage=Destruction_Frame_Start-1;
 sCharacter9[character].ChangeImage=1;
 sCharacter9[character].LifeMode=3;
}
void CCharacter9Map::DeleteCharacter(int character)
{
 for(int n=character+1;n<CharacterAmount;n++) sCharacter9[n-1]=sCharacter9[n];
 CharacterAmount--;
}
void CCharacter9Map::AnimateCharacter(void)
{
 Step++;
 Step=Step%2;
 if (Step!=0) return;
 for(int n=0;n<CharacterAmount;n++)
 {
  if (sCharacter9[n].LifeMode==0)
  {
   DeleteCharacter(n);
   n--;
   continue;
  }
  sCharacter9[n].CurrentImage+=sCharacter9[n].ChangeImage;
  if (sCharacter9[n].LifeMode==1)
  {
   if (sCharacter9[n].CurrentImage==Birth_Frame_End-1)
   {
    sCharacter9[n].LifeMode=2;
    sCharacter9[n].CurrentImage=Life_Frame_Start-1;
	continue;
   }
  }
  if (sCharacter9[n].LifeMode==2)
  {
   if (sCharacter9[n].CurrentImage==Life_Frame_End-1) sCharacter9[n].CurrentImage=Life_Frame_Start-1;
   continue;
  }
  if (sCharacter9[n].LifeMode==3)
  {
   if (sCharacter9[n].CurrentImage==Destruction_Frame_End-1) sCharacter9[n].LifeMode=0;
   continue;
  }
 }
}
void CCharacter9Map::Processing(void)
{
 if (TimerDoStartCharacter>0)
 {
  TimerDoStartCharacter--;
  if (TimerDoStartCharacter==0)
  {
   CreateNewCharacter(RND(300)+18+Radius,RND(190-Radius)+Radius+53);
   TimerDoStartCharacter=RND(5000)+500;
  }
 }
 MoveCharacter();
}
void CCharacter9Map::MoveCharacter(void)
{
 float pd_x=cPaddle.GetX();
 float pd_y=cPaddle.GetY();
 float pd_wd2=cPaddle.GetWidth()/2-4;
 float pd_hd2=cPaddle.GetHeight()/2;
 int pd_face[4];

 for(int n=0;n<CharacterAmount;n++)
 {
  if (sCharacter9[n].LifeMode!=2) continue;
  sCharacter9[n].TimeOfLife--;
  if (sCharacter9[n].TimeOfLife==0)//����� ����� ��������� �����������
  {
   DestroyCharacter(n);
   continue;
  }
  float new_x=sCharacter9[n].X;
  float new_y=sCharacter9[n].Y;
  if (sCharacter9[n].ModeOfMove==1)
  {
   new_x--;
   new_y--;
  }
  if (sCharacter9[n].ModeOfMove==2)
  {
   new_x--;
   new_y++;
  }
  if (sCharacter9[n].ModeOfMove==3)
  {
   new_x++;
   new_y--;
  }
  if (sCharacter9[n].ModeOfMove==4)
  {
   new_x++;
   new_y++;
  }
  //�������� ������������ �� ��������
  int face1=CircleIntercectionToLine(0,52,480,52,new_x,new_y,(float)Radius);//�������
  int face2=CircleIntercectionToLine(0,252,480,252,new_x,new_y,(float)Radius);//������
  int face3=CircleIntercectionToLine(17,0,17,272,new_x,new_y,(float)Radius);//�����
  int face4=CircleIntercectionToLine(458,0,458,272,new_x,new_y,(float)Radius);//������
  if (face1+face2+face3+face4!=0) sCharacter9[n].TimeOfMove=0;
  else
  {
   sCharacter9[n].X=new_x;
   sCharacter9[n].Y=new_y;
  }
  //�������� ������������ � ������
  //����� �����
  pd_face[0]=CircleIntercectionToLine(pd_x-pd_wd2,pd_y-pd_hd2+4,pd_x-pd_wd2,pd_y+pd_hd2-4,sCharacter9[n].X,sCharacter9[n].Y,(float)Radius);
  //������ �����
  pd_face[1]=CircleIntercectionToLine(pd_x+pd_wd2,pd_y-pd_hd2+4,pd_x+pd_wd2,pd_y+pd_hd2-4,sCharacter9[n].X,sCharacter9[n].Y,(float)Radius);
  //������� ��������
  pd_face[2]=CircleIntercectionToCircle(pd_x,pd_y-pd_hd2+4,4,sCharacter9[n].X,sCharacter9[n].Y,(float)Radius);
  //������ ��������
  pd_face[3]=CircleIntercectionToCircle(pd_x,pd_y+pd_hd2-4,4,sCharacter9[n].X,sCharacter9[n].Y,(float)Radius);
  if (pd_face[0]+pd_face[1]+pd_face[2]+pd_face[3]!=0)
  {
   cSoundControl.Play(SOUND_DESTROY);
   DestroyCharacter(n);
  }
  if (sCharacter9[n].TimeOfMove>0) sCharacter9[n].TimeOfMove--;
  if (sCharacter9[n].TimeOfMove==0)
  {
   sCharacter9[n].TimeOfMove=RND(200)+10;
   sCharacter9[n].ModeOfMove=RND(5);
  }
 }
}
void CCharacter9Map::PutCharacter(void)
{
 for(int n=0;n<CharacterAmount;n++)
 {
  cObject_Sprite.SetSpriteCoord(0,sCharacter9[n].CurrentImage);
  cObject_Sprite.Put((int)sCharacter9[n].X-(int)Radius,(int)sCharacter9[n].Y-(int)Radius);
 }
}
void CCharacter9Map::DestroyAllCharacter(void)
{
 for(int n=0;n<CharacterAmount;n++)
 {
  if (sCharacter9[n].LifeMode!=2 && sCharacter9[n].LifeMode!=1) continue;
  DestroyCharacter(n);
 }
}
void CCharacter9Map::ImpactCharacterToBall(int ball_x,int ball_y,int ball_radius,float &vx,float &vy)
{
 int n;
 //��������� ���
 for(n=0;n<CharacterAmount;n++)
 {
  if (sCharacter9[n].LifeMode!=2) continue;
  if (CircleIntercectionToCircle(ball_x,ball_y,ball_radius,sCharacter9[n].X,sCharacter9[n].Y,Radius)==true)
  {
   cSoundControl.Play(SOUND_DESTROY);
   DestroyCharacter(n);
   float angle=RNDf(360.0f);
   vy=sin(M_PI/180.0*angle);
   vx=cos(M_PI/180.0*angle);
  }
 }
}
bool CCharacter9Map::ImpactCharacterToMissile(int xleft,int xright,int yup,int ydown)
{
 int n;
 for(n=0;n<CharacterAmount;n++)
 {
  if (sCharacter9[n].LifeMode!=2) continue;
  int pd_face1=CircleIntercectionToLine((float)xleft,(float)yup,(float)xright,(float)yup,sCharacter9[n].X,sCharacter9[n].Y,(float)Radius);//�������
  int pd_face2=CircleIntercectionToLine((float)xleft,(float)ydown,(float)xright,(float)ydown,sCharacter9[n].X,sCharacter9[n].Y,(float)Radius);//������
  int pd_face3=CircleIntercectionToLine((float)xleft,(float)yup,(float)xleft,(float)ydown,sCharacter9[n].X,sCharacter9[n].Y,(float)Radius);//�����
  int pd_face4=CircleIntercectionToLine((float)xright,(float)yup,(float)xright,(float)ydown,sCharacter9[n].X,sCharacter9[n].Y,(float)Radius);//������
  if (pd_face1+pd_face2+pd_face3+pd_face4!=0)
  {
   cSoundControl.Play(SOUND_DESTROY);
   DestroyCharacter(n);
  }
 }
 return(false);//������ �� ��������
}
