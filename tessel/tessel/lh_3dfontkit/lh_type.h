//
//  lh_type.h
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/11/6.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef lh_type_h
#define lh_type_h

#include <vector>
typedef struct LFPoint
{
    double x = 0.0;
    double y = 0.0;
    LFPoint(double a, double b):x(a), y(b){}
    LFPoint(){}
} LF_POINT;

typedef struct LFPoint3
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    LFPoint3(double a, double b, double c):
    x(a), y(b), z(c){}
    LFPoint3(){}
} LF_POINT3;

typedef std::vector< std::vector<LFPoint> >  A_CHAEACTER;
typedef std::vector< std::vector<LFPoint3> >  A_CHAEACTER_QUAD;

/*
#define insertpoint(pbuff, x, y , z) {\
*(pbuff) = x;\
*((pbuff) + 1) = y;\
*((pbuff) + 2) = z;\}
*/

#endif /* lh_type_h */
