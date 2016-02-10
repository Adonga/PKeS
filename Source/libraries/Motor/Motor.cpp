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

 int Motor::Update( IRC *irc1, IRC *irc2, MyGyro *mygyro ,Control::LR *lr,Odometrie *odo)
  {
	ChangeSpeed(lr);
	
    int t1,t2,t3,t4,t5;
    
	switch(cMode){

      case Wait:
            Stop();
            
            break;
      
      case Drive:
      
          t1=irc1->cValue;
          t2=irc2->cValue;
          t3=irc1->lValue;
          t4=irc2->lValue;
		  dir=Forward;

		  //check distance ...
		  
		  t5=(odo->left());
		  
		  t5*=44;
		  //t5-=driveDistance;
		  
		  //Serial.println(t5);
		  
		  if(t5>(driveDistance-22))
		  {
			//Stop();
			return 1;
		  }
		  
		  
		  
          break;

      case DriveBy:
	  
	  /*
            t1=irc1->cValue;
            t2=irc2->cValue;
            ChangeSpeed(VerySlow);  


            
            if(t1>150&&t2>150){
             g=0;
             dir=Stay;
             cMode=Drive;
            }


*/

/*
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

*/

		Serial.println("lol");
		cMode=Wait;
            break;
      
      case Rotate:
			  int rotationTicks= (driveDistance* 10)/52;
      					
				if( rotationTicks > 0)
				{
					dir = Right;
					if (odo->right() >= rotationTicks && mygyro->getUsefulNumber() >= driveDistance )
						return 1;
				}
				else if ( rotationTicks < 0 )
				{
					if (odo->left() >= -rotationTicks && mygyro->getUsefulNumber() <= -driveDistance)
						return 1;
					dir = Left;
				}else 
				{
					return 1;
				}
/**	  
          t1=mygyro->getUsefulNumber();
		  t2=mygyro->gyroChanged;
          if(!t1||((t2>-5) && (t2<5))){
            dir=Stay;
            ChangeSpeed(Zero);
            //g=0;
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
*/
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


  void Motor::ChangeSpeed( Control::LR *leftright )
  {
//		m_pid.setMotorspeed(mode);
		//currentSpeedLR[0] = limit <= leftright->leftM ? limit : leftright->leftM;
		//currentSpeedLR[1] = limit <= leftright->rightM ? limit : leftright->rightM;

		currentSpeedLR[0] = leftright->leftM;
		currentSpeedLR[1] = leftright->rightM;


    ChangeMove(dir);
  }

  void Motor::ChangeSpeed( SpeedMode mode )
  {
//		m_pid.setMotorspeed(mode);
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

    int cSpeedl = currentSpeedLR[0];//currentSpeed + offSetLeft;
		int cSpeedr = currentSpeedLR[1];//currentSpeed + offSetRight;
//		cSpeedr = cSpeedr<0? limit:cSpeedr;
/*
		Serial.print(cSpeedl);
		Serial.print(" L|R ");
		Serial.println(cSpeedr);
*/

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

