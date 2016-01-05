#ifndef DISPLAY_H
#define DISPLAY_H
#include <Wire.h>
#include <imu.h>
#include <Arduino.h>
enum DisplayDotMode{
  None,One,Two
};


class Display
{

public:

    unsigned char d1,d2,d3;

    DisplayDotMode dot=None;
    Display();

    unsigned char toBin(int a);

    void SetDotMode(DisplayDotMode a);

    void ShowCleared();
    
    void Show();

    int showSmallNumber(int n,DisplayDotMode bla=None);

};
#endif

