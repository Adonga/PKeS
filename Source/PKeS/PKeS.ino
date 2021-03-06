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
#include "control.h"


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
      Serial.print("Added the following Command:");
      Serial.print(type);
      Serial.print(" ");
      Serial.println(val);  
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

  void ShowQueue()
  {
    if(!CSize){
      Serial.println("The Queue is Empty.");
    }
    for(int i=0;i<CSize;i++)
    {
      
      Serial.print("The following Command: ");
      Serial.print(types[(CSPointer+i)%CacheSize]);
      Serial.print(" ");
      Serial.print(values[(CSPointer+i)%CacheSize]);
      Serial.println(" is queued.");
        
    }
  }

  void Empty(bool show=true)
  {
    if(show)
    {
      char t=0;
      int v=0;
      for(;CSize>0;)
      {
          GetValues(t,v);
          Serial.print("The queued Command: ");
          Serial.print(t);
          Serial.print(" ");
          Serial.print(v);
          Serial.println(" has been deleted.");
            
      }
    }
    else
    {
      CSPointer=CEPointer;
      CSize=0;
    }
  }
};


Display dis;
MyGyro mygyro;
Motor motor;
Waterscale scale;
ADConverter adc;
InputCache ICache;


//int s1[10]={272,217,179,153,138,130,125,118,111,110};
int s1[10]={366,278,208,178,148,130,108,95,89,80};
IRC irc1(s1);
//int s2[10]={506,400,334,284,253,226,205,185,170,158};
int s2[10]={407,290,223,190,155,136,112,101,90,81};
IRC irc2(s2);
int speed150 = 156;

   Odometrie odo;
   Control control(speed150);
  ISR( INT2_vect )
  {
   odo.interrupt1++;  //rechts
   if(odo.interrupt1>=motor.driveDistance){
    motor.done=true;
    motor.Stop();
   }
  }
  
  ISR ( PCINT1_vect )
  {
    odo.interrupt2++; //links
    if(odo.interrupt2>=motor.driveDistance){
    motor.done=true;
    motor.Stop();
   }
  }


void ShowCommands()
{
  Serial.println("The Following Inputs are available:");
  Serial.println("h -> show Help");
  Serial.println("s -> the Bot Stops all current Actions");
  Serial.println("f (int) -> queues a forward command with specified the value");
  Serial.println("r (int) -> queues a rotation command with specified the value");
  Serial.println("c -> clears the Queue from all current Commands");
  Serial.println("z -> shows all queues Commands");
  Serial.println("d -> the Bot will start to progress the queued Commands");
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(57600);
  _delay_ms(100);
  
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
    control.getpid().setP( 100 );
    control.getpid().setI( 000 );
    control.getpid().setD( 040 );
    control.setMotorspeed( speed150, true );
    motor.limit = speed150;
    control.setTargetDirection( 0 );
/*
   Serial.println(TCCR1A);
   Serial.println(TCCR1B);
   Serial.println(TCCR4A);
   Serial.println(TCCR4B);
  */
  _delay_ms(10);
  _delay_ms(10);
  
  Serial.println("The Bot is now ready for Input");
  ShowCommands();
}




static byte km=0;
static int val=0;
static bool otherin=false;
char t=0;
bool forward = true;
bool turn = false;

static char currentType=0;
static int currentValue=0;

static bool nextCommand=false;

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
/*      
      Serial.print("Interrupt1: ");
      Serial.print(odo.interrupt1);
      Serial.print("  Interrupt2: ");
      Serial.println(odo.interrupt2);
*/
  
