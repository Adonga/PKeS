#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include "Gyro.h"
#include "IRC.h"
#include "control.h"
#include "Odometire.h"

enum Direction{
  Stay,Forward,Backward,Right,Left
};

enum SpeedMode{
  Zero=0,VeryVerySlow=148,VerySlow=152,Slow=175,Middle=200,LittleFast=220,Fast=255,BLA=150
};

enum MoveMode{
  Drive,DriveBy,Rotate,Wait
};

class Motor
{

public:


  Direction dir=Stay;
  SpeedMode currentSpeed=Zero;
  MoveMode cMode=Drive;

  int driveDistance=0;//in mm

  bool done=false;
  bool invert=false;

  Motor();
  

  void init();

  long oldtime=millis();
  long timewait=0;
  char h=0;
  char g=0;
	int limit = 0;
  int Update( IRC *irc1, IRC *irc2, MyGyro *mygyro ,Control::LR *leftright, Odometrie *odo);


  void Stop();


  void ChangeSpeed( Control::LR *leftright );
	void ChangeSpeed( SpeedMode mode );

  void ChangeMode(MoveMode mm);


  void ChangeMove(Direction now);
	
	int offSetLeft = 0;
	int offSetRight= 0;
  
	int currentSpeedLR[2];

  
};

#endif
