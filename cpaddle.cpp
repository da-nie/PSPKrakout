#include "cpaddle.h"

CPaddle cPaddle;

void CPaddle::Create(char *Path)
{
 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssprites\\paddle_normal.tga",Path);
 cObject_PaddleNormal.Load(FileName);
 sprintf(FileName,"%ssprites\\paddle_double.tga",Path);
 cObject_PaddleDouble.Load(FileName);
 delete(FileName);
 cObject_PaddleNormal.SetSpriteSize(18,37);
 cObject_PaddleDouble.SetSpriteSize(18,62);
 cObject_PaddleNormal.SetBlendColor(0,0,0);
 cObject_PaddleDouble.SetBlendColor(0,0,0);
}
void CPaddle::Release(void)
{
 cObject_PaddleNormal.Delete();
 cObject_PaddleDouble.Delete();
}
//--------------------------------------------------------------------------
int CPaddle::GetHeight(void)
{
 return(Height);
}
int CPaddle::GetWidth(void)
{
 return(Width);
}
int CPaddle::GetX(void)
{
 return((int)X);
}
int CPaddle::GetY(void)
{
 return((int)Y);
}
bool CPaddle::GetMagnet(void)
{
 return(Magnet);
}
bool CPaddle::GetMissile(void)
{
 return(Missile);
}
//--------------------------------------------------------------------------
void CPaddle::InitializeToStartLevel(void)
{
 X=440;
 Y=152;
 Height=cObject_PaddleNormal.HeightImage;
 Width=cObject_PaddleNormal.WidthImage;
 Double=false;
 Missile=false;
 Magnet=false;
 Speed=0;
 Move=false;
}
void CPaddle::PutPaddle(void)
{
 if (Double==false)
 {
  if (Missile==false) cObject_PaddleNormal.SetSpriteCoord(0,0);
                 else cObject_PaddleNormal.SetSpriteCoord(0,1);
  cObject_PaddleNormal.Put((int)(X-Width/2),(int)(Y-Height/2));
 }
 else
 {
  if (Missile==false) cObject_PaddleDouble.SetSpriteCoord(0,0);
                 else cObject_PaddleDouble.SetSpriteCoord(0,1);
  cObject_PaddleDouble.Put((int)(X-Width/2),(int)(Y-Height/2));
 }
}
void CPaddle::MoveUp(void)
{
 if (Speed>0) Speed=0;
 Speed-=0.5;
 Move=true;
 if (Speed<-5) Speed=-5;
}
void CPaddle::MoveDown(void)
{
 if (Speed<0) Speed=0;
 Speed+=0.5;
 Move=true;
 if (Speed>5) Speed=5;
}
void CPaddle::Processing(void)
{
 if (Speed==0) return;
 if (Move==false) Speed=0;
 Y+=Speed;
 if (Y-Height/2<52) Y=(float)(52+Height/2);
 if (Y+Height/2>252) Y=(float)(252-Height/2);
 Move=false;
}
void CPaddle::DoubleMode(void)
{
 Double=true;
 Height=cObject_PaddleDouble.HeightImage;
 Width=cObject_PaddleDouble.WidthImage;
 if (Y-Height/2<52) Y=(float)(52+Height/2);
 if (Y+Height/2>252) Y=(float)(252-Height/2);
}
void CPaddle::NormalMode(void)
{
 Double=false;
 Height=cObject_PaddleNormal.HeightImage;
 Width=cObject_PaddleNormal.WidthImage;
 if (Y-Height/2<52) Y=(float)(52+Height/2);
 if (Y+Height/2>252) Y=(float)(252-Height/2);
}
void CPaddle::MissileModeOn(void)
{
 Missile=true;
}
void CPaddle::MissileModeOff(void)
{
 Missile=false;
}
void CPaddle::MagnetModeOn(void)
{
 Magnet=true;
}
void CPaddle::MagnetModeOff(void)
{
 Magnet=false;
}
void CPaddle::CorrectUpYPos(int dy)
{
 if (Y-Height/2-dy-2<52) Y=(float)(52+Height/2+dy+2);
}
void CPaddle::CorrectDownYPos(int dy)
{
 if (Y+Height/2+dy+2>252) Y=(float)(252-Height/2-dy-2);
}
