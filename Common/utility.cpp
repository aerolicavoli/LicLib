/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
*
*  File Name:    utility.cpp
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
#include "utility.h"

float sign(float value)
{
    return float((value > 0) - (value < 0));
}

float constrainTo180(float ang)
{
    ang = fmod(ang + 180, 360);
    if(ang < 0)
    {
        ang += 360;
    }
    return(ang - 180);
}

/*
** Symetric Limit
*/
float limit(float val, float lim)
{
	return(limit(val,-lim,lim));
}

/*
** Nonsymetric Limit
*/
float limit(float val, float lowerLim, float upperLim)
{
    if(val >= upperLim)
    {
        return(upperLim);
    }
    else if (val <= lowerLim)
    {
        return(lowerLim);
    } else
    {
    	return(val);
    }

    
}

/*
 * Match the next token with a list of keywords.
 * The list of keywords is in PROGMEM, separated by spaces.
 * returns either the position of the found keyword (0..n),
 * PARSER_NOMATCH, or PARSER_EOL at the end of line
 */
int8_t parseKeyword(char *keyWord, const char *keys)
{

    char *key = (char *)keys;
    int8_t i = 0;
    if (keyWord)
    {
        while (pgm_read_byte(key))
        {
            key = tokcasecmp(keyWord, key);
            if (key == 0)
            {
                return i;  // match
            }
            key++;  // skip delimiter
            i++; // next keyword
        }
    }
    else
    {
        return -2;
    }
    return -1;
}


// /*
// ** Returns pointer to first char in nth word (0 idxed)
// ** Of a space delimited string
// */
// char *nthSpaceDelimWord(char *keys, int n)
// {
// 	char *t = (char *)keys;
// 	int wordIdx = 0;
// 	while(wordIdx < n)
// 	{
// 		while (*t > ',')
// 		{
// 			t++;
//     	}
//     	t++
//     	wordIdx++
//     }
//     return(t);

// }

char *tokcasecmp(char *tok, char *target)
{
    char c1, c2;
    char *t = (char *)target;

    do
    {
        c1 = TOUPPER(*tok);
        c2 = TOUPPER(*t);
        if (c2 <= ',')
        {
            /* Match */
            if (c1 != 0)
            {
                return t;  // oops; both strings didn't end.
            }
            return 0; //match!
        }
        tok++;
        t++;
    }
    while (c1 == c2);
    /* Non-match.  Advance target ptr */
    while (*t > ',')
    {
        t++;
    }
    return t;
}