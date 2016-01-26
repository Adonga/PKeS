#include <Wire.h>
#include <imu.h>
#include "Display.h"
#include "Gyro.h"
#include "Motor.h"
#include "Waterscale.h"
#include "IRC.h"
#include "ADConverter.h"

Display dis;
MyGyro mygyro;
Motor motor;
Waterscale scale;
ADConverter adc;

int s1[10]={272,217,179,153,138,130,125,118,111,110};
IRC irc1(s1);
int s2[10]={506,400,334,284,253,226,205,185,170,158};
IRC irc2(s2);


void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(57600);

  
  _delay_ms(100);
  Serial.println("0123456789");
  dis.ShowCleared();

  Wire.begin();

    // Clear the 'sleep' bit to start the sensor.
    MPU9150_writeSensor(MPU9150_PWR_MGMT_1, 0);

    MPU9150_setupCompass();

  for (int i =0;i<mygyro.lange;i++)
    {
      mygyro.gyroAverage += MPU9150_readSensor(MPU9150_GYRO_ZOUT_L,MPU9150_GYRO_ZOUT_H);
      _delay_ms(100);
    }
    mygyro.gyroAverage /= mygyro.lange;

    motor.init();

   Serial.println(TCCR1A);
   Serial.println(TCCR1B);
   Serial.println(TCCR4A);
   Serial.println(TCCR4B);
}



char t=0;
void loop() 
{
  // put your main code here, to run repeatedly:

  
  int b=adc.convert();
  irc1.addvalue(b);
  int k=adc.convert(1);
  irc2.addvalue(k);
  //scale.Update();
  mygyro.Update();
  int h;

  int t1 = irc2.cValue;
  int t2 = irc1.cValue;

  switch(t){
    case 0:
          dis.ShowCleared();
          break;
    case 1:
          irc1.showConverted(&dis);
          break;
    case 2:
          irc2.showConverted(&dis);
          break;
    case 3:
          h=mygyro.getUsefulNumber();
          dis.showSmallNumber(h);
          break;
    case 4:
          h=motor.Update(t1,t2, &mygyro);
          if(h)motor.ChangeMode(Drive);
          break;

    case 5:
          motor.Stop();
          t=0;
          break;
    case 6:
          motor.cMode=Rotate;
          h=motor.Update(t1,t2, &mygyro);
          dis.showSmallNumber(mygyro.getUsefulNumber());
          if(h)t=5;
          break;
    case 7:
          motor.dir=Forward;
          motor.ChangeSpeed(Fast);  
          break;

    case 8:
          mygyro.resetValue();
          t=3;
          break;
    case 9:
          motor.dir=Backward;
          motor.ChangeSpeed(Fast);
          break;
    default:
    t=0;
    Serial.println("default");break;
  
    
  }
 
  _delay_ms(100);
//*
                // read the incoming byte:
                byte incomingByte = Serial.read();
                //Serial.println(incomingByte);
                switch(incomingByte)
                {
                  case 's':t=0;break;
                  case '1':t=1;break;
                  case '2':t=2;break;
                  case '3':t=3;break;
                  case '4':t=4;break;
                  case '5':t=5;break;
                  case '6':t=6;break;
                  case '7':t=7;break;
                  case '8':t=8;break;
                  case '9':t=9;break;
                  case 'a':t=10;break;
                  case 'b':t=11;break;
                  case 'c':t=12;break;
                  case 'd':t=13;break;
                  case 'e':t=14;break;
                  case 'f':t=15;break;
                  case 'g':t=16;break;
                  case 'h':t=17;break;
                  default: break;
                }
//*/
  
  /*
  double dT = ( (double) MPU9150_readSensor(MPU9150_TEMP_OUT_L,MPU9150_TEMP_OUT_H) + 12412.0) / 340.0;
  Serial.print(dT);
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_CMPS_XOUT_L,MPU9150_CMPS_XOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_CMPS_YOUT_L,MPU9150_CMPS_YOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_CMPS_ZOUT_L,MPU9150_CMPS_ZOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_GYRO_XOUT_L,MPU9150_GYRO_XOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_GYRO_YOUT_L,MPU9150_GYRO_YOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_GYRO_ZOUT_L,MPU9150_GYRO_ZOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_ACCEL_XOUT_L,MPU9150_ACCEL_XOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_ACCEL_YOUT_L,MPU9150_ACCEL_YOUT_H));
  Serial.print("  ");
  Serial.print(MPU9150_readSensor(MPU9150_ACCEL_ZOUT_L,MPU9150_ACCEL_ZOUT_H));
  Serial.println();
  delay(100);
  //*/
}