//  int dire = odo.distanceL() - odo.distanceR(); //pos is it has a right twist negative left twist
  int dire = odo.left() - odo.right() ; 
  Control::LR lr = control.currentSpeed( -dire );
  //Serial.println(dire); //-> 7

  switch(t){
    case 0:
          dis.ShowCleared();
          motor.Stop();
          odo.reset();
          break;
    case 1:
          irc1.showConverted(&dis,false);
          break;
    case 2:
          irc2.showConverted(&dis,false);
          break;
    case 3:
          h=mygyro.getUsefulNumber();
          dis.showSmallNumber(h);
          break;
    case 4:
    
          if(nextCommand)
          {
            ICache.GetValues(currentType,currentValue);
            odo.reset();
            motor.done=false;
            motor.invert=false;
            b=adc.convert();
            irc1.addvalue(b);
            k=adc.convert(1);
            irc2.addvalue(k);
            if(currentType=='f'&&currentValue==0)
            {
               t=0;
               break;  
            }
            switch(currentType)
            {
              case 'f':
              motor.ChangeMode(Drive);
              motor.driveDistance=currentValue*100/44;

              break;


              case 'r':
              motor.ChangeMode(Rotate);
              if(((currentValue*10)/36)<0){
                motor.invert=true;
                motor.driveDistance=-((currentValue*10)/36);
              }else{
                motor.driveDistance=((currentValue*10)/36);  
              }
              

              break;


              default: Serial.println("that shouldnt be here ...");break;
              
            }

            nextCommand=false;
          }
          h=motor.Update(&irc1,&irc2, &mygyro ,&lr, &odo);

          //h is 1 if the command is competed

          if(h==1)
          {
            Serial.print("The Following Command has been executed: ");
            Serial.print(currentType);
            Serial.print(" ");
            Serial.println(currentValue);
            nextCommand=true;
            b=adc.convert();
            irc1.addvalue(b);
            k=adc.convert(1);
            irc2.addvalue(k);
          }
          else if(h==-1)
          {
            //command cant be completed
            Serial.print("The Current Command: ");
            Serial.print(currentType);
            Serial.print(" ");
            Serial.print(currentValue);
            Serial.println(" cant be completed, the queue is beeing deleted.");
            ICache.Empty();
            nextCommand=true;
            b=adc.convert();
            irc1.addvalue(b);
            k=adc.convert(1);
            irc2.addvalue(k);
          }
          
          
          break;
    case 5:
          motor.cMode=Rotate;
          h=motor.Update(&irc1,&irc2,&mygyro ,&lr,&odo);
          dis.showSmallNumber(mygyro.getUsefulNumber());
          
          /*
          if(h)motor.ChangeMode(Drive);
          _delay_ms(20);
          */

          break;
    case 6:
          OCR1A  = km;
          OCR1B  = 0;
          OCR4A  = km;
          OCR4B  = 0;  
          Serial.println(km);
          break;
    case 7:
    /*
          motor.dir=Forward;
          motor.ChangeSpeed( &lr );
          _delay_ms(20);
      */
          ShowCommands();
          t=0;
          break;

    case 8:
          
          ICache.ShowQueue();
          t=0; 
          break;
    case 9:
          ICache.Empty(true);
          t=0;  
          break;
    default:
    t=0;
    Serial.println("default");break;
  
    
  }
 
 //if(a){_delay_ms(10000); a = false;}
  _delay_ms(100);
//*
                // read the incoming byte:
               
               while(true){
                
              
               byte incomingByte = Serial.read();
                //Serial.println(incomingByte);
                static char type=0;
                static int sign=1;
                //static bool samevalue=true;
                if(otherin){
                  switch(incomingByte){
                    case '-':sign*=-1;break;
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

                      //if(samevalue)
                      //{
                      //  samevalue=false;  
                      //}
                      //else
                      if(val>0)
                      {
                      
                        switch(type)
                        {
                          case 'f':if(sign==1){
                            ICache.AddValue(type,sign*val);
                          }else{
                            Serial.print("Command f cant have a value with sign. The Command is not added.");
                          }
                          break;
                          case 'r':ICache.AddValue(type,sign*val);break;
                          default:/*lol*/break;
                        }
                        sign=1;
                        val=0;
                        type=0;
                        otherin=false;

                        switch(incomingByte)
                        {
                          case 'f':
                            type='f';
                            otherin=true;
                          break;

                          case 'r':
                            type='r';
                            otherin=true;
                          break;
                        default:break;  
                        }
                        //samevalue=true;
                      }
                          
                          
                          break;
                  }
                }else{
                  switch(incomingByte)
                {
                  case 's':t=0;break;
                  case '1':t=1;break;
                  case '2':t=2;break;
                  //case '3':t=3;break;
                  case 'd':t=4;nextCommand=true;break;
                  //case '5':t=5;break;
                  //case '6':t=6;break;
                  //case '7':t=7;break;
                  //case '8':t=8;break;
                  //case '9':t=9;break;
                  //case 'a':t=10;break;
                  //case 'b':t=11;break;
                  //case 'c':t=12;break;
                  //case 'd':t=13;break;
                  //case 'e':t=14;break;
                  case 'h':t=7;break;
                  case 'c':t=9;break;
                  case 'z':t=8;break;
                  case 'w':km+=1;break;
                  //case 'm':km-=1;break;
                  case 'f':otherin=true;type='f';break;
                  case 'r':otherin=true;type='r';break;  
                  default: break;
                }  
                }
                if(otherin)continue;
                break;
                
                }
}



