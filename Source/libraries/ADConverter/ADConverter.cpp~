#include "ADConverter.h"

  ADConverter::ADConverter()
  {
    PRR0&=~(1);  
    
    
    ADCSRA|=_BV(ADEN)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0);
    ADMUX&=~_BV(REFS1);
    ADMUX|=_BV(REFS0);

    ADMUX&=~(1);
    ADMUX|=(2);
    ADMUX&=~(4);
    ADMUX&=~(8);
    ADMUX&=~(16);
    ADMUX&=~(32);
    
    
    
    ADCSRB&=~(1);
    ADCSRB&=~(2);
    ADCSRB&=~(4);
    //ADCSRA|=_BV(ADATE);
    //DDRF&=~(1);
    //DDRF&=~(2);
    

    ADCSRA|=_BV(ADSC);
    while(!bit_is_set(ADCSRA,ADIF));
    ADCSRA |= _BV(ADIF);
    int b=ADCL|(ADCH<<8);
}
  
  int ADConverter::convert(char port)
  {
    ADMUX&=~(63);
    port&=(63);
    ADMUX|=port;
 
    
    ADCSRA|=_BV(ADSC);
    while(!bit_is_set(ADCSRA,ADIF));
    ADCSRA |= _BV(ADIF);
    
    int b=ADCL|((ADCH)<<8);
    return b;
  }
  
};

