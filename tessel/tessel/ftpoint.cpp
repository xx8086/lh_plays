//
//  ftpoint.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/31.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "ftpoint.hpp"
#include <math.h>


bool operator == (const FTPoint &a, const FTPoint &b)
{
    return((a.values[0] == b.values[0]) && (a.values[1] == b.values[1]) && (a.values[2] == b.values[2]));
}


bool operator != (const FTPoint &a, const FTPoint &b)
{
    return((a.values[0] != b.values[0]) || (a.values[1] != b.values[1]) || (a.values[2] != b.values[2]));
}

FTPoint FTPoint::Normalise()
{
    double norm = sqrt(values[0] * values[0]
                       + values[1] * values[1]
                       + values[2] * values[2]);
    if(norm == 0.0)
    {
        return *this;
    }
    
    FTPoint temp(values[0] / norm, values[1] / norm, values[2] / norm);
    return temp;
}
