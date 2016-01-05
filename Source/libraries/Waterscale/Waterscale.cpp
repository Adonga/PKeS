#include "Waterscale.h"
 
  Waterscale::Waterscale( ){}

  void Waterscale::Show()
  {
    displayTilt( accelY, accelX );
  }
  void Waterscale::Update()
  {

  accel[count][0] = MPU9150_readSensor(MPU9150_ACCEL_XOUT_L,MPU9150_ACCEL_XOUT_H);
  accel[count][1] = MPU9150_readSensor(MPU9150_ACCEL_YOUT_L,MPU9150_ACCEL_YOUT_H);
  count %= 5;
  for(int i = 0; i< 5; i++)
  {
    accelX += accel[i][0];
    accelY += accel[i][1];
  }
  accelX = accelX/5;
  accelY = accelY/5;
  accelX = accelX/10; // on lisa we have to divide by 100
  accelY = accelY/10; // on meggie by 10 who knows why

  //Serial.print(accelX);
  //Serial.print(" X  ");
  //Serial.print(accelY);
  //Serial.print(" Y  ");
 
  
  }
  
  void Waterscale::displayTilt( int tiltX, int  tiltY)
  {
    unsigned char bitNumber [3];

      bitNumber [0] = 0;
      bitNumber [1] = 0;
      bitNumber [2] = 0;
     if( tiltY < 59 && tiltY > -59 )
     {
      bitNumber [0] |= convertToBit(0);
      bitNumber [1] |= convertToBit(0);
      bitNumber [2] |= convertToBit(0);      //neutral
     }
     if(tiltX < 25 && tiltX >-25  )
     {
      bitNumber [1] |= convertToBit(3);
      bitNumber [1] |= convertToBit(4); //neutral
     }
    //_______________________________________________________________-
    if(tiltX > 170)
    {
      bitNumber [0] = convertToBit(-1);
      bitNumber [1] = convertToBit(-1);
      bitNumber [2] = convertToBit(-1);
    }
    else if( tiltX > 150 )
    {
      bitNumber [0] |= convertToBit(4); // left
    }
    else if( tiltX > 120 )
    {
      bitNumber [0] |= convertToBit(3);
      bitNumber [0] |= convertToBit(4);
    }
    else if( tiltX > 90 )
    {
      bitNumber [0] |= convertToBit(3);
    }
    else if( tiltX > 60 )
    {
      bitNumber [0] |= convertToBit(3);
      bitNumber [1] |= convertToBit(4);
    }
    else if( tiltX > 30 )
    {
      bitNumber [1] |= convertToBit(4);
    }
    //_______________________________________________________________-
    else if(tiltX > -30 && tiltX != 0)
    {
      bitNumber [1] |= convertToBit(3);
    }
    else if( tiltX > -60 && tiltX != 0 )
    {
      bitNumber [2] |= convertToBit(4); // left
      bitNumber [1] |= convertToBit(3);
    }
    else if( tiltX > -90  && tiltX != 0)
    {
      bitNumber [2] |= convertToBit(4);
    }
    else if( tiltX > -120  && tiltX != 0)
    {
      bitNumber [2] |= convertToBit(4);
      bitNumber [2] |= convertToBit(3);
    }
    else if( tiltX > -150 && tiltX != 0)
    {
      bitNumber [2] |= convertToBit(3);
    }
    else if( tiltX <= -170 && tiltX != 0)
    {
      bitNumber [0] = convertToBit(-1);
      bitNumber [1] = convertToBit(-1);
      bitNumber [2] = convertToBit(-1);
    }
    //_______________________________________________________________-
    if( tiltY > 170 )
    {
      bitNumber [0] |= convertToBit( -1);
      bitNumber [1] |= convertToBit( -1);
      bitNumber [2] |= convertToBit( -1);      //tilt up
    }
    else if( tiltY > 120 )
    {
      bitNumber [0] |= convertToBit( 1);
      bitNumber [1] |= convertToBit( 1);
      bitNumber [2] |= convertToBit( 1);      //tilt up
    }
    else if( tiltY > 60 )
    {
      bitNumber [0] |= convertToBit( 1);
      bitNumber [1] |= convertToBit( 1);
      bitNumber [2] |= convertToBit( 1);      //tilt up
      bitNumber [0] |= convertToBit( 0);
      bitNumber [1] |= convertToBit( 0);
      bitNumber [2] |= convertToBit( 0);      //tilt up
    }
    //_______________________________________________________________-
   
    if( tiltY < -30 && tiltY > -60)
    {
      bitNumber [0] |= convertToBit( 2);
      bitNumber [1] |= convertToBit( 2);
      bitNumber [2] |= convertToBit( 2);      //tilt up
      bitNumber [0] |= convertToBit( 0);
      bitNumber [1] |= convertToBit( 0);
      bitNumber [2] |= convertToBit( 0);      //tilt up
    }
    else if( tiltY < -20 && tiltY > -170 )
    {
      bitNumber [0] |= convertToBit(2);
      bitNumber [1] |= convertToBit(2 );
      bitNumber [2] |= convertToBit(2);      //tilt down
    }
    else if( tiltY < -20 && tiltY <= -170)
    {
      bitNumber [0] |= convertToBit( -1);
      bitNumber [1] |= convertToBit( -1);
      bitNumber [2] |= convertToBit( -1);
    }

    putNumbertoDisplay( bitNumber );

  }


  unsigned char Waterscale::convertToBit( int in )
  {
    unsigned char charNumber;
    switch(in)
    {
                         //  GFEDCBA
      case -1: charNumber = 0b11111111; break;//       Error
      case 0: charNumber = 0b01000000; break;//       Neutral
      case 1: charNumber = 0b00000001; break;//       Tilt up
      case 2: charNumber = 0b00001000; break;//       Tilt down
      case 3: charNumber = 0b00000110; break;//       Tilt right
      case 4: charNumber = 0b00110000; break;//       Tilt left
      default: charNumber = 0b00000000; break;//
    }
    return charNumber;
  }
 
  void Waterscale::putNumbertoDisplay(unsigned char charNumber [3])
  {

 
      PORTC &= ~( 1 << PC2);
      PORTC &= ~(1  << PC4 );
      _delay_us(1); // wating for dataenable
      PORTC |= (1  << PC3 );
      _delay_us(1);
      PORTC |= ( 1 << PC2 );//set clock to 0
      _delay_us(1);
      PORTC |= (1 << PC4); //set data enable to 0

     
    //writeOnDisplay
    for (int i = 0; i< 3 ;i++)
    {
      for( int j = 0; j<8 ; j++)
      {
        PORTC &= ~( 1 << PC2);
        PORTC &= ~(1  << PC4 );
        _delay_us(1); // wating for dataenable
        if( ( charNumber [i] >> j) & 0x01 ) 
        {
          PORTC |= ( 1 << PC3);
        }
        else
        {
          PORTC &= ~( 1 << PC3 );
        }
        _delay_us(1);
        PORTC |= ( 1 << PC2 );//set clock to 0
        _delay_us(1);
        PORTC |= (1 << PC4); //set data enable to 0
      }   
    }
    for( int i = 24;i <36;i++ )
    {
      PORTC &= ~( 1 << PC2);
      PORTC &= ~(1  << PC4 );
      _delay_us(1); // wating for dataenable
      PORTC &= ~(1  << PC3 );
      _delay_us(1);
      PORTC |= ( 1 << PC2 );//set clock to 0
      _delay_us(1);
      PORTC |= (1 << PC4); //set data enable to 0
    }
  }
    

