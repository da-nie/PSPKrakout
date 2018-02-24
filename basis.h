#ifndef BASIS_H_INCLUDED
#define BASIS_H_INCLUDED

#include <stdio.h>

struct SGameParameters
{
 int Life;//�����
 int Score;//����
 int Level;//�������
 int BallSpeed;//�������� �����
 int BallSpeedPrizeTime;//����� ������ ������ ��������� � ����������
 bool RightWall;//true-���������� ������ ������
 int RightWallTime;//����� ������������� ������ ������
 int StartLevel;//��������� ������� ��� ��������
 bool EnableCharacter[10];//true-�������� �������� �� ������
 bool Practics;//true-����� ��������
 bool Demonstrations;//true-����� ������������
 int ControlMode;//����� ����������
 int LevelAmount;//���������� ��������� �������
 int SelectBlock_X;//��������� ���� ��� ��������������� ���������� ������
 int SelectBlock_Y;
};
//--------------------------------------------------------------------------
int RND(int Max);
float RNDf(float Max);
void Normalize(float &vx,float &vy);
void Refraction(float vx,float vy,float nx,float ny,float &rx,float &ry);
int CircleIntercectionToCircle(float xc1,float yc1,float rc1,float xc2,float yc2,float rc2);
int CircleIntercectionToLine(float x1,float y1,float x2,float y2,float xc,float yc,float r);
bool FindStringInFile(FILE *File,char *SearchString);
bool GetParameters(char *FileName,char *ParametersName,int *parameters);
#endif
