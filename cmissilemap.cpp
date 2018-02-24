#include "cmissilemap.h"
#include "ccharactermap.h"
#include "cblockmap.h"
#include "basis.h"
extern CBlockMap cBlockMap;
extern CCharacterMap cCharacterMap;

//--------------------------------------------------------------------------
CMissileMap cMissileMap;

void CMissileMap::Create(char *Path)
{
 int frame_size_x;
 int frame_size_y;
 int blend_color_r;
 int blend_color_g;
 int blend_color_b;

 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssprites\\missile.txt",Path);
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
 sprintf(FileName,"%ssprites\\missile.tga",Path);
 cObject_Missile.Load(FileName);
 cObject_Missile.SetSpriteSize(frame_size_x,frame_size_y);
 cObject_Missile.SetBlendColor(blend_color_r,blend_color_g,blend_color_b);
 delete(FileName);
}
void CMissileMap::Release(void)
{
 cObject_Missile.Delete();
}
//--------------------------------------------------------------------------
void CMissileMap::InitializeToStartLevel(void)
{
 CurrentMissile=0;
}
void CMissileMap::CreateNewMissile(float x,float y)
{
 sMissile_Array[CurrentMissile].X=x;
 sMissile_Array[CurrentMissile].Y=y;
 sMissile_Array[CurrentMissile].Image=0;
 sMissile_Array[CurrentMissile].Height=cObject_Missile.HeightImage;
 sMissile_Array[CurrentMissile].Width=cObject_Missile.WidthImage;
 CurrentMissile++;
 if (CurrentMissile>=100) CurrentMissile=99;
}
void CMissileMap::DeleteMissile(int missile)
{
 if (CurrentMissile==0) return;
 for(int n=missile+1;n<CurrentMissile;n++) sMissile_Array[n-1]=sMissile_Array[n];
 CurrentMissile--;
}
void CMissileMap::PutAllMissile(void)
{
 for(int n=0;n<CurrentMissile;n++)
 {
  cObject_Missile.SetSpriteCoord(sMissile_Array[n].Image,0);
  cObject_Missile.Put((int)(sMissile_Array[n].X-sMissile_Array[n].Width/2),(int)(sMissile_Array[n].Y-sMissile_Array[n].Height/2));
 }
}
void CMissileMap::Processing(void)
{
 for(int n=0;n<CurrentMissile;n++)
 {
  float xleft=sMissile_Array[n].X-sMissile_Array[n].Width/2;
  float xright=sMissile_Array[n].X+sMissile_Array[n].Width/2;
  float yup=sMissile_Array[n].Y-sMissile_Array[n].Height/2;
  float ydown=sMissile_Array[n].Y+sMissile_Array[n].Height/2;
  int x1=(int)(xleft-100)/10-2;
  int x2=(int)(xright-100)/10+2;
  int y1=(int)(yup-62)/15-2;
  int y2=(int)(ydown-62)/15+2;
  if (x1<0) x1=0;
  if (y1<0) y1=0;
  if (x2>24) x2=24;
  if (y2>11) y2=11;
  for(int x=x1;x<=x2;x++)
   for(int y=y1;y<=y2;y++)
   {
    int xb1=x*10+100;
    int xb2=(x+1)*10+100;
    int yb1=y*15+62;
    int yb2=(y+1)*15+62;
    if (xleft>xb2) continue;
    if (xright<xb1) continue;
    if (yup>yb2) continue;
    if (ydown<yb1) continue;
    cBlockMap.DestroyBlock(x,y);
   }
  sMissile_Array[n].X-=2;
  bool res=cCharacterMap.ImpactCharacterToMissile((int)xleft,(int)xright,(int)yup,(int)ydown);
  if (sMissile_Array[n].X-sMissile_Array[n].Width/2<17 || res==true)
  {
   DeleteMissile(n);
   n--;
   continue;
  }
 }
}
void CMissileMap::ChangeImageFrame(void)
{
 for(int n=0;n<CurrentMissile;n++)
 {
  sMissile_Array[n].Image++;
  if (sMissile_Array[n].Image==Frame) sMissile_Array[n].Image=0;
 }
}
