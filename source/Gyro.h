#include <Wire.h>
#include <imu.h>


class MyGyro 
{
  public:
  
MyGyro()
  {
  }
  
  long gyroAverage;
  static const int lange = 25;
  long gyroLast = 0;
  long gyroNow = 0;
  long gyroChanged = 0;
  long lastTime = 0;
  long nowTime = 0;
  long elapsedTime = 0;  

  void Update()
  {
    gyroLast = gyroNow;
    gyroNow = MPU9150_readSensor(MPU9150_GYRO_ZOUT_L,MPU9150_GYRO_ZOUT_H);
    lastTime = nowTime;
    nowTime = millis();

   long lastChange = gyroChanged;
   long tmp =  ( gyroNow - gyroAverage );
   tmp = tmp/100;   
   if( gyroAverage +5 < gyroNow || gyroNow < gyroAverage -5  )
    {
      gyroChanged += tmp;
    }
    
    elapsedTime = nowTime - lastTime;     
    if(lastChange == gyroChanged )
    {
      gyroChanged =  gyroChanged ;
    }
  }

  
  long getUsefulNumber()
  {
    const int magicNumber = 8; //why?
    int tmp = gyroChanged * magicNumber; //
    int tmp2 = elapsedTime;
   
    return tmp/tmp2 ;
  }
  private:
  
};
