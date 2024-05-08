#ifndef _EWMA
#define _EWMA
#include "Arduino.h"

class EWMA {
  
  private:

    float _alpha; //coefficient
    float _value; //latest value

  public:

    EWMA() {} //constructor
    EWMA(int value, float alpha); //constructor
    void setup(int value, float alpha); //setter function for initial value & coefficient
    int process(int value); //return the 'smoothed' value

};

#endif