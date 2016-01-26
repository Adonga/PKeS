#ifndef ADC_H
#define ADC_H
#include "Arduino.h"
class ADConverter
{

  public:

  ADConverter();
  
  int convert(char port=0);
  
};

#endif
