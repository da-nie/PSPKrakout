#ifndef CGAME_H_INCLUDED
#define CGAME_H_INCLUDED

#include "cobject.h"

class CGame
{
 protected:
  CSprite cSprite_Back;
  //-----------------------------------------------------------
 public:
  CGame(void);
  ~CGame();
  //-----------------------------------------------------------
  void Create(char *Path);
  void Release(void);
  //-----------------------------------------------------------
  bool InitializeToStartGame(char *Path);
  bool InitializeToStartNextLevel(char *Path);
  void InitializeToReStartLevel(void);
  void SaveScreenShot(char *Path);
  void ComputerControl(void);
  //-----------------------------------------------------------
  void StartGame(char *Path);
  //-----------------------------------------------------------
};

#endif
