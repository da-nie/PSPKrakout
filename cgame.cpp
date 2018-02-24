#include <pspctrl.h>
#include <math.h>

#include "cgame.h"
#include "video.h"
#include "cballmap.h"
#include "cpaddle.h"
#include "cblockmap.h"
#include "cmissilemap.h"
#include "ccharactermap.h"
#include "cfont.h"
#include "basis.h"
#include "csoundcontrol.h"

extern CBallMap cBallMap;
extern CBlockMap cBlockMap;
extern CMissileMap cMissileMap;
extern CCharacterMap cCharacterMap;
extern CPaddle cPaddle;
extern CFont cFont;
extern SGameParameters sGameParameters;
extern CSoundControl cSoundControl;

extern bool done;
//--------------------------------------------------------------------------
CGame::CGame(void)
{
}
CGame::~CGame()
{
 Release();
}
void CGame::Create(char *Path)
{
 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssprites\\back.tga",Path);
 cSprite_Back.Load(FileName);
 delete(FileName);
}
void CGame::Release(void)
{
 cSprite_Back.Release();
}
//--------------------------------------------------------------------------
bool CGame::InitializeToStartGame(char *Path)
{
 cBallMap.InitializeToStartLevel();
 cMissileMap.InitializeToStartLevel();
 cPaddle.InitializeToStartLevel();
 cBallMap.SetAllBallToPaddle();
 cCharacterMap.InitializeToStartLevel();
 if (sGameParameters.Practics==true) sGameParameters.Level=sGameParameters.StartLevel;
                                else sGameParameters.Level=1;
 sGameParameters.Life=10;
 sGameParameters.Score=0;
 sGameParameters.BallSpeed=2;
 sGameParameters.RightWall=false;
 sGameParameters.RightWallTime=0;
 sGameParameters.SelectBlock_X=-1;
 sGameParameters.SelectBlock_Y=-1;
 return(cBlockMap.LoadMap(Path));
}
bool CGame::InitializeToStartNextLevel(char *Path)
{
 sGameParameters.Level++;
 if (cBlockMap.LoadMap(Path)==false)//������ ������ ���, ����� � �������
 {
  sGameParameters.Level=1;
  if (cBlockMap.LoadMap(Path)==false) return(false);//������ �������� ������� ������
 }
 sGameParameters.BallSpeed=2;
 sGameParameters.RightWall=false;
 sGameParameters.RightWallTime=0;
 cBallMap.InitializeToStartLevel();
 cPaddle.InitializeToStartLevel();
 cMissileMap.InitializeToStartLevel();
 cBallMap.SetAllBallToPaddle();
 cCharacterMap.InitializeToStartLevel();
 return(true);
}
void CGame::InitializeToReStartLevel(void)
{
 cBallMap.InitializeToStartLevel();
 cPaddle.InitializeToStartLevel();
 cMissileMap.InitializeToStartLevel();
 cBallMap.SetAllBallToPaddle();
 cCharacterMap.InitializeToStartLevel();

 sGameParameters.BallSpeed=2;
 sGameParameters.RightWall=false;
 sGameParameters.RightWallTime=0;
 sGameParameters.SelectBlock_X=-1;
 sGameParameters.SelectBlock_Y=-1;
}
void CGame::SaveScreenShot(char *Path)
{
 char *FileName=new char[strlen(Path)+256];
 //���� ��������� �������
 int index=1;
 while(done==false)
 {
  sprintf(FileName,"%sscreen/screen%i.tga",Path,index);
  SceUID SceUID_File;
  SceUID_File=sceIoOpen(FileName,PSP_O_RDONLY,0777);//��������� ����
  if (SceUID_File<=0) break;//������ ����� ���
  sceIoClose(SceUID_File);
  index++;
 }
 Video_SaveScreenAsTGA(FileName);
 delete(FileName);
}
void CGame::ComputerControl(void)
{
 //�������������� ���������� ������
 int selectball=cBallMap.GetNearestToVerticalLineBallNumber((float)(cPaddle.GetX()-cPaddle.GetWidth()/2));
 if (selectball>=0)
 {
  float bx,by;//���������� ����
  float vx;//��������� ��������� ����
  float vy;
  cBallMap.GetBallVector(selectball,vx,vy);
  cBallMap.GetBallCoord(selectball,bx,by);
  if (bx>=300 && vx>0)//���� ��� ������ � ����� � ����� �� ��
  {
   if (by>cPaddle.GetY()+cPaddle.GetHeight()/2) cPaddle.MoveDown();
   if (by<cPaddle.GetY()-cPaddle.GetHeight()/2) cPaddle.MoveUp();
   if (by<=cPaddle.GetY()+cPaddle.GetHeight()/2 && by>=cPaddle.GetY()-cPaddle.GetHeight()/2)//������������� ��������� �����
   {
    //���� �� ��� �� ������� ����, �� ������� ��� ������
	bool onselectblock=false;
	int x,y;
	SBlock sBlock;
    if (sGameParameters.SelectBlock_X!=-1 && sGameParameters.SelectBlock_Y!=-1)
    {
     sBlock=cBlockMap.GetBlock(sGameParameters.SelectBlock_X,sGameParameters.SelectBlock_Y);
     if (sBlock.Obstacle==0 || sBlock.Space==1) onselectblock=true;
    }
	else onselectblock=false;
    //������, �� ��������� �� �������� ����� �� ����
    //������ �������� (�� ���������� M-B-S-+-E-X-G--);
    int SelectLevel=0;//��� ���������� �����
    for(x=0;x<26;x++)
	{
     for(y=0;y<13;y++)
	 {
      sBlock=cBlockMap.GetBlock(x,y);
      if (sBlock.Space==1) continue;//��� ��� �����
	  if (sBlock.Obstacle==0) continue;//���� ����, �� �� ������������
      if (sBlock.Prize==-1) continue;//���� ��� �����
      if (sBlock.PrizeActive==0) continue;//���� �� ����������� (���������� �� �����)
      int level=-1;
      if (sBlock.Prize==0) level=7;//B
	  if (sBlock.Prize==1) level=6;//S
	  if (sBlock.Prize==2) level=8;//M
	  if (sBlock.Prize==3) level=3;//X
	  if (sBlock.Prize==4) level=1;//G
	  if (sBlock.Prize==5) level=5;//E
	  if (sBlock.Prize==6) level=4;//+
	  if (sBlock.Prize==7) level=2;//-
	  if (SelectLevel<=level)//���� � ����� ������� �����������
	  {
       sGameParameters.SelectBlock_X=x;
	   sGameParameters.SelectBlock_Y=y;
       SelectLevel=level;
	  }
	 }
	}
    if (SelectLevel!=0) onselectblock=false;
    if (onselectblock==true)
    {
     //����� ������� ����, �� ������� �� ������� ���
	 sGameParameters.SelectBlock_X=-1;
     sGameParameters.SelectBlock_Y=-1;
     int maxblock=0;
     for(x=0;x<26;x++)
      for(y=0;y<13;y++)
	  {
       sBlock=cBlockMap.GetBlock(x,y);
       if (sBlock.Space==1) continue;
       if (sBlock.Obstacle==0) continue;
       maxblock++;
	  }
     //������ �� ����� ������� � ��� ����� ���������� ������
     int search=(int)RND(maxblock);
     for(x=0;x<26;x++)
	 {
      for(y=0;y<13;y++)
	  {
       sBlock=cBlockMap.GetBlock(x,y);
       if (sBlock.Space==1) continue;
	   if (sBlock.Obstacle==0) continue;
       sGameParameters.SelectBlock_X=x;
	   sGameParameters.SelectBlock_Y=y;
	   search--;
	   if (search==0) break;
	  }
      if (search==0) break;
	 }
	}
    //�������� ��������� �����
    if (sGameParameters.SelectBlock_X!=-1 && sGameParameters.SelectBlock_Y!=-1)
    {
     //���������� ������ �����
     float bc_x=(float)(sGameParameters.SelectBlock_X*10+100+5);
     float bc_y=(float)(sGameParameters.SelectBlock_Y*15+62+7.5);
     //������ ��������� ���� �� ����
     float rx=bc_x-bx;
     float ry=(bc_y-by);
     Normalize(rx,ry);
     //������ �� ����� ������ ��������� ����, ��������� ����� ������ ���� ������� � �����
     float nx=(rx-vx)/2;
     float ny=(ry-vy)/2;
     Normalize(nx,ny);
     //���������, ����� �� ���� ������
     if (ny>1) ny=1;
     if (ny<-1) ny=-1;
     float dangle=180.0/M_PI*asin(ny);//���� �������
     float dy=dangle/10.0;//�������� �� ������
     dy*=(cPaddle.GetWidth()/2);
     float paddle_y=cPaddle.GetY()+dy;
     //������������ ��������� �����
	 if (paddle_y>cPaddle.GetY()) cPaddle.MoveDown();
	 if (paddle_y<cPaddle.GetY()) cPaddle.MoveUp();
    }
   }
  }
  else
  {
   if (cPaddle.GetMissile()==1)//���� ���� ������
   {
    int block_y=0;
	int maxblock_in_line=0;
    for(int y=9;y>=1;y--)
	{
     int block_in_line=0;
     for(int x=0;x<=24;x++)
     {
      SBlock sBlock=cBlockMap.GetBlock(x,y);
      if (sBlock.Obstacle==0 || sBlock.Space==1) continue;
      block_in_line++;
     }
     if (maxblock_in_line<block_in_line)
	 {
	  maxblock_in_line=block_in_line;
	  block_y=y;
	 }
	}
    float up_by=(float)(block_y*15+62+5);
    float down_by=(float)(block_y*15+62+10);
    if (up_by>cPaddle.GetY()) cPaddle.MoveDown();
	if (down_by<cPaddle.GetY()) cPaddle.MoveUp();
	if (cPaddle.GetY()<=down_by && cPaddle.GetY()>=up_by)
	{
     cSoundControl.Play(SOUND_MISSILE);
     cPaddle.MissileModeOff();
     cMissileMap.CreateNewMissile((float)cPaddle.GetX(),(float)cPaddle.GetY());
	}
   }
  }
 }
 cBallMap.StartAllBall();
}



