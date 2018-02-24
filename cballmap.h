#ifndef CBALLMAP_H_INCLUDED
#define CBALLMAP_H_INCLUDED

#include "cobject.h"

struct SBall
{
 int Radius;//������ ����
 float X;//���������� ������ ����
 float Y;
 float Vx;//������ ����� ����
 float Vy;
 int Mode;//��������� ���� (0-������� �� ������, 1-�����)
 int Image;//����� �������� ����
 int WaitTime;//����� �������� ���� �����
 bool NoWaitTime;//true-��� ������ ��-�� ����� G
 int MagnetDy;//�������� ��� ���������� ���� ������������ ������ �����
};
class CBallMap
{
 protected:
  //-----------------------------------------------------------
  CObject cObject_Ball;//������� ����
  SBall sBall_Array[100];//����
  int CurrentBall;//������� ����� �����
  int Frame;//����� ������ ����
  //-----------------------------------------------------------
 public:
  //-----------------------------------------------------------
  void Create(char *Path);//������� ������ BALLMAP
  void Release(void);//���������� ������ BALLMAP
  //-----------------------------------------------------------
  int GetCurrentBall(void);//�������� ������� ����� �����
  void GetBallCoord(int ball,float &x,float &y);//�������� ���������� ����
  void GetBallVector(int ball,float &vx,float &vy);//�������� ������ ����� ����
  //-----------------------------------------------------------
  void InitializeToStartLevel(void);//���������������� ���� �� ������ ������
  //-----------------------------------------------------------
  void CreateNewBall(float x,float y,float vx,float vy,int mode);//������� ����� ���
  void DeleteBall(int ball);//������� ���
  //-----------------------------------------------------------
  void PutAllBall(void);//������� ��� ����
  //-----------------------------------------------------------
  void ChangeImageFrame(void);//������� ���� ������� ����
  //-----------------------------------------------------------
  bool Processing(void);//������� ���������� ������
  bool MoveBall(int ball);//������� ���
  void StartAllBall(void);//��������� ��� ���� � ����
  void SetAllBallToPaddle(void);//������������ ��� � ����� (�� �����������)
  int GetNearestBallNumber(float x,float y);//�������� ��������� � ����������� ���
  int GetNearestToVerticalLineBallNumber(float x);//�������� ��������� � ����������� ����� ��� (����� ��� ������������)
  //-----------------------------------------------------------
};
#endif
