#include "cobject.h"

//------------------------------------------------------------------------------
CObject::CObject(void)
{
 cSprite.Release();
 Blend=false;
}
CObject::~CObject(void)
{
 cSprite.Release();
}
bool CObject::Load(char *FileName)
{
 Delete();
 ImageXPos=0;
 ImageYPos=0;
 if (cSprite.Load(FileName)==false) return(false);
 return(true);
}
void CObject::Put(int x,int y)
{
 cSprite.PutSpriteItem(x,y,ImageXPos*WidthImage,ImageYPos*HeightImage,WidthImage,HeightImage,Blend);
}
void CObject::Put(int x,int y,unsigned char *Mask)
{
 cSprite.PutSpriteItemMask(x,y,ImageXPos*WidthImage,ImageYPos*HeightImage,WidthImage,HeightImage,Mask);
}
void CObject::SetSpriteSize(int width,int height)
{
 HeightImage=height;
 WidthImage=width;
}
void CObject::SetSpriteCoord(int y,int x)
{
 ImageXPos=x;
 ImageYPos=y;
}
void CObject::SetBlendColor(unsigned char r,unsigned char g,unsigned char b)
{
 cSprite.SetAlpha(0,r,g,b);
 Blend=true;
}
void CObject::Delete(void)
{
 cSprite.Release();
 Blend=false;
}
