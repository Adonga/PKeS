#include "Display.h"


    Display::Display()
    {
      DDRC|=(28);
    }

    unsigned char Display::toBin(int a)
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

    void Display::	SetDotMode(DisplayDotMode a){dot=a;}

    void Display::ShowCleared()
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
    
    void Display::Show()
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


    int Display::showSmallNumber(int n,DisplayDotMode bla)
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

