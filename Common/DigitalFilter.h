/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    DigitalFilter.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef DigitalFilter_h
#define DigitalFilter_h

#define MAX_FILTER_ORDER 30

class DigitalFilter
{

public:
	DigitalFilter(int order);

	void Set(float input);
	float Get(void);
	float calcFilter(float input);

	int _filterOrder;
	float _taps[MAX_FILTER_ORDER];

private:
	float _history[MAX_FILTER_ORDER];
	unsigned int _lastIndex;

};


#endif