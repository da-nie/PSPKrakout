#include <math.h>
#include "ccharacter7map.h"
#include "cballmap.h"
#include "cpaddle.h"
#include "basis.h"
#include "csoundcontrol.h"

extern CBallMap cBallMap;
extern CPaddle cPaddle;
extern SGameParameters sGameParameters;
extern CSoundControl cSoundControl;

void CCharacter7Map::Create(char *Path)
{
 int frame_size_x;
 int frame_size_y;
 int blend_color_r;
 int blend_color_g;
 int blend_color_b;
 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssprites\\t7.txt",Path);
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
 sprintf(FileName,"%ssprites\\t7.tga",Path);
 cObject_Sprite.Load(FileName);
 cObject_Sprite.SetSpriteSize(frame_size_x,frame_size_y);
 cObject_Sprite.SetBlendColor(blend_color_r,blend_color_g,blend_color_b);
 Radius=(float)frame_size_x/2;
 if (Radius<frame_size_y/2) Radius=(float)frame_size_y/2;
 delete(FileName);
}
void CCharacter7Map::Release(void)
{
 cObject_Sprite.Delete();
}
void CCharacter7Map::InitializeToStartLevel(void)
{
 CharacterAmount=0;
 TimerDoStartCharacter=RND(5000)+100;
 Step=0;
}
void CCharacter7Map::CreateNewCharacter(int x,int y)
{
 if (CharacterAmount>=2 || sGameParameters.EnableCharacter[7]==false) return;
 cSoundControl.Play(SOUND_BIRTH);
 for(int n=0;n<6;n++)
 {
  sCharacter7[CharacterAmount].X[n]=(float)x;
  sCharacter7[CharacterAmount].Y[n]=(float)y+Radius*2*n;
  sCharacter7[CharacterAmount].LifeMode[n]=1;
  sCharacter7[CharacterAmount].CurrentImage[n]=(Birth_Frame_Start-1);
  sCharacter7[CharacterAmount].ChangeImage[n]=1;
  sCharacter7[CharacterAmount].ModeOfMove[n]=0;
  sCharacter7[CharacterAmount].TimeOfMove[n]=0;
 }
 sCharacter7[CharacterAmount].TimeOfLife=RND(10000)+1000;
 CharacterAmount++;
}
void CCharacter7Map::DestroyCharacter(int character,int segment)
{
 sCharacter7[character].CurrentImage[segment]=Destruction_Frame_Start-1;
 sCharacter7[character].ChangeImage[segment]=1;
 sCharacter7[character].LifeMode[segment]=3;
}
void CCharacter7Map::DeleteCharacter(int character)
{
 for(int n=character+1;n<CharacterAmount;n++) sCharacter7[n-1]=sCharacter7[n];
 CharacterAmount--;
}
void CCharacter7Map::AnimateCharacter(void)
{
 int n,m;
 Step++;
 Step=Step%2;
 if (Step!=0) return;
 for(n=0;n<CharacterAmount;n++)
 {
  int segment=6;
  for(m=0;m<6;m++)
  {
   if (sCharacter7[n].LifeMode[m]==0) segment--;
  }
  if (segment==0)//все сегменты змейки разрушены
  {
   DeleteCharacter(n);
   n--;
   continue;
  }
  for(m=0;m<6;m++)
  {
   if (sCharacter7[n].LifeMode[m]==0) continue;
   sCharacter7[n].CurrentImage[m]+=sCharacter7[n].ChangeImage[m];
   if (sCharacter7[n].LifeMode[m]==1)
   {
    if (sCharacter7[n].CurrentImage[m]==Birth_Frame_End-1)
    {
     sCharacter7[n].LifeMode[m]=2;
     sCharacter7[n].CurrentImage[m]=Life_Frame_Start-1+RND(Life_Frame_End-Life_Frame_Start);
     continue;
    }
   }
   if (sCharacter7[n].LifeMode[m]==2)
   {
    if (sCharacter7[n].CurrentImage[m]==Life_Frame_End-1) sCharacter7[n].CurrentImage[m]=Life_Frame_Start-1;
    continue;
   }
   if (sCharacter7[n].LifeMode[m]==3)
   {
    if (sCharacter7[n].CurrentImage[m]==Destruction_Frame_End-1) sCharacter7[n].LifeMode[m]=0;
    continue;
   }
  }
 }
}
void CCharacter7Map::Processing(void)
{
 if (TimerDoStartCharacter>0)
 {
  TimerDoStartCharacter--;
  if (TimerDoStartCharacter==0)
  {
   CreateNewCharacter(RND(100)+18+Radius,240-Radius*12);
   TimerDoStartCharacter=RND(5000)+500;
  }
 }
 MoveCharacter();
}
void CCharacter7Map::MoveCharacter(void)
{
 int n,m;
 //параметры битки
 float pd_x=cPaddle.GetX();
 float pd_y=cPaddle.GetY();
 float pd_wd2=cPaddle.GetWidth()/2-4;
 float pd_hd2=cPaddle.GetHeight()/2;
 int pd_face[4];

 for(n=0;n<CharacterAmount;n++)
 {
  int segment=6;
  for(m=0;m<6;m++)
  {
   if (sCharacter7[n].LifeMode[m]!=2) segment--;
  }
  if (segment==0) continue;
  sCharacter7[n].TimeOfLife--;
  if (sCharacter7[n].TimeOfLife==0)//время жизни персонажа закончилось
  {
   for(m=0;m<6;m++)
   {
    if (sCharacter7[n].LifeMode[m]!=2) continue;
    DestroyCharacter(n,m);
   }
   continue;
  }
  for(m=0;m<6;m++)
  {
   float new_x=sCharacter7[n].X[m];
   float new_y=sCharacter7[n].Y[m];
   //прямое направление
   if (sCharacter7[n].ModeOfMove[m]==0) new_y--;//движемся вверх
   if (sCharacter7[n].ModeOfMove[m]==1) new_x++;//движемся вправо
   if (sCharacter7[n].ModeOfMove[m]==2) new_y++;//движемся вниз
   if (sCharacter7[n].ModeOfMove[m]==3) new_x++;//движемся вправо
   //обратное направление
   if (sCharacter7[n].ModeOfMove[m]==5) new_y--;//движемся вверх
   if (sCharacter7[n].ModeOfMove[m]==6) new_x--;//движемся вправо
   if (sCharacter7[n].ModeOfMove[m]==7) new_y++;//движемся вниз
   if (sCharacter7[n].ModeOfMove[m]==8) new_x--;//движемся вправо
   //проверка столкновения со стенками
   int face1=CircleIntercectionToLine(0,52,480,52,new_x,new_y,(float)Radius);//верхняя
   int face2=CircleIntercectionToLine(0,252,480,252,new_x,new_y,(float)Radius);//нижняя
   int face3=CircleIntercectionToLine(17,0,17,272,new_x,new_y,(float)Radius);//левая
   int face4=CircleIntercectionToLine(458,0,458,272,new_x,new_y,(float)Radius);//правая
   if (face1+face2+face3+face4!=0)//столкнулись со стенками и нужно менять режим движения
   {
    sCharacter7[n].ModeOfMove[m]++;
    if (sCharacter7[n].ModeOfMove[m]==4) sCharacter7[n].ModeOfMove[m]=0;
    if (sCharacter7[n].ModeOfMove[m]==9) sCharacter7[n].ModeOfMove[m]=5;
    if (sCharacter7[n].ModeOfMove[m]==1) sCharacter7[n].TimeOfMove[m]=80;
    if (sCharacter7[n].ModeOfMove[m]==3) sCharacter7[n].TimeOfMove[m]=80;
    if (sCharacter7[n].ModeOfMove[m]==6) sCharacter7[n].TimeOfMove[m]=80;
    if (sCharacter7[n].ModeOfMove[m]==8) sCharacter7[n].TimeOfMove[m]=80;
    if (face4!=0) sCharacter7[n].ModeOfMove[m]=5;//меняем направление на обратное
    if (face3!=0) sCharacter7[n].ModeOfMove[m]=0;
   }
   if (sCharacter7[n].TimeOfMove[m]>0) sCharacter7[n].TimeOfMove[m]--;
   if (sCharacter7[n].TimeOfMove[m]==0)
   {
    if (sCharacter7[n].ModeOfMove[m]==1) sCharacter7[n].ModeOfMove[m]++;
    if (sCharacter7[n].ModeOfMove[m]==3) sCharacter7[n].ModeOfMove[m]++;
    if (sCharacter7[n].ModeOfMove[m]==6) sCharacter7[n].ModeOfMove[m]++;
    if (sCharacter7[n].ModeOfMove[m]==8) sCharacter7[n].ModeOfMove[m]++;
    if (sCharacter7[n].ModeOfMove[m]==4) sCharacter7[n].ModeOfMove[m]=0;
    if (sCharacter7[n].ModeOfMove[m]==9) sCharacter7[n].ModeOfMove[m]=5;
   }
   if (face1+face2+face3+face4==0)
   {
    sCharacter7[n].X[m]=new_x;
    sCharacter7[n].Y[m]=new_y;
   }
   if (sCharacter7[n].LifeMode[m]!=2) continue;
   //проверка столкновения с биткой
   //левая грань
   pd_face[0]=CircleIntercectionToLine(pd_x-pd_wd2,pd_y-pd_hd2+4,pd_x-pd_wd2,pd_y+pd_hd2-4,sCharacter7[n].X[m],sCharacter7[n].Y[m],(float)Radius);
   //правая грань
   pd_face[1]=CircleIntercectionToLine(pd_x+pd_wd2,pd_y-pd_hd2+4,pd_x+pd_wd2,pd_y+pd_hd2-4,sCharacter7[n].X[m],sCharacter7[n].Y[m],(float)Radius);
   //верхний полукруг
   pd_face[2]=CircleIntercectionToCircle(pd_x,pd_y-pd_hd2+4,4,sCharacter7[n].X[m],sCharacter7[n].Y[m],(float)Radius);
   //нижний полукруг
   pd_face[3]=CircleIntercectionToCircle(pd_x,pd_y+pd_hd2-4,4,sCharacter7[n].X[m],sCharacter7[n].Y[m],(float)Radius);
   if (pd_face[0]+pd_face[1]+pd_face[2]+pd_face[3]!=0)
   {
    cSoundControl.Play(SOUND_DESTROY);
    DestroyCharacter(n,m);
   }
  }
 }
}
void CCharacter7Map::PutCharacter(void)
{
 for(int n=0;n<CharacterAmount;n++)
 {
  for(int m=0;m<6;m++)
  {
   if (sCharacter7[n].LifeMode[m]==0) continue;
   cObject_Sprite.SetSpriteCoord(0,sCharacter7[n].CurrentImage[m]);
   cObject_Sprite.Put((int)sCharacter7[n].X[m]-(int)Radius,(int)sCharacter7[n].Y[m]-(int)Radius);
  }
 }
}
void CCharacter7Map::DestroyAllCharacter(void)
{
 for(int n=0;n<CharacterAmount;n++)
 {
  for(int m=0;m<6;m++)
  {
   if (sCharacter7[n].LifeMode[m]!=2 && sCharacter7[n].LifeMode[m]!=1) continue;
   DestroyCharacter(n,m);
  }
 }
}
void CCharacter7Map::ImpactCharacterToBall(int ball_x,int ball_y,int ball_radius,float &vx,float &vy)
{
 int n,m;
 //отклоняем мяч
 for(n=0;n<CharacterAmount;n++)
 {
  for(m=0;m<6;m++)
  {
   if (sCharacter7[n].LifeMode[m]!=2) continue;
   if (CircleIntercectionToCircle(ball_x,ball_y,ball_radius,sCharacter7[n].X[m],sCharacter7[n].Y[m],Radius)==true)
   {
    cSoundControl.Play(SOUND_DESTROY);
    DestroyCharacter(n,m);
    float angle=RNDf(360.0f);
    vy=sin(M_PI/180.0*angle);
    vx=cos(M_PI/180.0*angle);
   }
  }
 }
}
bool CCharacter7Map::ImpactCharacterToMissile(int xleft,int xright,int yup,int ydown)
{
 int n,m;
 for(n=0;n<CharacterAmount;n++)
 {
  for(m=0;m<6;m++)
  {
   if (sCharacter7[n].LifeMode[m]!=2) continue;
   int pd_face1=CircleIntercectionToLine((float)xleft,(float)yup,(float)xright,(float)yup,sCharacter7[n].X[m],sCharacter7[n].Y[m],(float)Radius);//верхняя
   int pd_face2=CircleIntercectionToLine((float)xleft,(float)ydown,(float)xright,(float)ydown,sCharacter7[n].X[m],sCharacter7[n].Y[m],(float)Radius);//нижняя
   int pd_face3=CircleIntercectionToLine((float)xleft,(float)yup,(float)xleft,(float)ydown,sCharacter7[n].X[m],sCharacter7[n].Y[m],(float)Radius);//левая
   int pd_face4=CircleIntercectionToLine((float)xright,(float)yup,(float)xright,(float)ydown,sCharacter7[n].X[m],sCharacter7[n].Y[m],(float)Radius);//правая
   if (pd_face1+pd_face2+pd_face3+pd_face4!=0)
   {
    cSoundControl.Play(SOUND_DESTROY);
    DestroyCharacter(n,m);
   }
  }
 }
 return(false);//ракету не взрываем
}
