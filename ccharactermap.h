#ifndef CCHARACTERMAP_H_INCLUDED
#define CCHARACTERMAP_H_INCLUDED

#include "ccharacter1map.h"
#include "ccharacter2map.h"
#include "ccharacter3map.h"
#include "ccharacter4map.h"
#include "ccharacter5map.h"
#include "ccharacter6map.h"
#include "ccharacter7map.h"
#include "ccharacter8map.h"
#include "ccharacter9map.h"

class CCharacterMap
{
 protected:
  CCharacter1Map cCharacter1Map;
  CCharacter2Map cCharacter2Map;
  CCharacter3Map cCharacter3Map;
  CCharacter4Map cCharacter4Map;
  CCharacter5Map cCharacter5Map;
  CCharacter6Map cCharacter6Map;
  CCharacter7Map cCharacter7Map;
  CCharacter8Map cCharacter8Map;
  CCharacter9Map cCharacter9Map;
 public:
  void Create(char *Path);
  void Release(void);
  void InitializeToStartLevel(void);
  void PutAllCharacter(void);
  void DestroyAllCharacter(void);
  void ChangeImageFrame(void);
  void Processing(void);
  void ImpactCharacterToBall(int ball_x,int ball_y,int ball_radius,float &vx,float &vy);
  bool ImpactCharacterToMissile(int xleft,int xright,int yup,int ydown);
};
#endif
