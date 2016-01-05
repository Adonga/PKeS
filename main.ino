#include <Wire.h>
#include <imu.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  _delay_ms(100);
  Serial.println("123456789");
  dis.ShowCleared();
  Wire.begin();

    // Clear the 'sleep' bit to start the sensor.
    MPU9150_writeSensor(MPU9150_PWR_MGMT_1, 0);

    MPU9150_writeSensor( MPU9150_CONFIG, 0x4 ); //  6 == 5Hz, 4 == 20Hz,  0==256Hz

    MPU9150_setupCompass();

    for (int i =0;i<mygyro.lange;i++)
    {
      mygyro.gyroAverage += MPU9150_readSensor(MPU9150_GYRO_ZOUT_L,MPU9150_GYRO_ZOUT_H);
      _delay_ms(100);
    }
    mygyro.gyroAverage /= mygyro.lange;
    unsigned long StartTime = millis();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  mygyro.Update();
  dis.showSmallNumber( mygyro.getUsefulNumber() );
  if(mygyro.gyroChanged != 0)
  {
   Serial.print("GyroChange    " );
   Serial.print( mygyro.getUsefulNumber() );
   Serial.print("  GyroValue    " );
   Serial.println(mygyro.gyroNow );
  }
 _delay_ms(100);
}
