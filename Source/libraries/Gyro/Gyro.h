#ifndef GYRO_H
#define GYRO_H
#include <Wire.h>
#include <imu.h>
#include <Arduino.h>

class MyGyro 
{
  public:
  
MyGyro();
  
  long gyroAverage;
  static const int lange = 25;
  long gyroLast = 0;
  long gyroNow = 0;
  long gyroChanged = 0;
  long lastTime = 0;
  long nowTime = 0;
  long elapsedTime = 0;  

  void Update();

  
  long getUsefulNumber();

  void resetValue();

  private:
  
};

#endif
