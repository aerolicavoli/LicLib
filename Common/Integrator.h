/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    Integrator.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Integrator_h
#define Integrator_h

#include "Utility.h"
class Integrator
{

public:
    Integrator();

   float calcIntegral(float dx1, float xlimit);
   float calcIntegral(float dx1, float xlimit, unsigned long time);
   float calcIntegral(float dx1, float xlimit, unsigned long time,bool enabled);
   
   void reset(void);

    float  _dx0, _pastX;
    unsigned long _pastT;
};

#endif
