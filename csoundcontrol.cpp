#include <string.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <psprtc.h>

#include "csoundcontrol.h"

extern CSound cSound;

CSoundControl cSoundControl;

CSoundControl::CSoundControl(void)
{
 LastID=-1;
}

void CSoundControl::Create(char *Path)
{
 char *FileName=new char[strlen(Path)+255];
 sprintf(FileName,"%ssound\\k_impact_1.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_IMPACT_1);
 sprintf(FileName,"%ssound\\k_impact_2.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_IMPACT_2);
 sprintf(FileName,"%ssound\\k_impact_3.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_IMPACT_3);
 sprintf(FileName,"%ssound\\k_impact_4.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_IMPACT_4);
 sprintf(FileName,"%ssound\\k_impact_5.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_IMPACT_5);
 sprintf(FileName,"%ssound\\k_impact_6.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_IMPACT_6);

 sprintf(FileName,"%ssound\\k_break.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_BREAK);

 sprintf(FileName,"%ssound\\k_prize_open.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_PRIZE_OPEN);

 sprintf(FileName,"%ssound\\k_x.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_PRIZE_X);
 sprintf(FileName,"%ssound\\k_b.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_PRIZE_B);
 sprintf(FileName,"%ssound\\k_e.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_PRIZE_E);
 sprintf(FileName,"%ssound\\k_g.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_PRIZE_G);
 sprintf(FileName,"%ssound\\k_s.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_PRIZE_S);
 sprintf(FileName,"%ssound\\k_s_end.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_PRIZE_S_END);
 sprintf(FileName,"%ssound\\k_speed_+.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_PRIZE_PLUS);
 sprintf(FileName,"%ssound\\k_speed_-.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_PRIZE_MINUS);

 sprintf(FileName,"%ssound\\k_missile.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_MISSILE);
 sprintf(FileName,"%ssound\\k_missile_on.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_MISSILE_ON);

 sprintf(FileName,"%ssound\\k_destroy.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_DESTROY);
 sprintf(FileName,"%ssound\\k_dollar.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_DOLLAR);
 sprintf(FileName,"%ssound\\k_double_ball.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_DOUBLE_BALL);

 sprintf(FileName,"%ssound\\k_birth.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_BIRTH);

 sprintf(FileName,"%ssound\\k_lose.ksd",Path);
 cSound.LoadNewSound(FileName,SOUND_LOSE);

 delete(FileName);
}
void CSoundControl::Play(int ID)
{
 if (LastID==ID)
 {
  u64 tick_frequency=sceRtcGetTickResolution();//текущее значение частоты
  u64 current_tick;
  sceRtcGetCurrentTick(&current_tick);//текущее значение частоты
  double time_span=((double)(current_tick-LastID_tick))/(double)tick_frequency;//прошедшее время
  if (time_span<0.1) return;//почти одновременные одинаковые звуки запрещаем
 }
 cSound.Play(ID);
 LastID=ID;
 sceRtcGetCurrentTick(&LastID_tick);//запоминаем время
}
void CSoundControl::StopAllSound(void)
{
 LastID=-1;
 cSound.StopAllSound();
}
bool CSoundControl::CheckEndAllSound(void)
{
 return(cSound.CheckEndAllSound());
}
