#include "cblockmap.h"
#include "cpaddle.h"
#include "cfont.h"
#include "basis.h"
#include "csoundcontrol.h"

extern CFont cFont;
extern SGameParameters sGameParameters;
extern CPaddle cPaddle;
extern CSoundControl cSoundControl;
//--------------------------------------------------------------------------
CBlockMap cBlockMap;

void CBlockMap::Create(char *Path)
{
 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssprites\\block.tga",Path);
 cObject_Block.Load(FileName);
 cObject_Block.SetSpriteSize(10,15);
 sprintf(FileName,"%ssprites\\block_prize.tga",Path);
 cObject_BlockPrize.Load(FileName);
 cObject_BlockPrize.SetSpriteSize(10,15);
 sprintf(FileName,"%ssprites\\upframe.tga",Path);
 cObject_UpWall.Load(FileName);
 cObject_UpWall.SetSpriteSize(480,28);
 cObject_UpWall.SetBlendColor(0,0,0);
 sprintf(FileName,"%ssprites\\downframe.tga",Path);
 cObject_DownWall.Load(FileName);
 cObject_DownWall.SetSpriteSize(480,28);
 cObject_DownWall.SetBlendColor(0,0,0);
 sprintf(FileName,"%ssprites\\leftframe.tga",Path);
 cObject_LeftWall.Load(FileName);
 cObject_LeftWall.SetSpriteSize(17,23);
 cObject_LeftWall.SetBlendColor(0,0,0);
 delete(FileName);
 int n,m;
 //создадим маски для растворения блоков
 for(n=0;n<150;n++) Dust[0][n]=1;
 for(n=1;n<10;n++)
  for(m=0;m<150;m++)
  {
   Dust[n][m]=Dust[n-1][m];
   if (rand()>=RAND_MAX/2) Dust[n][m]=0;
  }
}
void CBlockMap::Release(void)
{
 cObject_Block.Delete();
 cObject_BlockPrize.Delete();
 cObject_UpWall.Delete();
 cObject_DownWall.Delete();
 cObject_LeftWall.Delete();
}
//--------------------------------------------------------------------------
SBlock CBlockMap::GetBlock(int x,int y)
{
 return(sBlock_Array[y][x]);
}
//--------------------------------------------------------------------------
bool CBlockMap::LoadMap(char *Path)
{
 int n;
 int x,y;
 char *FileName=new char[strlen(Path)+255];
 //загружаем требуемый уровень
 sprintf(FileName,"%slevels\\level_%i.klv",Path,sGameParameters.Level);
 FILE *file=fopen(FileName,"rb");
 if (file==NULL)
 {
  delete(FileName);
  return(false);
 }
 for(y=0;y<12;y++)
  for(x=0;x<25;x++)
  {
   int Space,Prize,Image,Break;
   fread(&Space,1,sizeof(Space),file);
   if (Space!=1)
   {
    fread(&Image,1,sizeof(Image),file);
    fread(&Break,1,sizeof(Break),file);
    fread(&Prize,1,sizeof(Prize),file);
    sBlock_Array[y][x].ImageX=Image;
    sBlock_Array[y][x].ImageY=0;
    sBlock_Array[y][x].Prize=Prize;
    sBlock_Array[y][x].Break=Break;
    sBlock_Array[y][x].Obstacle=true;
    sBlock_Array[y][x].Dust=0;
	sBlock_Array[y][x].PrizeActive=false;
   }
   if (Space==0) sBlock_Array[y][x].Space=false;
            else sBlock_Array[y][x].Space=true;
  }
 for(n=1;n<10;n++)
 {
  unsigned char state;
  fread(&state,1,sizeof(state),file);
  if (state==0) sGameParameters.EnableCharacter[n]=false;
           else sGameParameters.EnableCharacter[n]=true;
 }
 fclose(file);
 delete(FileName);
 return(true);
}
void CBlockMap::PutMap(void)
{
 int n;
 cObject_DownWall.Put(0,243);
 cObject_UpWall.Put(0,31);
 if (sGameParameters.RightWall==1) for(n=0;n<8;n++) cObject_LeftWall.Put(462,n*23+32+28);
 for(n=0;n<8;n++) cObject_LeftWall.Put(0,n*23+32+28);
 for(int x=0;x<25;x++)
  for(int y=0;y<12;y++)
  {
   if (sBlock_Array[y][x].Space==true) continue;
   if (sBlock_Array[y][x].Prize==-1)
   {
    cObject_Block.SetSpriteCoord(sBlock_Array[y][x].ImageY,sBlock_Array[y][x].ImageX);
    if (sBlock_Array[y][x].Dust==0) cObject_Block.Put(100+x*10,62+y*15);
                               else cObject_Block.Put(100+x*10,62+y*15,Dust[sBlock_Array[y][x].Dust]);
   }
   if (sBlock_Array[y][x].Prize!=-1 && sBlock_Array[y][x].PrizeActive==false)
   {
    cObject_Block.SetSpriteCoord(sBlock_Array[y][x].ImageY,sBlock_Array[y][x].ImageX);
    if (sBlock_Array[y][x].Dust==0) cObject_Block.Put(100+x*10,62+y*15);
                               else cObject_Block.Put(100+x*10,62+y*15,Dust[sBlock_Array[y][x].Dust]);
   }
   if (sBlock_Array[y][x].Prize!=-1 && sBlock_Array[y][x].PrizeActive==true)
   {
    cObject_BlockPrize.SetSpriteCoord(sBlock_Array[y][x].ImageY,sBlock_Array[y][x].ImageX);
    if (sBlock_Array[y][x].Dust==0) cObject_BlockPrize.Put(100+x*10,62+y*15);
                               else cObject_BlockPrize.Put(100+x*10,62+y*15,Dust[sBlock_Array[y][x].Dust]);
   }
  }
 //рисуем статистику
 char string[255];

 sprintf(string,"Уровень:%i",sGameParameters.Level);
 cFont.PrintAt(0,5,string,true);
 sprintf(string,"Жизней:%i",sGameParameters.Life);
 cFont.PrintAt(150,5,string,true);
 sprintf(string,"Счет:%i",sGameParameters.Score);
 cFont.PrintAt(290,5,string,true);
}
void CBlockMap::ActivateBlock(int x,int y)
{
 sBlock_Array[y][x].Break--;
 if (sBlock_Array[y][x].Break==0)
 {
  if (sBlock_Array[y][x].Prize==-1)
  {
   cSoundControl.Play(SOUND_BREAK);
   sBlock_Array[y][x].Obstacle=false;//если блок не призовой
   sGameParameters.Score+=100;//начислим очки
  }
  else //если блок приз - откроем его
  {
   if (sBlock_Array[y][x].PrizeActive==false)//открытие приза
   {
    cSoundControl.Play(SOUND_PRIZE_OPEN);
    sBlock_Array[y][x].Break=1;
    sBlock_Array[y][x].ImageX=4;
    sBlock_Array[y][x].ImageY=sBlock_Array[y][x].Prize;
    sBlock_Array[y][x].PrizeActive=true;
	sBlock_Array[y][x].PrizeTime=1000;
	sGameParameters.Score+=10;
   }
   else//задействуем приз
   {
    sGameParameters.Score+=1000;//начислим очки
    sBlock_Array[y][x].Obstacle=false;//приз задействован
    if (sBlock_Array[y][x].Prize==0) ExplosionBlock(x,y);//приз B-бомба
    if (sBlock_Array[y][x].Prize==1)//приз S-стенка
	{
     cSoundControl.Play(SOUND_PRIZE_S);
     sGameParameters.RightWall=true;
	 sGameParameters.RightWallTime=500;
     cPaddle.MagnetModeOff();
	 cPaddle.MissileModeOff();
	}
	if (sBlock_Array[y][x].Prize==2)//приз M-ракета
	{
     cSoundControl.Play(SOUND_MISSILE_ON);
	 cPaddle.MissileModeOn();
	 cPaddle.MagnetModeOff();
	}
    if (sBlock_Array[y][x].Prize==3)//приз X-жизнь
	{
     cSoundControl.Play(SOUND_PRIZE_X);
     if (sGameParameters.Practics==false) sGameParameters.Life++;
	}
	if (sBlock_Array[y][x].Prize==4)//приз G-прилипание мяча
	{
     cSoundControl.Play(SOUND_PRIZE_G);
	 cPaddle.MagnetModeOn();
     cPaddle.MissileModeOff();
	 cPaddle.NormalMode();
	}
	if (sBlock_Array[y][x].Prize==5)//приз E-удвоенная битка
	{
     cSoundControl.Play(SOUND_PRIZE_E);
	 cPaddle.DoubleMode();
     cPaddle.MissileModeOff();
     cPaddle.MagnetModeOff();
	}
	if (sBlock_Array[y][x].Prize==6)//приз +-ускорение мячей
	{
     cSoundControl.Play(SOUND_PRIZE_PLUS);
	 sGameParameters.BallSpeedPrizeTime=300;
	 sGameParameters.BallSpeed++;
	 if (sGameParameters.BallSpeed>3) sGameParameters.BallSpeed=3;
	}
	if (sBlock_Array[y][x].Prize==7)//приз --замедление мячей
	{
     cSoundControl.Play(SOUND_PRIZE_MINUS);
	 sGameParameters.BallSpeedPrizeTime=300;
   	 sGameParameters.BallSpeed--;
	 if (sGameParameters.BallSpeed<1) sGameParameters.BallSpeed=1;
	}
   }
  }
 }
 else
 {
  sGameParameters.Score+=10;
  int sound=(6*rand())/RAND_MAX;
  if (sound==0) cSoundControl.Play(SOUND_IMPACT_1);
  if (sound==1) cSoundControl.Play(SOUND_IMPACT_2);
  if (sound==2) cSoundControl.Play(SOUND_IMPACT_3);
  if (sound==3) cSoundControl.Play(SOUND_IMPACT_4);
  if (sound==4) cSoundControl.Play(SOUND_IMPACT_5);
  if (sound==5) cSoundControl.Play(SOUND_IMPACT_6);
 }
}
void CBlockMap::ChangeImageFrame(void)
{
 if (sGameParameters.BallSpeedPrizeTime>0)
 {
  sGameParameters.BallSpeedPrizeTime--;
  if (sGameParameters.BallSpeedPrizeTime==0) sGameParameters.BallSpeed=2;
 }
 if (sGameParameters.RightWall==1)
 {
  if (sGameParameters.RightWallTime>0) sGameParameters.RightWallTime--;
  if (sGameParameters.RightWallTime==0)
  {
   cSoundControl.Play(SOUND_PRIZE_S_END);
   sGameParameters.RightWall=false;
  }
 }
 for(int x=0;x<25;x++)
  for(int y=0;y<12;y++)
  {
   if (sBlock_Array[y][x].Space==true) continue;
   if (sBlock_Array[y][x].Obstacle==false)//растворение блока
   {
    sBlock_Array[y][x].Dust++;
	if (sBlock_Array[y][x].Dust==10) sBlock_Array[y][x].Space=true;
   }
   if (sBlock_Array[y][x].Prize!=-1)//блок призовой
   {
    if (sBlock_Array[y][x].PrizeActive==1)
	{
     if (sBlock_Array[y][x].ImageX>0)
	 {
      sBlock_Array[y][x].ImageX--;
	 }
	 sBlock_Array[y][x].PrizeTime--;
	 if (sBlock_Array[y][x].PrizeTime==0) sBlock_Array[y][x].Obstacle=false;//уничтожим приз
	}
   }
  }
}
void CBlockMap::ExplosionBlock(int x,int y)
{
 cSoundControl.Play(SOUND_PRIZE_B);
 int x1=x-1;
 int x2=x+1;
 int y1=y-1;
 int y2=y+1;
 if (x1<0) x1=0;
 if (y1<0) y1=0;
 if (x2>24) x2=24;
 if (y2>11) y2=11;
 for(int xc=x1;xc<=x2;xc++)
  for(int yc=y1;yc<=y2;yc++) DestroyBlock(xc,yc);
}
void CBlockMap::DestroyBlock(int x,int y)
{
 if (sBlock_Array[y][x].Space==1) return;
 sBlock_Array[y][x].Obstacle=false;
 sGameParameters.Score+=10;
}
int CBlockMap::LevelOver(void)
{
 for(int x=0;x<25;x++)
  for(int y=0;y<12;y++)
  {
   if (sBlock_Array[y][x].Space==false) return(0);
  }
 return(1);
}

