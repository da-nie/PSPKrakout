#include "cfont.h"

CFont cFont;

CFont::CFont(void)
{
}
CFont::~CFont()
{
 Release();
}
bool CFont::Load(char *Path)
{
 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssprites\\font.tga",Path);
 bool res=cSprite_Font.Load(FileName);
 delete(FileName);
 cSprite_Font.SetAlpha(0,0,0,0);
 return(res);
}
void CFont::Release(void)
{
 cSprite_Font.Release();
}
void CFont::PrintAt(int x,int y,char *string,bool alpha)
{
 int n;
 if (string==NULL) return;
 int length=strlen(string);
 for(n=0;n<length;n++,x+=13)
 {
  int code=-1;
  int b=(unsigned char)string[n];
  if (b<=(unsigned char)'~') code=b-32;
  if (b>=(unsigned char)'À') code=b-(unsigned char)'À'+(unsigned char)'~'-30;
  if (code<0) continue;
  cSprite_Font.PutSpriteItem(x,y,code*13,0,13,22,alpha);
 }
}
