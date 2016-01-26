#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include "Gyro.h"
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


  Direction dir=Stay;
  SpeedMode currentSpeed=Zero;
  MoveMode cMode=Drive;
  Motor();
  

  void init();

  long oldtime=millis();
  long timewait=0;
  char h=0;
  char g=0;
  int Update(int t1, int t2, MyGyro *mygyro);

  void bla();

  void Stop();


  void ChangeSpeed(SpeedMode mode);

  void ChangeMode(MoveMode mm);


  void ChangeMove(Direction now);
  
  
};

#endif
