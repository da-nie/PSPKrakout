#include "ccharactermap.h"

CCharacterMap cCharacterMap;

void CCharacterMap::Create(char *Path)
{
 cCharacter1Map.Create(Path);
 cCharacter2Map.Create(Path);
 cCharacter3Map.Create(Path);
 cCharacter4Map.Create(Path);
 cCharacter5Map.Create(Path);
 cCharacter6Map.Create(Path);
 cCharacter7Map.Create(Path);
 cCharacter8Map.Create(Path);
 cCharacter9Map.Create(Path);
}
void CCharacterMap::Release(void)
{
 cCharacter1Map.Release();
 cCharacter2Map.Release();
 cCharacter3Map.Release();
 cCharacter4Map.Release();
 cCharacter5Map.Release();
 cCharacter6Map.Release();
 cCharacter7Map.Release();
 cCharacter8Map.Release();
 cCharacter9Map.Release();
}
void CCharacterMap::InitializeToStartLevel(void)
{
 cCharacter1Map.InitializeToStartLevel();
 cCharacter2Map.InitializeToStartLevel();
 cCharacter3Map.InitializeToStartLevel();
 cCharacter4Map.InitializeToStartLevel();
 cCharacter5Map.InitializeToStartLevel();
 cCharacter6Map.InitializeToStartLevel();
 cCharacter7Map.InitializeToStartLevel();
 cCharacter8Map.InitializeToStartLevel();
 cCharacter9Map.InitializeToStartLevel();
}
void CCharacterMap::PutAllCharacter(void)
{
 cCharacter1Map.PutCharacter();
 cCharacter2Map.PutCharacter();
 cCharacter3Map.PutCharacter();
 cCharacter4Map.PutCharacter();
 cCharacter5Map.PutCharacter();
 cCharacter6Map.PutCharacter();
 cCharacter7Map.PutCharacter();
 cCharacter8Map.PutCharacter();
 cCharacter9Map.PutCharacter();
}
void CCharacterMap::DestroyAllCharacter(void)
{
 cCharacter1Map.DestroyAllCharacter();
 cCharacter2Map.DestroyAllCharacter();
 cCharacter3Map.DestroyAllCharacter();
 cCharacter4Map.DestroyAllCharacter();
 cCharacter5Map.DestroyAllCharacter();
 cCharacter6Map.DestroyAllCharacter();
 cCharacter7Map.DestroyAllCharacter();
 cCharacter8Map.DestroyAllCharacter();
 cCharacter9Map.DestroyAllCharacter();
}
void CCharacterMap::ChangeImageFrame(void)
{
 cCharacter1Map.AnimateCharacter();
 cCharacter2Map.AnimateCharacter();
 cCharacter3Map.AnimateCharacter();
 cCharacter4Map.AnimateCharacter();
 cCharacter5Map.AnimateCharacter();
 cCharacter6Map.AnimateCharacter();
 cCharacter7Map.AnimateCharacter();
 cCharacter8Map.AnimateCharacter();
 cCharacter9Map.AnimateCharacter();
}
void CCharacterMap::Processing(void)
{
 cCharacter1Map.Processing();
 cCharacter2Map.Processing();
 cCharacter3Map.Processing();
 cCharacter4Map.Processing();
 cCharacter5Map.Processing();
 cCharacter6Map.Processing();
 cCharacter7Map.Processing();
 cCharacter8Map.Processing();
 cCharacter9Map.Processing();
}
void CCharacterMap::ImpactCharacterToBall(int ball_x,int ball_y,int ball_radius,float &vx,float &vy)
{
 cCharacter1Map.ImpactCharacterToBall(ball_x,ball_y,ball_radius,vx,vy);
 cCharacter2Map.ImpactCharacterToBall(ball_x,ball_y,ball_radius,vx,vy);
 cCharacter3Map.ImpactCharacterToBall(ball_x,ball_y,ball_radius,vx,vy);
 cCharacter4Map.ImpactCharacterToBall(ball_x,ball_y,ball_radius,vx,vy);
 cCharacter5Map.ImpactCharacterToBall(ball_x,ball_y,ball_radius,vx,vy);
 cCharacter6Map.ImpactCharacterToBall(ball_x,ball_y,ball_radius,vx,vy);
 cCharacter7Map.ImpactCharacterToBall(ball_x,ball_y,ball_radius,vx,vy);
 cCharacter8Map.ImpactCharacterToBall(ball_x,ball_y,ball_radius,vx,vy);
 cCharacter9Map.ImpactCharacterToBall(ball_x,ball_y,ball_radius,vx,vy);
}
bool CCharacterMap::ImpactCharacterToMissile(int xleft,int xright,int yup,int ydown)
{
 bool ret=false;
 if (cCharacter1Map.ImpactCharacterToMissile(xleft,xright,yup,ydown)==true) ret=true;
 if (cCharacter2Map.ImpactCharacterToMissile(xleft,xright,yup,ydown)==true) ret=true;
 if (cCharacter3Map.ImpactCharacterToMissile(xleft,xright,yup,ydown)==true) ret=true;
 if (cCharacter4Map.ImpactCharacterToMissile(xleft,xright,yup,ydown)==true) ret=true;
 if (cCharacter5Map.ImpactCharacterToMissile(xleft,xright,yup,ydown)==true) ret=true;
 if (cCharacter6Map.ImpactCharacterToMissile(xleft,xright,yup,ydown)==true) ret=true;
 if (cCharacter7Map.ImpactCharacterToMissile(xleft,xright,yup,ydown)==true) ret=true;
 if (cCharacter8Map.ImpactCharacterToMissile(xleft,xright,yup,ydown)==true) ret=true;
 if (cCharacter9Map.ImpactCharacterToMissile(xleft,xright,yup,ydown)==true) ret=true;
 return(ret);
}
