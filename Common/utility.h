/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    utility.h
*  Version:      1
*  Author:       Adam Licavoli
*  Modified by:  Adam Licavoli
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef utility_h
#define utility_h



#define TOUPPER(a) ((a >= 'a' && a <= 'z') ? (a & ~('a'-'A')) : a)
#define CTRL(x) (x-64)


float sign(float value);
float constrainTo180(float ang);
float limit(float val, float lim);
float limit(float val, float upperLim, float lowerLim);


int8_t parseKeyword(char *keyWord, const char *keys);
// char *nthSpaceDelimWord(char *keys, int n);
char *tokcasecmp(char *tok, char *target);


#endif
