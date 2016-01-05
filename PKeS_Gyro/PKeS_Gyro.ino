#include <Wire.h>
#include <imu.h>

enum DisplayDotMode{
  None,One,Two
};


class Display
{

public:

    unsigned char d1,d2,d3;

    DisplayDotMode dot=None;
    Display()
    {
      DDRC|=(28);
    }

    unsigned char toBin(int a)
    {
      switch(a)
      {
        case -1:return 0b01000110;
        case  0:return 0b00111111;
        case  1:return 0b00000110;
        case  2:return 0b01011011;
        case  3:return 0b01001111;
        case  4:return 0b01100110;
        case  5:return 0b01101101;
        case  6:return 0b01111101;
        case  7:return 0b00000111;
        case  8:return 0b01111111;
        case  9:return 0b01101111;
        default:return 0b00000000;     
      }
    }

    void SetDotMode(DisplayDotMode a){dot=a;}

    void ShowCleared()
    {
      unsigned char b1,b2,b3;
      b1=d1;
      b2=d2;
      b3=d3;
      DisplayDotMode hg=dot;
      d1=0;
      d2=0;
      d3=0;
      hg=None;

      Show();
      dot=hg;
      d1=b1;
      d2=b2;
      d3=b3;      
    }
    
    void Show()
    {
      switch(dot){
        case None:break;
        case One: d2|=0b10000000;break;
        case Two: d1|=0b10000000;break;
      }

      
      PORTC&=~(1<<2);
      PORTC&=~(1<<4);
      _NOP();
      _NOP();
      PORTC|=(1<<3);
      _NOP();
      _NOP();
      _NOP();
      _NOP();
      _NOP();
      PORTC|=(1<<2);
      _NOP();
      _NOP();
      //PORTC|=(1<<4);

      for(unsigned char i=0;i<8;i++)
      {
        PORTC&=~(1<<3);
        PORTC&=~(1<<2);
        //PORTC&=~(1<<4);
        _NOP();
        _NOP();
        PORTC|=(((d1>>i)&1)<<3);
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        PORTC|=(1<<2);
        _NOP();
        _NOP();
        //PORTC|=(1<<4);
      }


      for(unsigned char i=0;i<8;i++)
      {
        PORTC&=~(1<<3);
        PORTC&=~(1<<2);
        //PORTC&=~(1<<4);
        _NOP();
        _NOP();
        PORTC|=(((d2>>i)&1)<<3);
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        PORTC|=(1<<2);
        _NOP();
        _NOP();
        //PORTC|=(1<<4);
      }

      for(unsigned char i=0;i<8;i++)
      {
        PORTC&=~(1<<3);
        PORTC&=~(1<<2);
        //PORTC&=~(1<<4);
        _NOP();
        _NOP();
        PORTC|=(((d3>>i)&1)<<3);
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        PORTC|=(1<<2);
        _NOP();
        _NOP();
        //PORTC|=(1<<4);
      }

      for(unsigned char i=0;i<11;i++)
      {
        PORTC&=~(1<<2);
      //PORTC&=~(1<<4);
      _NOP();
      _NOP();
      PORTC|=(1<<3);
      _NOP();
      _NOP();
      _NOP();
      _NOP();
      _NOP();
      PORTC|=(1<<2);
      _NOP();
      _NOP();
      //PORTC|=(1<<4);
      }
      PORTC|=(1<<4);
      
    }


    int showSmallNumber(int n,DisplayDotMode bla=None)
    {
      dot=bla;
        int error=0;

        int b=min(999,max(-199,n));
        if(b!=n)error=1;


        if(b<0){
          if(b<-100){
            d1=toBin(-1);
          }else{
            d1=0b01000000;
          }
          b=-b;
        }else{
          d1=toBin(b/100);
        }

        d2=toBin((b/10)%10);
        d3=toBin((b)%10);
        Show();

        return error;  
    }

};

Display dis;



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


MyGyro mygyro;
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
