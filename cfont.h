#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include "csprite.h"

//�����
class CFont
{
 protected:
  CSprite cSprite_Font;//������ ������
 public:
  CFont(void);//�����������
  ~CFont();//����������
  bool Load(char *Path);//��������� �����
  void Release(void);//������� �����
  void PrintAt(int x,int y,char *string,bool alpha);//���������� ������
};
#endif
