#include <string.h>
#include <math.h>

#include "cballmap.h"
#include "cpaddle.h"
#include "cblockmap.h"
#include "cmissilemap.h"
#include "ccharactermap.h"
#include "basis.h"
#include "csoundcontrol.h"

extern CPaddle cPaddle;
extern CBlockMap cBlockMap;
extern CMissileMap cMissileMap;
extern CCharacterMap cCharacterMap;
extern SGameParameters sGameParameters;
extern CSoundControl cSoundControl;

CBallMap cBallMap;

void CBallMap::Create(char *Path)
{
 int frame_size_x;
 int frame_size_y;
 int blend_color_r;
 int blend_color_g;
 int blend_color_b;
 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssprites\\ball.txt",Path);
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
 if (GetParameters(FileName,(char*)"FRAME:",&Frame)==false)
 {
  delete(FileName);
  return;
 }
 sprintf(FileName,"%ssprites\\ball.tga",Path);
 cObject_Ball.Load(FileName);
 cObject_Ball.SetSpriteSize(frame_size_x,frame_size_y);
 cObject_Ball.SetBlendColor(blend_color_r,blend_color_g,blend_color_b);
 delete(FileName);
}
void CBallMap::Release(void)
{
 cObject_Ball.Delete();
}
//--------------------------------------------------------------------------
int CBallMap::GetCurrentBall(void)
{
 return(CurrentBall);
}
void CBallMap::GetBallCoord(int ball,float &x,float &y)
{
 x=sBall_Array[ball].X;
 y=sBall_Array[ball].Y;
}
void CBallMap::GetBallVector(int ball,float &vx,float &vy)
{
 vx=sBall_Array[ball].Vx;
 vy=sBall_Array[ball].Vy;
}
//--------------------------------------------------------------------------
void CBallMap::InitializeToStartLevel(void)
{
 CurrentBall=0;
 float vy=sin(M_PI/180.0*190.0);
 float vx=cos(M_PI/180.0*190.0);
 CreateNewBall(415,152,vx,vy,0);
}
//--------------------------------------------------------------------------
void CBallMap::CreateNewBall(float x,float y,float vx,float vy,int mode)
{
 sBall_Array[CurrentBall].X=x;
 sBall_Array[CurrentBall].Y=y;
 sBall_Array[CurrentBall].Radius=6;
 sBall_Array[CurrentBall].Vx=vx;
 sBall_Array[CurrentBall].Vy=vy;
 sBall_Array[CurrentBall].Mode=mode;
 sBall_Array[CurrentBall].Image=0;
 sBall_Array[CurrentBall].WaitTime=0;
 sBall_Array[CurrentBall].NoWaitTime=false;
 sBall_Array[CurrentBall].MagnetDy=0;
 if (mode==0)
 {
  sBall_Array[CurrentBall].WaitTime=100;
  sBall_Array[CurrentBall].NoWaitTime=false;
 }
 CurrentBall++;
}
void CBallMap::DeleteBall(int ball)
{
 if (CurrentBall==0) return;
 for(int n=ball+1;n<CurrentBall;n++) sBall_Array[n-1]=sBall_Array[n];
 CurrentBall--;
}
//--------------------------------------------------------------------------
void CBallMap::PutAllBall(void)
{
 for(int n=0;n<CurrentBall;n++)
 {
  cObject_Ball.SetSpriteCoord(0,sBall_Array[n].Image);
  cObject_Ball.Put((int)sBall_Array[n].X-sBall_Array[n].Radius,(int)sBall_Array[n].Y-sBall_Array[n].Radius);
 }
}
//--------------------------------------------------------------------------
void CBallMap::ChangeImageFrame(void)
{
 for(int n=0;n<CurrentBall;n++)
 {
  sBall_Array[n].Image++;
  if (sBall_Array[n].Image==Frame) sBall_Array[n].Image-=Frame;
  if (sBall_Array[n].Mode==0 && sBall_Array[n].NoWaitTime==false && sBall_Array[n].WaitTime>0)
  {
   sBall_Array[n].WaitTime--;
   if (sBall_Array[n].WaitTime==0) sBall_Array[n].Mode=1;
  }
 }
}
//--------------------------------------------------------------------------
bool CBallMap::Processing(void)
{
 for(int n=0;n<CurrentBall;n++)
 {
  bool noball=MoveBall(n);
  if (noball==true)
  {
   DeleteBall(n);
   n--;
   continue;
  }
 }
 if (CurrentBall==0) return(true);
 return(false);
}
bool CBallMap::MoveBall(int ball)
{
 int n;
 SBall *sBallPtr=&sBall_Array[ball];
 if (sBallPtr->Mode==0)
 {
  sBallPtr->X=(float)cPaddle.GetX()-(float)cPaddle.GetWidth()/2-sBallPtr->Radius+2;
  sBallPtr->Y=(float)cPaddle.GetY()+sBallPtr->MagnetDy;
  if (sBallPtr->MagnetDy>0)//мяч ниже центра битки
  {
   int dy=sBallPtr->MagnetDy+sBallPtr->Radius-cPaddle.GetHeight()/2;
   if (dy>0) cPaddle.CorrectDownYPos(dy);
  }
  if (sBallPtr->MagnetDy<0)//мяч выше центра битки
  {
   int dy=(int)(fabs(sBallPtr->MagnetDy)+sBallPtr->Radius-cPaddle.GetHeight()/2);
   if (dy>0) cPaddle.CorrectUpYPos(dy);
  }
  //проверим столкновение с персонажами
  cCharacterMap.ImpactCharacterToBall((int)sBallPtr->X,(int)sBallPtr->Y,sBallPtr->Radius,sBallPtr->Vx,sBallPtr->Vy);
  return(false);//мяч следует за биткой и летать не может
 }
 float dx=sBallPtr->Vx;
 float dy=-sBallPtr->Vy;
 float new_x=sBallPtr->X+dx;
 float new_y=sBallPtr->Y+dy;
 //проверим столкновение с персонажами
 cCharacterMap.ImpactCharacterToBall((int)sBallPtr->X,(int)sBallPtr->Y,sBallPtr->Radius,sBallPtr->Vx,sBallPtr->Vy);
 //проверим столкновение с другими мячами
 for(n=0;n<CurrentBall;n++)
 {
  if (n==ball) continue;
  if (CircleIntercectionToCircle(new_x,new_y,sBallPtr->Radius,sBall_Array[n].X,sBall_Array[n].Y,sBall_Array[n].Radius)!=0)//есть столкновение
  {
   float rx=sBall_Array[n].X;
   float ry=sBall_Array[n].Y;
   float nx=new_x-rx;
   float ny=new_y-ry;
   float norma=sqrt(nx*nx+ny*ny);
   if (norma==0) norma=1;
   nx/=norma;
   ny/=norma;
   Refraction(sBallPtr->Vx,sBallPtr->Vy,nx,ny,sBallPtr->Vx,sBallPtr->Vy);
   sBallPtr->X=rx+nx*((float)sBallPtr->Radius*2+1);
   sBallPtr->Y=ry+ny*((float)sBallPtr->Radius*2+1);
   dx=sBallPtr->Vx;
   dy=-sBallPtr->Vy;
   new_x=sBallPtr->X+dx;
   new_y=sBallPtr->Y+dy;
   break;
  }
 }
 //проверим столкновение со стенками
 int face1=CircleIntercectionToLine(0,52,480,52,new_x,new_y,(float)sBallPtr->Radius);//верхняя
 int face2=CircleIntercectionToLine(0,252,480,252,new_x,new_y,(float)sBallPtr->Radius);//нижняя
 int face3=CircleIntercectionToLine(17,0,17,272,new_x,new_y,(float)sBallPtr->Radius);//левая
 int face4=CircleIntercectionToLine(458,0,458,272,new_x,new_y,(float)sBallPtr->Radius);//правая
 if (sGameParameters.RightWall==false) face4=0;
 //верням мяч на игровое поле
 if (face1!=0) sBallPtr->Y=53+sBallPtr->Radius;
 if (face2!=0) sBallPtr->Y=251-sBallPtr->Radius;
 if (face3!=0) sBallPtr->X=18+sBallPtr->Radius;
 if (face4!=0) sBallPtr->X=457-sBallPtr->Radius;
 //проверим столкновение с биткой
 float pd_x=cPaddle.GetX();
 float pd_y=cPaddle.GetY();
 float pd_wd2=cPaddle.GetWidth()/2-4;
 float pd_hd2=cPaddle.GetHeight()/2;
 int pd_face[4];
 //левая грань
 pd_face[0]=CircleIntercectionToLine(pd_x-pd_wd2,pd_y-pd_hd2+4,pd_x-pd_wd2,pd_y+pd_hd2-4,new_x,new_y,(float)sBallPtr->Radius);
 //правая грань
 pd_face[1]=CircleIntercectionToLine(pd_x+pd_wd2,pd_y-pd_hd2+4,pd_x+pd_wd2,pd_y+pd_hd2-4,new_x,new_y,(float)sBallPtr->Radius);
 //верхний полукруг
 pd_face[2]=CircleIntercectionToCircle(pd_x,pd_y-pd_hd2+4,4,new_x,new_y,(float)sBallPtr->Radius);
 //нижний полукруг
 pd_face[3]=CircleIntercectionToCircle(pd_x,pd_y+pd_hd2-4,4,new_x,new_y,(float)sBallPtr->Radius);
 //корректируем отражения
 if (new_y>=pd_y-pd_hd2+4) pd_face[2]=0;
 if (new_y<=pd_y+pd_hd2-4) pd_face[3]=0;
 if (pd_face[2]+pd_face[3]!=0)
 {
  pd_face[0]=0;
  pd_face[1]=0;
 }
 //проверим столкновение с блоками
 int x1=(int)((new_x-sBallPtr->Radius)-100)/10-1;
 int x2=(int)((new_x+sBallPtr->Radius)-100)/10+1;
 int y1=(int)((new_y-sBallPtr->Radius)-62)/15-1;
 int y2=(int)((new_y+sBallPtr->Radius)-62)/15+1;
 if (x1<0) x1=0;
 if (y1<0) y1=0;
 if (x2>24) x2=24;
 if (y2>11) y2=11;
 for(int x=x1;x<=x2;x++)
  for(int y=y1;y<=y2;y++)
  {
   SBlock sBlock=cBlockMap.GetBlock(x,y);
   int impact=0;
   if (sBlock.Obstacle==false || sBlock.Space==true) continue;
   int xleft=x*10+100;
   int xright=(x+1)*10+99;
   int yup=y*15+62;
   int ydown=(y+1)*15+61;
   if (CircleIntercectionToLine((float)xleft,(float)ydown,(float)xleft,(float)yup,new_x,new_y,(float)sBallPtr->Radius))
   {
	face3=1;
	impact=1;
   }
   if (CircleIntercectionToLine((float)xright,(float)ydown,(float)xright,(float)yup,new_x,new_y,(float)sBallPtr->Radius))
   {
	face4=1;
	impact=1;
   }
   if (CircleIntercectionToLine((float)xleft,(float)yup,(float)xright,(float)yup,new_x,new_y,(float)sBallPtr->Radius))
   {
	face1=1;
	impact=1;
   }
   if (CircleIntercectionToLine((float)xleft,(float)ydown,(float)xright,(float)ydown,new_x,new_y,(float)sBallPtr->Radius))
   {
    face2=1;
	impact=1;
   }
   if (impact==1) cBlockMap.ActivateBlock(x,y);
  }
 //верхняя грань
 if (face1!=0) Refraction(sBallPtr->Vx,sBallPtr->Vy,0,1,sBallPtr->Vx,sBallPtr->Vy);
 //нижняя грань
 if (face2!=0) Refraction(sBallPtr->Vx,sBallPtr->Vy,0,-1,sBallPtr->Vx,sBallPtr->Vy);
 //левая грань
 if (face3!=0) Refraction(sBallPtr->Vx,sBallPtr->Vy,-1,0,sBallPtr->Vx,sBallPtr->Vy);
 //правая грань
 if (face4!=0) Refraction(sBallPtr->Vx,sBallPtr->Vy,1,0,sBallPtr->Vx,sBallPtr->Vy);
 //обработаем столкновение с биткой
 if (pd_face[0]!=0)//левая грань
 {
  float dy=(float)(new_y-cPaddle.GetY())/pd_wd2;
  float dangle=10*dy;
  float nx=cos(M_PI/180.0*(180+dangle));
  float ny=sin(M_PI/180.0*(180+dangle));
  Refraction(sBallPtr->Vx,sBallPtr->Vy,nx,ny,sBallPtr->Vx,sBallPtr->Vy);
  if (sBallPtr->Vx>0) sBallPtr->Vx=-sBallPtr->Vx;
  sBallPtr->X=(float)(cPaddle.GetX()-5-cPaddle.GetWidth()/2);
 }
 if (pd_face[1]!=0)//правая грань
 {
  float dy=(float)(new_y-cPaddle.GetY())/pd_wd2;
  float dangle=10*dy;
  float nx=cos(M_PI/180.0*dangle);
  float ny=sin(M_PI/180.0*dangle);
  Refraction(sBallPtr->Vx,sBallPtr->Vy,nx,ny,sBallPtr->Vx,sBallPtr->Vy);
  if (sBallPtr->Vx<0) sBallPtr->Vx=-sBallPtr->Vx;
  sBallPtr->X=(float)(cPaddle.GetX()+5+cPaddle.GetWidth()/2);
 }
 if (pd_face[2]!=0)//верхний полукруг
 {
  float rx=pd_x;
  float ry=pd_y-pd_hd2+4;
  float nx=new_x-rx;
  float ny=new_y-ry;
  Normalize(nx,ny);
  Refraction(sBallPtr->Vx,sBallPtr->Vy,nx,ny,sBallPtr->Vx,sBallPtr->Vy);
  sBallPtr->X=rx+nx*((float)sBallPtr->Radius+6);
  sBallPtr->Y=ry+ny*((float)sBallPtr->Radius+6);
  //проверим, не упёрся ли мяч в стенку
  if (new_y-sBallPtr->Radius<=52)
  {
   //смещаем мяч
   sBallPtr->Y=53+sBallPtr->Radius;
   //смещаем битку
   cPaddle.CorrectUpYPos(sBallPtr->Radius*2);
  }
 }
 if (pd_face[3]!=0)//нижний полукруг
 {
  float rx=pd_x;
  float ry=pd_y+pd_hd2-4;
  float nx=new_x-rx;
  float ny=new_y-ry;
  Normalize(nx,ny);
  Refraction(sBallPtr->Vx,sBallPtr->Vy,nx,ny,sBallPtr->Vx,sBallPtr->Vy);
  sBallPtr->X=rx+nx*((float)sBallPtr->Radius+6);
  sBallPtr->Y=ry+ny*((float)sBallPtr->Radius+6);
  //проверим, не упёрся ли мяч в стенку
  if (new_y+sBallPtr->Radius>=252)
  {
   //смещаем мяч
   sBallPtr->Y=251-sBallPtr->Radius;
   //смещаем битку
   cPaddle.CorrectDownYPos(sBallPtr->Radius*2);
  }
 }
 if (cPaddle.GetMagnet()==1 && pd_face[0]+pd_face[1]+pd_face[2]+pd_face[3]!=0)
 {
  sBallPtr->Mode=0;
  sBallPtr->NoWaitTime=true;
  sBallPtr->MagnetDy=(int)(sBallPtr->Y-cPaddle.GetY());
 }
 if (pd_face[0]+pd_face[1]+pd_face[2]+pd_face[3]!=0) cSoundControl.Play(SOUND_IMPACT_5);
 //нельзя допускать вертикальных углов полёта мяча
 if (sBallPtr->Vx<0.2 && sBallPtr->Vx>=0)
 {
  sBallPtr->Vx+=0.2;
  Normalize(sBallPtr->Vx,sBallPtr->Vy);
 }
 if (sBallPtr->Vx>-0.2 && sBallPtr->Vx<=0)
 {
  sBallPtr->Vx-=0.2;
  Normalize(sBallPtr->Vx,sBallPtr->Vy);
 }
 if (face1+face2+face3+face4+pd_face[0]+pd_face[1]+pd_face[2]+pd_face[3]==0)
 {
  sBallPtr->X=new_x;
  sBallPtr->Y=new_y;
 }
 if (face1+face2+face3+face4!=0) cSoundControl.Play(SOUND_IMPACT_1);
 if (sBallPtr->X>=485+sBallPtr->Radius) return(true);//мяч вылетел за пределы поля
 return(false);
}
void CBallMap::StartAllBall(void)
{
 for(int n=0;n<CurrentBall;n++) sBall_Array[n].Mode=1;
}
void CBallMap::SetAllBallToPaddle(void)
{
 for(int n=0;n<CurrentBall;n++)
 {
  if (sBall_Array[n].Mode==0)
  {
   sBall_Array[n].X=(float)cPaddle.GetX()-(float)cPaddle.GetWidth()/2-sBall_Array[n].Radius+3;
   sBall_Array[n].Y=(float)cPaddle.GetY();
  }
 }
}
int CBallMap::GetNearestBallNumber(float x,float y)
{
 int selectball=-1;
 float mindist2=100000000;
 for(int m=0;m<CurrentBall;m++)
 {
  float bx,by;
  bx=sBall_Array[m].X;
  by=sBall_Array[m].Y;
  float dist2=(bx-x)*(bx-x)+(by-y)*(by-y);
  if (dist2<mindist2)
  {
   mindist2=dist2;
   selectball=m;
  }
 }
 return(selectball);
}
int CBallMap::GetNearestToVerticalLineBallNumber(float x)
{
 int selectball=-1;
 float mindist2=100000000;
 for(int m=0;m<CurrentBall;m++)
 {
  float bx,by;
  bx=sBall_Array[m].X;
  by=sBall_Array[m].Y;
  float dist2=(bx-x)*(bx-x);
  if (dist2<mindist2)
  {
   mindist2=dist2;
   selectball=m;
  }
 }
 return(selectball);
}
