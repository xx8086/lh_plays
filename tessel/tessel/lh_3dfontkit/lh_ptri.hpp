//
//  lh_ptri.hpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/11/6.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef lh_ptri_hpp
#define lh_ptri_hpp
#include "lh_type.h"

class CLhPTri
{
public:
    CLhPTri();
    ~CLhPTri();
public:
    float* create_thri(float, A_CHAEACTER&, unsigned int&);
    float* create_side(A_CHAEACTER_QUAD&, unsigned int&);
    
    void insertpoint(float* buff, float x, float y, float z);
private:
    void que2tri(float* buffer, LFPoint3& a, LFPoint3& b, LFPoint3& c, LFPoint3& d);
};

#endif /* lh_ptri_hpp */
