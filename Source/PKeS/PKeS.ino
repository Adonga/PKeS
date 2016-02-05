#include <Wire.h>
#include <imu.h>
#include "Display.h"
#include "Gyro.h"
#include "Motor.h"
#include "Waterscale.h"
#include "IRC.h"
#include "ADConverter.h"
#include "Odometire.h"
#include "PID.h"



class InputCache
{
  static const int CacheSize=32;
  char types[CacheSize];
  int values[CacheSize];

  int CSPointer=0;
  int CEPointer=0;
  int CSize=0;
public:
     
  void AddValue(char type,int val)
  {
      if(CSize==CacheSize)
      {
      Serial.print("Cache full, the following Command Cant be added:");
      Serial.print(type);
      Serial.print(" ");
      Serial.println(val);  
      }
      else
      {
       types[CEPointer]=type;
       values[CEPointer]=val;
       CEPointer=(CEPointer+1)%CacheSize;
       CSize++; 
      }
  }

  void GetValues(char& type,int& value)
  {
    if(CSPointer==CEPointer)
    {
      type='f';
      value=0;
    }
    else
    {
    type=types[CSPointer];
    value=values[CSPointer];
    CSPointer=(CSPointer+1)%CacheSize;
    CSize--;
    }
    
  }
};




Display dis;
MyGyro mygyro;
Motor motor;
Waterscale scale;
ADConverter adc;
InputCache ICache;

int s1[10]={272,217,179,153,138,130,125,118,111,110};
IRC irc1(s1);
int s2[10]={506,400,334,284,253,226,205,185,170,158};
IRC irc2(s2);


   Odometrie odo;

  ISR( INT2_vect )
  {
   motor.interrupt1++;  //rechts
  }
  
  ISR ( PCINT1_vect )
  {
    motor.interrupt2++; //links
  }

  bool a = true;

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
    odo.init();
    _delay_ms(10);
    motor.m_pid.setTarget(50);
   Serial.println(TCCR1A);
   Serial.println(TCCR1B);
   Serial.println(TCCR4A);
   Serial.println(TCCR4B);
  
}


static byte km=0;
static int val=0;
static bool otherin=false;
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

          Serial.print("interrupt1r: ");
          Serial.print(motor.interrupt1);
          Serial.print("    interrupt2l ");
          Serial.println(motor.interrupt2);


  switch(t){
    case 0:
          dis.ShowCleared();
          motor.Stop();
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
          h=motor.Update(&irc1,&irc2, &mygyro );
          if(h)motor.ChangeMode(Drive);
          break;

    case 5:
          motor.cMode=Rotate;
          h=motor.Update(&irc1,&irc2,&mygyro );
          dis.showSmallNumber(mygyro.getUsefulNumber());
          break;
    case 6:
          OCR1A  = km;
          OCR1B  = 0;
          OCR4A  = km;
          OCR4B  = 0;  
          Serial.println(km);
          break;
    case 7:
          motor.dir=Backward;
          motor.ChangeSpeed(Middle);  
          
          break;

    case 8:
          motor.dir=Right;
          motor.ChangeSpeed(Middle);  
          break;
    case 9:
          motor.dir=Left;
          motor.ChangeSpeed(Middle);  
          break;
    default:
    t=0;
    Serial.println("default");break;
  
    
  }
 
 //if(a){_delay_ms(10000); a = false;}
  _delay_ms(100);
//*
                // read the incoming byte:
               byte incomingByte = Serial.read();
                //Serial.println(incomingByte);
                static char type=0;
                static int vorzeichen=1;
                static bool samevalue=true;
                if(otherin){
                  switch(incomingByte){
                    case '-':vorzeichen*=-1;break;
                    case '1':val=(val*10) +1;break;
                    case '2':val=(val*10) +2;break;
                    case '3':val=(val*10) +3;break;
                    case '4':val=(val*10) +4;break;
                    case '5':val=(val*10) +5;break;
                    case '6':val=(val*10) +6;break;
                    case '7':val=(val*10) +7;break;
                    case '8':val=(val*10) +8;break;
                    case '9':val=(val*10) +9;break;
                    case '0':val=(val*10) ;break;
                    
                    default:

                      if(samevalue)
                      {
                        samevalue=false;  
                      }
                      else
                      {

                        switch(type)
                        {
                          case 'f':ICache.addValue(type,val);break;
                          case 'r':ICache.addValue(type,val*12);break;
                          default:/*lol*/break;
                        }
                        vorzeichen=1;
                        val=0;
                        type=0;
                        otherin=false;
                        samevalue=true;
                      }
                          
                          
                          break;
                  }
                }else{
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
                  //case 'a':t=10;break;
                  //case 'b':t=11;break;
                  //case 'c':t=12;break;
                  //case 'd':t=13;break;
                  //case 'e':t=14;break;
                  //case 'f':t=15;break;
                  //case 'g':t=16;break;
                  //case 'h':t=17;break;
                  case 'w':km+=1;break;
                  case 'm':km-=1;break;
                  case 'f':otherin=true;type='f';break;
                  case 'r':otherin=true;type='r';break;  
                  default: break;
                }  
                }
}



