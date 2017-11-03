//
//  ftpoint.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/31.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "ftpoint.hpp"
#include <math.h>

bool operator==(const FTPoint &a, const FTPoint &b)
{
    return ((a._values[0] == b._values[0]) &&
            (a._values[1] == b._values[1]) &&
            (a._values[2] == b._values[2]));
}

bool operator!=(const FTPoint &a, const FTPoint &b)
{
    return ((a._values[0] != b._values[0]) ||
            (a._values[1] != b._values[1]) ||
            (a._values[2] != b._values[2]));
}

FTPoint FTPoint::normalise()
{
    double norm = sqrt(_values[0] * _values[0] + _values[1] * _values[1] + _values[2] * _values[2]);
    const double EPSILON = 1e-9;
    if (fabs(norm) < EPSILON)
    {
        return *this;
    }

    FTPoint temp(_values[0] / norm, _values[1] / norm, _values[2] / norm);
    return temp;
}