void CGame::StartGame(char *Path)
{
 if (InitializeToStartGame(Path)==false) return;//��� ������ ������
 int frame=0;
 SceCtrlData pad;
 u64 tick_frequency=sceRtcGetTickResolution();
 u64 begin_tick;
 u64 current_tick;
 sceRtcGetCurrentTick(&begin_tick);//������� �������� �������
 bool enable_control=false;
 while(!done)
 {
  if (enable_control==false)
  {
   sceRtcGetCurrentTick(&current_tick);//������� �������� �������
   double time_span=((double)(current_tick-begin_tick))/(double)tick_frequency;//��������� �����
   if (time_span>0.3) enable_control=true;
  }
  if (enable_control==true)//������� ���-������.
  {
   //���������� ����������� �� ����������
   sceCtrlReadBufferPositive(&pad,1);
   if (pad.Buttons&PSP_CTRL_LTRIGGER)
   {
    cFont.PrintAt(25,150,(char*)"��� ����������� ������� ������� R.",true);
    Video_ViewDisplayPage();
    while(!done)
    {
     sceCtrlReadBufferPositive(&pad,1);
     if (pad.Buttons&PSP_CTRL_RTRIGGER) break;
    }
   }
   if (pad.Buttons&PSP_CTRL_SELECT) SaveScreenShot(Path);//������� ��������
   if (pad.Buttons&PSP_CTRL_START) break;//����� � ����
   //��������� ������� ���������� � ������ ������������
   if (sGameParameters.Demonstrations==false)
   {
    if (sGameParameters.ControlMode==0)
    {
     if (pad.Buttons&PSP_CTRL_TRIANGLE) cPaddle.MoveUp();//����� �����
     if (pad.Buttons&PSP_CTRL_CROSS) cPaddle.MoveDown();//����� ����
    }
    if (sGameParameters.ControlMode==1)
    {
     if (pad.Buttons&PSP_CTRL_UP) cPaddle.MoveUp();//����� �����
     if (pad.Buttons&PSP_CTRL_DOWN) cPaddle.MoveDown();//����� ����
    }
    if (sGameParameters.ControlMode==2)
    {
     int delta=(pad.Ly-127);
     if (fabs(delta)>15)
     {
      int offset=log(fabs(delta));
      for(int d=0;d<offset;d++)
      {
       if (delta<0) cPaddle.MoveUp();//����� �����
       if (delta>0) cPaddle.MoveDown();//����� ����
      }
     }
    }
    if ((pad.Buttons&PSP_CTRL_SQUARE && sGameParameters.ControlMode==0) || //������ ����� � ������
        (pad.Buttons&PSP_CTRL_CROSS && (sGameParameters.ControlMode==1 || sGameParameters.ControlMode==2)))
    {
     cBallMap.StartAllBall();
     if (cPaddle.GetMissile()==true)
     {
      cSoundControl.Play(SOUND_MISSILE);
      cPaddle.MissileModeOff();
      cMissileMap.CreateNewMissile((float)cPaddle.GetX(),(float)cPaddle.GetY());
     }
    }
   }
  }
  //� ������ ������������ ��������� ������ ���������
  if (sGameParameters.Demonstrations==true) ComputerControl();
  //���������, �� �������� �� ��
  if (cBlockMap.LevelOver()==1)//������!
  {
   if (InitializeToStartNextLevel(Path)==false) return;//������
  }
  //��������� �������� � ����� ��������
  frame++;
  frame%=3;
  cSprite_Back.Put(0,0,false);
  cBallMap.PutAllBall();
  cBlockMap.PutMap();
  cMissileMap.PutAllMissile();
  cCharacterMap.PutAllCharacter();
  cPaddle.PutPaddle();
  if (frame==0)
  {
   cBallMap.ChangeImageFrame();
   cBlockMap.ChangeImageFrame();
   cMissileMap.ChangeImageFrame();
   cCharacterMap.ChangeImageFrame();
  }
  cPaddle.Processing();
  cMissileMap.Processing();
  cCharacterMap.Processing();
  //��������� ������ �����
  bool noball=false;
  for(int s=0;s<sGameParameters.BallSpeed;s++)
  {
   noball=cBallMap.Processing();
   if (noball==true) break;
  }
  if (noball==true)//����������� ����
  {
   cSoundControl.Play(SOUND_LOSE);
   while(cSoundControl.CheckEndAllSound()==false);
   cCharacterMap.DestroyAllCharacter();
   if (sGameParameters.Practics==false )//���� � ��� �� ����� ��������
   {
    sGameParameters.Life--;
    if (sGameParameters.Life==0) return;//����� ����
   }
   InitializeToReStartLevel();//������������� �������
  }
  Video_ViewDisplayPage();
  Video_ChangeDisplayPage();
 }
}
