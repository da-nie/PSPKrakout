#include <math.h>
#include "ccharacter4map.h"
#include "cballmap.h"
#include "cpaddle.h"
#include "basis.h"
#include "csoundcontrol.h"

extern CBallMap cBallMap;
extern CPaddle cPaddle;
extern SGameParameters sGameParameters;
extern CSoundControl cSoundControl;

void CCharacter4Map::Create(char *Path)
{
 int frame_size_x;
 int frame_size_y;
 int blend_color_r;
 int blend_color_g;
 int blend_color_b;
 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssprites\\t4.txt",Path);
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
 sprintf(FileName,"%ssprites\\t4.tga",Path);
 cObject_Sprite.Load(FileName);
 cObject_Sprite.SetSpriteSize(frame_size_x,frame_size_y);
 cObject_Sprite.SetBlendColor(blend_color_r,blend_color_g,blend_color_b);
 Radius=(float)frame_size_x/2;
 if (Radius<frame_size_y/2) Radius=(float)frame_size_y/2;
 delete(FileName);
}
void CCharacter4Map::Release(void)
{
 cObject_Sprite.Delete();
}
void CCharacter4Map::InitializeToStartLevel(void)
{
 CharacterAmount=0;
 TimerDoStartCharacter=RND(5000)+100;
 Step=0;
}
void CCharacter4Map::CreateNewCharacter(int x,int y)
{
 if (CharacterAmount>=3 || sGameParameters.EnableCharacter[4]==false) return;
 cSoundControl.Play(SOUND_BIRTH);
 sCharacter4[CharacterAmount].X=(float)x;
 sCharacter4[CharacterAmount].Y=(float)y;
 sCharacter4[CharacterAmount].CurrentImage=Birth_Frame_Start-1;
 sCharacter4[CharacterAmount].ChangeImage=1;
 sCharacter4[CharacterAmount].LifeMode=1;

 sCharacter4[CharacterAmount].TimeOfLife=RND(5000)+500;
 sCharacter4[CharacterAmount].ModeOfMove=0;
 CharacterAmount++;
}
void CCharacter4Map::DestroyCharacter(int character)
{
 sCharacter4[character].CurrentImage=Destruction_Frame_Start-1;
 sCharacter4[character].ChangeImage=1;
 sCharacter4[character].LifeMode=3;
}
void CCharacter4Map::DeleteCharacter(int character)
{
 for(int n=character+1;n<CharacterAmount;n++) sCharacter4[n-1]=sCharacter4[n];
 CharacterAmount--;
}
void CCharacter4Map::AnimateCharacter(void)
{
 Step++;
 Step=Step%2;
 if (Step!=0) return;
 for(int n=0;n<CharacterAmount;n++)
 {
  if (sCharacter4[n].LifeMode==0)
  {
   DeleteCharacter(n);
   n--;
   continue;
  }
  sCharacter4[n].CurrentImage+=sCharacter4[n].ChangeImage;
  if (sCharacter4[n].LifeMode==1)
  {
   if (sCharacter4[n].CurrentImage==Birth_Frame_End-1)
   {
    sCharacter4[n].LifeMode=2;
    sCharacter4[n].CurrentImage=Life_Frame_Start-1;
	continue;
   }
  }
  if (sCharacter4[n].LifeMode==2)
  {
   if (sCharacter4[n].CurrentImage==Life_Frame_End-1) sCharacter4[n].ChangeImage=-1;
   if (sCharacter4[n].CurrentImage==Life_Frame_Start-1) sCharacter4[n].ChangeImage=1;
   continue;
  }
  if (sCharacter4[n].LifeMode==3)
  {
   if (sCharacter4[n].CurrentImage==Destruction_Frame_End-1) sCharacter4[n].LifeMode=0;
   continue;
  }
 }
}
void CCharacter4Map::Processing(void)
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
void CCharacter4Map::MoveCharacter(void)
{
 float pd_x=cPaddle.GetX();
 float pd_y=cPaddle.GetY();
 float pd_wd2=cPaddle.GetWidth()/2-4;
 float pd_hd2=cPaddle.GetHeight()/2;
 int pd_face[4];

 for(int n=0;n<CharacterAmount;n++)
 {
  if (sCharacter4[n].LifeMode!=2) continue;
  sCharacter4[n].TimeOfLife--;
  if (sCharacter4[n].TimeOfLife==0)//время жизни персонажа закончилось
  {
   DestroyCharacter(n);
   continue;
  }
  float new_x=sCharacter4[n].X;
  float new_y=sCharacter4[n].Y;
  if (sCharacter4[n].ModeOfMove==0)//ждём, пока мяч подлетит к нам на некоторое расстояние
  {
   int selectball=cBallMap.GetNearestBallNumber(new_x,new_y);
   if (selectball>=0)
   {
    float bx,by;
    cBallMap.GetBallCoord(selectball,bx,by);
	float dist2=(bx-new_x)*(bx-new_x)+(by-new_y)*(by-new_y);
	if (dist2<150*150) sCharacter4[n].ModeOfMove=1;
    continue;
   }
  }
  if (sCharacter4[n].ModeOfMove==1)//атакуем мяч
  {
   int selectball=cBallMap.GetNearestBallNumber(new_x,new_y);
   if (selectball>=0)
   {
    float bx,by;
    cBallMap.GetBallCoord(selectball,bx,by);
    if (new_x>bx) new_x-=(float)1.5;
    if (new_x<bx) new_x+=(float)1.5;
    if (new_y>by) new_y-=(float)1.5;
    if (new_y<by) new_y+=(float)1.5;
   }
  }
  //проверка столкновения со стенками
  int face1=CircleIntercectionToLine(0,52,480,52,new_x,new_y,(float)Radius);//верхняя
  int face2=CircleIntercectionToLine(0,252,480,252,new_x,new_y,(float)Radius);//нижняя
  int face3=CircleIntercectionToLine(17,0,17,272,new_x,new_y,(float)Radius);//левая
  int face4=CircleIntercectionToLine(458,0,458,272,new_x,new_y,(float)Radius);//правая
  if (face1+face2+face3+face4==0)
  {
   sCharacter4[n].X=new_x;
   sCharacter4[n].Y=new_y;
  }
  //проверка столкновения с биткой
  //левая грань
  pd_face[0]=CircleIntercectionToLine(pd_x-pd_wd2,pd_y-pd_hd2+4,pd_x-pd_wd2,pd_y+pd_hd2-4,sCharacter4[n].X,sCharacter4[n].Y,(float)Radius);
  //правая грань
  pd_face[1]=CircleIntercectionToLine(pd_x+pd_wd2,pd_y-pd_hd2+4,pd_x+pd_wd2,pd_y+pd_hd2-4,sCharacter4[n].X,sCharacter4[n].Y,(float)Radius);
  //верхний полукруг
  pd_face[2]=CircleIntercectionToCircle(pd_x,pd_y-pd_hd2+4,4,sCharacter4[n].X,sCharacter4[n].Y,(float)Radius);
  //нижний полукруг
  pd_face[3]=CircleIntercectionToCircle(pd_x,pd_y+pd_hd2-4,4,sCharacter4[n].X,sCharacter4[n].Y,(float)Radius);
  if (pd_face[0]+pd_face[1]+pd_face[2]+pd_face[3]!=0)
  {
   cSoundControl.Play(SOUND_DESTROY);
   DestroyCharacter(n);
  }
 }
}
void CCharacter4Map::PutCharacter(void)
{
 for(int n=0;n<CharacterAmount;n++)
 {
  cObject_Sprite.SetSpriteCoord(0,sCharacter4[n].CurrentImage);
  cObject_Sprite.Put((int)sCharacter4[n].X-(int)Radius,(int)sCharacter4[n].Y-(int)Radius);
 }
}
void CCharacter4Map::DestroyAllCharacter(void)
{
 for(int n=0;n<CharacterAmount;n++)
 {
  if (sCharacter4[n].LifeMode!=2 && sCharacter4[n].LifeMode!=1) continue;
  DestroyCharacter(n);
 }
}
void CCharacter4Map::ImpactCharacterToBall(int ball_x,int ball_y,int ball_radius,float &vx,float &vy)
{
 int n;
 //отклоняем мяч
 for(n=0;n<CharacterAmount;n++)
 {
  if (sCharacter4[n].LifeMode!=2) continue;
  if (CircleIntercectionToCircle(ball_x,ball_y,ball_radius,sCharacter4[n].X,sCharacter4[n].Y,Radius)==true)
  {
   cSoundControl.Play(SOUND_DESTROY);
   DestroyCharacter(n);
   float angle=RNDf(80.0f)-40.0;
   vy=sin(M_PI/180.0*angle);
   vx=cos(M_PI/180.0*angle);
  }
 }
}
bool CCharacter4Map::ImpactCharacterToMissile(int xleft,int xright,int yup,int ydown)
{
 int n;
 for(n=0;n<CharacterAmount;n++)
 {
  if (sCharacter4[n].LifeMode!=2) continue;
  int pd_face1=CircleIntercectionToLine((float)xleft,(float)yup,(float)xright,(float)yup,sCharacter4[n].X,sCharacter4[n].Y,(float)Radius);//верхняя
  int pd_face2=CircleIntercectionToLine((float)xleft,(float)ydown,(float)xright,(float)ydown,sCharacter4[n].X,sCharacter4[n].Y,(float)Radius);//нижняя
  int pd_face3=CircleIntercectionToLine((float)xleft,(float)yup,(float)xleft,(float)ydown,sCharacter4[n].X,sCharacter4[n].Y,(float)Radius);//левая
  int pd_face4=CircleIntercectionToLine((float)xright,(float)yup,(float)xright,(float)ydown,sCharacter4[n].X,sCharacter4[n].Y,(float)Radius);//правая
  if (pd_face1+pd_face2+pd_face3+pd_face4!=0)
  {
   cSoundControl.Play(SOUND_DESTROY);
   DestroyCharacter(n);
  }
 }
 return(false);//ракету не взрываем
}
