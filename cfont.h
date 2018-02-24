#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include "csprite.h"

//шрифт
class CFont
{
 protected:
  CSprite cSprite_Font;//спрайт шрифта
 public:
  CFont(void);//конструктор
  ~CFont();//деструктор
  bool Load(char *Path);//загрузить шрифт
  void Release(void);//удалить шрифт
  void PrintAt(int x,int y,char *string,bool alpha);//напечатать строку
};
#endif
