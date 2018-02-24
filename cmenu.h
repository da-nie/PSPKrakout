#ifndef CMENU_H_INCLUDED
#define CMENU_H_INCLUDED

#include "cobject.h"
#include "cgame.h"

struct SHiScore
{
 char Name[21];//имя
 int Score;//количество очков
};

class CMenu
{
 protected:
  CSprite cSprite_Back;//спрайты фона
  CSprite cSprite_Name[7];//спрайты названия игры
  SHiScore sHiScore[10];//таблица рекордов
  int Phase;//фаза смещения букв названия
  //-----------------------------------------------------------
  CGame cGame;
 public:
  CMenu(void);
  ~CMenu();
  //-----------------------------------------------------------
  void Create(char *Path);
  void Release(void);
  //-----------------------------------------------------------
  void SaveHiScore(char *Path);//сохранение таблицы рекордов
  bool LoadHiScore(char *Path);//загрузка таблицы рекордов
  void CreateHiScore(void);//создать таблицу рекордов
  void FindAllLevels(char *Path);//найти все уровни
  void DrawHiScore(void);//вывести таблицу рекордов
  void InputHiScore(int score);//ввод имени в таблицу рекордов
  //-----------------------------------------------------------
  void Activate(char *Path);//запустить на выполнение
  //-----------------------------------------------------------
};

#endif
