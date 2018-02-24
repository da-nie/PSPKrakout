#include <math.h>
#include "ccharacter8map.h"
#include "cballmap.h"
#include "cblockmap.h"
#include "cpaddle.h"
#include "basis.h"
#include "csoundcontrol.h"

extern CBallMap cBallMap;
extern CBlockMap cBlockMap;
extern CPaddle cPaddle;
extern SGameParameters sGameParameters;
extern CSoundControl cSoundControl;

void CCharacter8Map::Create(char *Path)
{
 int frame_size_x;
 int frame_size_y;
 int blend_color_r;
 int blend_color_g;
 int blend_color_b;
 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssprites\\t8.txt",Path);
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
 sprintf(FileName,"%ssprites\\t8.tga",Path);
 cObject_Sprite.Load(FileName);
 cObject_Sprite.SetSpriteSize(frame_size_x,frame_size_y);
 cObject_Sprite.SetBlendColor(blend_color_r,blend_color_g,blend_color_b);
 Radius=(float)frame_size_x/2;
 if (Radius<frame_size_y/2) Radius=(float)frame_size_y/2;
 delete(FileName);
}
void CCharacter8Map::Release(void)
{
 cObject_Sprite.Delete();
}
void CCharacter8Map::InitializeToStartLevel(void)
{
 CharacterAmount=0;
 TimerDoStartCharacter=RND(5000)+100;
 Step=0;
}
void CCharacter8Map::CreateNewCharacter(int x,int y)
{
 if (CharacterAmount>=3 || sGameParameters.EnableCharacter[8]==false) return;
 cSoundControl.Play(SOUND_BIRTH);
 sCharacter8[CharacterAmount].X=(float)x;
 sCharacter8[CharacterAmount].Y=(float)y;
 sCharacter8[CharacterAmount].CurrentImage=Birth_Frame_Start-1;
 sCharacter8[CharacterAmount].ChangeImage=1;
 sCharacter8[CharacterAmount].LifeMode=1;

 sCharacter8[CharacterAmount].TimeOfLife=RND(5000)+500;
 sCharacter8[CharacterAmount].TimeOfMove=0;
 sCharacter8[CharacterAmount].ModeOfMove=0;
 CharacterAmount++;
}
void CCharacter8Map::DestroyCharacter(int character)
{
 sCharacter8[character].CurrentImage=Destruction_Frame_Start-1;
 sCharacter8[character].ChangeImage=1;
 sCharacter8[character].LifeMode=3;
}
void CCharacter8Map::DeleteCharacter(int character)
{
 for(int n=character+1;n<CharacterAmount;n++) sCharacter8[n-1]=sCharacter8[n];
 CharacterAmount--;
}
void CCharacter8Map::AnimateCharacter(void)
{
 Step++;
 Step=Step%2;
 if (Step!=0) return;
 for(int n=0;n<CharacterAmount;n++)
 {
  if (sCharacter8[n].LifeMode==0)
  {
   DeleteCharacter(n);
   n--;
   continue;
  }
  sCharacter8[n].CurrentImage+=sCharacter8[n].ChangeImage;
  if (sCharacter8[n].LifeMode==1)
  {
   if (sCharacter8[n].CurrentImage==Birth_Frame_End-1)
   {
    sCharacter8[n].LifeMode=2;
    sCharacter8[n].CurrentImage=Life_Frame_Start-1;
	continue;
   }
  }
  if (sCharacter8[n].LifeMode==2)
  {
   if (sCharacter8[n].CurrentImage==Life_Frame_End-1) sCharacter8[n].CurrentImage=Life_Frame_Start-1;
   continue;
  }
  if (sCharacter8[n].LifeMode==3)
  {
   if (sCharacter8[n].CurrentImage==Destruction_Frame_End-1) sCharacter8[n].LifeMode=0;
   continue;
  }
 }
}
void CCharacter8Map::Processing(void)
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
void CCharacter8Map::MoveCharacter(void)
{
 float pd_x=cPaddle.GetX();
 float pd_y=cPaddle.GetY();
 float pd_wd2=cPaddle.GetWidth()/2-4;
 float pd_hd2=cPaddle.GetHeight()/2;
 int pd_face[4];

 for(int n=0;n<CharacterAmount;n++)
 {
  if (sCharacter8[n].LifeMode!=2) continue;
  sCharacter8[n].TimeOfLife--;
  if (sCharacter8[n].TimeOfLife==0)//время жизни персонажа закончилось
  {
   DestroyCharacter(n);
   continue;
  }
  float new_x=sCharacter8[n].X;
  float new_y=sCharacter8[n].Y;
  if (sCharacter8[n].ModeOfMove==1) new_x--;
  if (sCharacter8[n].ModeOfMove==2) new_x++;
  if (sCharacter8[n].ModeOfMove==3) new_y--;
  if (sCharacter8[n].ModeOfMove==4) new_y++;
  //проверка столкновения со стенками
  int face1=CircleIntercectionToLine(0,52,480,52,new_x,new_y,(float)Radius);//верхняя
  int face2=CircleIntercectionToLine(0,252,480,252,new_x,new_y,(float)Radius);//нижняя
  int face3=CircleIntercectionToLine(17,0,17,272,new_x,new_y,(float)Radius);//левая
  int face4=CircleIntercectionToLine(458,0,458,272,new_x,new_y,(float)Radius);//правая
  if (face1+face2+face3+face4!=0) sCharacter8[n].TimeOfMove=0;
  else
  {
   sCharacter8[n].X=new_x;
   sCharacter8[n].Y=new_y;
  }
  //проверка столкновения с биткой
  //левая грань
  pd_face[0]=CircleIntercectionToLine(pd_x-pd_wd2,pd_y-pd_hd2+4,pd_x-pd_wd2,pd_y+pd_hd2-4,sCharacter8[n].X,sCharacter8[n].Y,(float)Radius);
  //правая грань
  pd_face[1]=CircleIntercectionToLine(pd_x+pd_wd2,pd_y-pd_hd2+4,pd_x+pd_wd2,pd_y+pd_hd2-4,sCharacter8[n].X,sCharacter8[n].Y,(float)Radius);
  //верхний полукруг
  pd_face[2]=CircleIntercectionToCircle(pd_x,pd_y-pd_hd2+4,4,sCharacter8[n].X,sCharacter8[n].Y,(float)Radius);
  //нижний полукруг
  pd_face[3]=CircleIntercectionToCircle(pd_x,pd_y+pd_hd2-4,4,sCharacter8[n].X,sCharacter8[n].Y,(float)Radius);
  if (pd_face[0]+pd_face[1]+pd_face[2]+pd_face[3]!=0)
  {
   cSoundControl.Play(SOUND_DESTROY);
   DestroyCharacter(n);
  }
  if (sCharacter8[n].TimeOfMove>0) sCharacter8[n].TimeOfMove--;
  if (sCharacter8[n].TimeOfMove==0)
  {
   sCharacter8[n].TimeOfMove=RND(200)+10;
   sCharacter8[n].ModeOfMove=RND(5);
  }
 }
}
void CCharacter8Map::PutCharacter(void)
{
 for(int n=0;n<CharacterAmount;n++)
 {
  cObject_Sprite.SetSpriteCoord(0,sCharacter8[n].CurrentImage);
  cObject_Sprite.Put((int)sCharacter8[n].X-(int)Radius,(int)sCharacter8[n].Y-(int)Radius);
 }
}
void CCharacter8Map::DestroyAllCharacter(void)
{
 for(int n=0;n<CharacterAmount;n++)
 {
  if (sCharacter8[n].LifeMode!=2 && sCharacter8[n].LifeMode!=1) continue;
  DestroyCharacter(n);
 }
}
void CCharacter8Map::ImpactCharacterToBall(int ball_x,int ball_y,int ball_radius,float &vx,float &vy)
{
 int n;
 int x,y;
 //отклоняем мяч
 for(n=0;n<CharacterAmount;n++)
 {
  if (sCharacter8[n].LifeMode!=2) continue;
  if (CircleIntercectionToCircle(ball_x,ball_y,ball_radius,sCharacter8[n].X,sCharacter8[n].Y,Radius)==true)
  {
   cSoundControl.Play(SOUND_DESTROY);
   DestroyCharacter(n);
   //ищем блок
   int b_x=0;
   int b_y=0;
   SBlock sBlock;
   int maxblock=0;
   for(x=0;x<26;x++)
    for(y=0;y<13;y++)
	{
     sBlock=cBlockMap.GetBlock(x,y);
     if (sBlock.Space==1) continue;
	 if (sBlock.Obstacle==0) continue;
     maxblock++;
	}
   int search=RND(maxblock)+1;
   for(x=0;x<26;x++)
   {
    for(y=0;y<13;y++)
	{
     sBlock=cBlockMap.GetBlock(x,y);
     if (sBlock.Space==1) continue;
	 if (sBlock.Obstacle==0) continue;
     b_x=x;
	 b_y=y;
	 search--;
	 if (search==0) break;
	}
    if (search==0) break;
   }
   //координаты центра блока
   float n_x=(float)(b_x*10+100+5);
   float n_y=(float)(b_y*15+62+7);
   vx=n_x-ball_x;
   vy=-(n_y-ball_y);
   Normalize(vx,vy);
  }
 }
}
bool CCharacter8Map::ImpactCharacterToMissile(int xleft,int xright,int yup,int ydown)
{
 int n;
 for(n=0;n<CharacterAmount;n++)
 {
  if (sCharacter8[n].LifeMode!=2) continue;
  int pd_face1=CircleIntercectionToLine((float)xleft,(float)yup,(float)xright,(float)yup,sCharacter8[n].X,sCharacter8[n].Y,(float)Radius);//верхняя
  int pd_face2=CircleIntercectionToLine((float)xleft,(float)ydown,(float)xright,(float)ydown,sCharacter8[n].X,sCharacter8[n].Y,(float)Radius);//нижняя
  int pd_face3=CircleIntercectionToLine((float)xleft,(float)yup,(float)xleft,(float)ydown,sCharacter8[n].X,sCharacter8[n].Y,(float)Radius);//левая
  int pd_face4=CircleIntercectionToLine((float)xright,(float)yup,(float)xright,(float)ydown,sCharacter8[n].X,sCharacter8[n].Y,(float)Radius);//правая
  if (pd_face1+pd_face2+pd_face3+pd_face4!=0)
  {
   cSoundControl.Play(SOUND_DESTROY);
   DestroyCharacter(n);
  }
 }
 return(false);//ракету не взрываем
}
