/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    Integrator.cpp
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Integrator.h"


Integrator::Integrator()
{
  reset();
}

void Integrator::reset(void)
{
  _pastT = millis();
  _pastX = 0.0;
  _dx0 = 0.0;
}

float Integrator::calcIntegral(float dx1, float xlimit)
{
    unsigned long time = millis();
    return(calcIntegral(dx1, xlimit, time));
}

float Integrator::calcIntegral(float dx1, float xlimit, unsigned long time)
{
  return(calcIntegral(dx1, xlimit, time,true));
}

float Integrator::calcIntegral(float dx1, float xlimit, unsigned long time, bool enabled)
{
  if( true == enabled)
  {
    float x = limit(_pastX + (float)(time - _pastT) * (_dx0 + dx1) / 2000.0, xlimit);

    _pastT = time;
    _dx0 = dx1;
    _pastX = x;
    return(x);
  } else
  {
    _pastT = time;
    _dx0 = dx1;
    return(limit(_pastX, xlimit));
  }    
}
