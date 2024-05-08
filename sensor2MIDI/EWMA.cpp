#include "EWMA.h"

EWMA::EWMA(int value, float alpha) {

  setup(value, alpha); //setup from constructor

}

void EWMA::setup(int value, float alpha) {

  _value = value; //set initial value
  _alpha = alpha; //set coefficient

}

int EWMA::process(int value) {

  _value = (_alpha * (float)value) + (1 - _alpha) * _value; //implement exponential weighted moving average
  return floor(_value); //return value rounded to nearest int

}