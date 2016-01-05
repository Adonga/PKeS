#include "Motor.h"

Motor::Motor()
{
    
    DDRH |= ( 1 << 3 );
    DDRH |= ( 1 << 4 );
    DDRB |= ( 1 << 5 );
    DDRB |= ( 1 << 6 );


    ICR1   = 255;
    TCCR1A = ( 1 << COM1A1 ) | ( 1 << COM1B1 ) | ( 1 << WGM11 );
    TCCR1B = ( 1 << WGM13  ) | ( 1 << WGM12 ) | ( 1 << CS10 );
    OCR1A  = 0;
    OCR1B  = 0;

    
    ICR4   = 255;
    TCCR4A = ( 1 << COM1A1 ) | ( 1 << COM1B1 ) | ( 1 << WGM11 );
    TCCR4B = ( 1 << WGM13  ) | ( 1 << WGM12 ) | ( 1 << CS10 );
    OCR4A  = 0;
    OCR4B  = 0;

  }

  void Motor::init()
  {
    DDRH |= ( 1 << 3 );
    DDRH |= ( 1 << 4 );
    DDRB |= ( 1 << 5 );
    DDRB |= ( 1 << 6 );


    ICR1   = 255;
    TCCR1A = ( 1 << COM1A1 ) | ( 1 << COM1B1 ) | ( 1 << WGM11 );
    TCCR1B = ( 1 << WGM13  ) | ( 1 << WGM12 ) | ( 1 << CS10 );
    OCR1A  = 0;
    OCR1B  = 0;

    
    ICR4   = 255;
    TCCR4A = ( 1 << COM1A1 ) | ( 1 << COM1B1 ) | ( 1 << WGM11 );
    TCCR4B = ( 1 << WGM13  ) | ( 1 << WGM12 ) | ( 1 << CS10 );
    OCR4A  = 0;
    OCR4B  = 0;

  }

 int Motor::Update()
  {
    int t1,t2,t3,t4;
    switch(cMode){

      case Wait:
            Stop();
            if(millis()-oldtime>=timewait){
              cMode=Drive;
              g=0;
            }

            break;
      
      case Drive:
          if(!g){
            cMode=Wait;
            oldtime=millis();
            timewait=50;
          }
          g++;
          t1=irc2.cValue;
          t2=irc1.cValue;

          if(t1>120){
             if(t2>100){
              dir=Forward;
              ChangeSpeed(Slow);
             }else{
               if((h++)%3)mygyro.gyroChanged=80*12;
               else mygyro.gyroChanged=-100*12;
              cMode=Rotate;
             }
             
          }else{
            if(t2>120){
              if((h++)%3)mygyro.gyroChanged=100*12;
              else mygyro.gyroChanged=-80*12;cMode=Rotate;
              cMode=Rotate;
            }else{
              dir=Backward;
              ChangeSpeed(VerySlow); 
            }
            
          }


          break;

      case Rotate:
          t1=mygyro.getUsefulNumber();

          if(!t1){
            dir=Stay;
            ChangeSpeed(Zero);
            g=0;
            return 1;
            break;
          }else if(t1<0){
            dir=Right;
            if(t1<-10){
              if(t1<-40){
                ChangeSpeed(Middle);
                break;
              }
              ChangeSpeed(Slow);
            }else{
              ChangeSpeed(VerySlow);
            }
          }else if(t1>0){
            dir=Left;
             if(t1>10){
              if(t1>40){
                ChangeSpeed(Middle);
                break;
              }
              ChangeSpeed(Slow);
             }else{
              ChangeSpeed(VerySlow);
             }
          }

          break;
    }

    return 0;
  }

  void Motor::bla()
  {
      OCR1A  = 0;
      OCR1B  = 0;
      OCR4A  = 128;
      OCR4B  = 255;
  }

  void Motor::Stop()
  {
    ChangeMove(Stay);    
  }


  void Motor::ChangeSpeed(SpeedMode mode)
  {
    currentSpeed=mode;
    ChangeMove(dir);
  }
  void Motor::ChangeMode(MoveMode mm)
  {
    
    cMode=mm;
    ChangeMove(Stay);  
  }

 void Motor::ChangeMove(Direction now)
  {
    PORTH|=(1<<3);
    _delay_us(200);
    PORTH&=~(1<<3);
    dir=now;
    
      switch(dir)
      {
      case Stay:
            OCR1A  = currentSpeed;
            OCR1B  = currentSpeed;
            OCR4A  = currentSpeed;
            OCR4B  = currentSpeed;        
            break;
      case Forward:
            OCR1A  = currentSpeed;
            OCR1B  = 0;
            OCR4A  = currentSpeed;
            OCR4B  = 0;
            break;
            
      case Right:
            OCR1A  = 0;
            OCR1B  = currentSpeed;
            OCR4A  = currentSpeed;
            OCR4B  = 0;
            break;
            
      case Backward:
            OCR1A  = 0;
            OCR1B  = currentSpeed;
            OCR4A  = 0;
            OCR4B  = currentSpeed;
            break;
      
      case Left:
            OCR1A  = currentSpeed;
            OCR1B  = 0;
            OCR4A  = 0;
            OCR4B  = currentSpeed;
            break;
      default:
            OCR1A  = 0;
            OCR1B  = 0;
            OCR4A  = 0;
            OCR4B  = 0;  
        break;
      }
  }

