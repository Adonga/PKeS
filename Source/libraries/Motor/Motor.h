#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
enum Direction{
  Stay,Forward,Backward,Right,Left
};

enum SpeedMode{
  Zero=0,VerySlow=64,Slow=86,Middle=128,Fast=255
};

enum MoveMode{
  Drive,Rotate,Wait
};

class Motor
{
public:

int s1[10]={272,217,179,153,138,130,125,118,111,110};
IRC irc1(s1);
int s2[10]={506,400,334,284,253,226,205,185,170,158};
IRC irc2(s2);

  Direction dir=Stay;
  SpeedMode currentSpeed=Zero;
  MoveMode cMode=Drive;
  Motor();
  

  void init();

  long oldtime=millis();
  long timewait=0;
  char h=0;
  char g=0;
  int Update();

  void bla();

  void Stop();


  void ChangeSpeed(SpeedMode mode);

  void ChangeMode(MoveMode mm);


  void ChangeMove(Direction now);
  
  
};

#endif
