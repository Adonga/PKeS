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

		m_pid.setPID(5,0,0);
		m_pid.setOutputLimits(2,5);
		m_pid.tarSpeed = 150;
  }

 int Motor::Update(IRC *irc1, IRC *irc2, MyGyro *mygyro)
  {
		int tmp = interrupt1 - interrupt2; //int right,int left
		if( (tmp-1)>0 || (tmp+1)<0 )
		{
		if( tmp > 0 ) 
			{offSetLeft+=2; }
		else 
	 		{offSetLeft-=2; }
		}
    int t1,t2,t3,t4;
    switch(cMode){

      case Wait:
            Stop();
            if(millis()-oldtime>=timewait)
						{
              cMode=Drive;
              g=1;
            }
            break;
      
      case Drive:
      /*
          if(!g){
            cMode=Drive;
            oldtime=millis();
            timewait=50;
            break;
          }
          g=1;
          */
          t1=irc1->cValue;
          t2=irc2->cValue;
          t3=irc1->lValue;
          t4=irc2->lValue;
          if(t1>230){

              h=(h+1)%2;
             
              dir=Forward;
              ChangeSpeed(VerySlow);
             
          }else{

            switch(h){
              case 0:
                  //dir=Right;
                  mygyro->gyroChanged=90*12;
                  break;

              case 1:
                  //dir=Left;
                  mygyro->gyroChanged=-90*12;
                  break;
                  
            }
            cMode=Rotate;
            break;
          }

          if(t2>230){

              h=(h+1)%2;
             
              dir=Forward;
              ChangeSpeed(VerySlow);
             
          }else{

            switch(h){
              case 0:
                  //dir=Right;
                  mygyro->gyroChanged=90*12;
                  break;

              case 1:
                  //dir=Left;
                  mygyro->gyroChanged=-90*12;
                  break;
                  
            }
            cMode=Rotate;
            
          }


          break;

      case DriveBy:
            t1=irc1->cValue;
            t2=irc2->cValue;
            ChangeSpeed(VerySlow);  


            
            if(t1>150&&t2>150){
             g=0;
             dir=Stay;
             cMode=Drive;
            }



            break;
      
      case Rotate:
          t1=mygyro->getUsefulNumber();

          if(!t1){
            dir=Stay;
            ChangeSpeed(Zero);
            g=0;
            return 1;
            break;
          }else if(t1<0){
            dir=Right;
            t1=-t1;
            if(t1<40){
              if(t1<10){
                ChangeSpeed(VeryVerySlow);
                break;
              }
              ChangeSpeed(VerySlow);
            }else{
              ChangeSpeed(VerySlow);
            }
          }else if(t1>0){
            dir=Left;
             if(t1<40){
              if(t1<10){
                ChangeSpeed(VeryVerySlow);
                break;
              }
              ChangeSpeed(VerySlow);
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
		int* currentSpeedLR  = m_pid.currentSpeed();
    int cSpeedl = currentSpeedLR[0];//currentSpeed + offSetLeft;
		int cSpeedr = currentSpeedLR[1];//currentSpeed + offSetRight;
      switch(dir)
      {
      case Stay:
            OCR1A  = cSpeedl;
            OCR1B  = cSpeedl;
            OCR4A  = cSpeedr;
            OCR4B  = cSpeedr;        
            break;
      case Forward:
            OCR1A  = cSpeedl;
            OCR1B  = 0;
            OCR4A  = cSpeedr;
            OCR4B  = 0;
            break;
            
      case Right:
            OCR1A  = 0;
            OCR1B  = cSpeedl;
            OCR4A  = cSpeedr;
            OCR4B  = 0;
            break;
            
      case Backward:
            OCR1A  = 0;
            OCR1B  = cSpeedl;
            OCR4A  = 0;
            OCR4B  = cSpeedr;
            break;
      
      case Left:
            OCR1A  = cSpeedl;
            OCR1B  = 0;
            OCR4A  = 0;
            OCR4B  = cSpeedr;
            break;
      default:
            OCR1A  = 0;
            OCR1B  = 0;
            OCR4A  = 0;
            OCR4B  = 0;  
        break;
      }
  }

