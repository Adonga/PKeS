#include "Gyro.h"
  
MyGyro::MyGyro()
  {
  }
  
 void MyGyro::Update()
  {
    gyroLast = gyroNow;
    gyroNow = MPU9150_readSensor(MPU9150_GYRO_ZOUT_L,MPU9150_GYRO_ZOUT_H);
    lastTime = nowTime;
    nowTime = millis();

   long tmp =  ( gyroNow - gyroAverage );
   tmp = tmp/100;   
   if( gyroAverage +5 < gyroNow || gyroNow < gyroAverage -5  )
    {
      gyroChanged += tmp;
    }
   
    elapsedTime = nowTime - lastTime;     
  }

 
  long MyGyro::getUsefulNumber()
  {
    const int magicNumber = 8; //why?
    int tmp = gyroChanged * magicNumber; //
    int tmp2 = elapsedTime;
   
    return tmp/tmp2 ;
  }

  void MyGyro::resetValue()
  {
    gyroChanged=0;
  }
  
  void MyGyro::setValue(int value)
  {
	gyroChanged=value*12;//the other magic number
  }
