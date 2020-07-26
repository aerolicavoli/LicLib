/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    DigitalFilter.cpp
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "DigitalFilter.h"

DigitalFilter::DigitalFilter(int order)
{
  _filterOrder = order;

  if(_filterOrder>MAX_FILTER_ORDER)
  {
    _filterOrder = MAX_FILTER_ORDER;
  }

  for(int i=0; i < MAX_FILTER_ORDER; ++i)
  {
    _taps[i] = 0.0;
    _history[i] = 0.0;
  }
  _lastIndex = 0;
}

void DigitalFilter::Set(float input)
{
  _history[_lastIndex++] = input;
  if(_lastIndex == _filterOrder)
  {
    _lastIndex = 0;
  }
}

float DigitalFilter::Get()
{
  float output = 0.0;
  int index = _lastIndex;
  for(int i=0; i < _filterOrder; ++i)
  {
    index = index !=0 ? index-1 : _filterOrder-1;
    output += _history[index]*_taps[i];
  }
  return(output);
}

float DigitalFilter::calcFilter(float input)
{
  Set(input);
  return(Get());
}