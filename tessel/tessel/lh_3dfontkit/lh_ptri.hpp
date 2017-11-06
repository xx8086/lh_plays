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
#include <map>

class CLhPTri
{
public:
    CLhPTri();
    ~CLhPTri();
public:
    float* create_thri(float, A_CHAEACTER&, unsigned int&);
    float* create_side(A_CHAEACTER_QUAD&, unsigned int&);

private:
    typedef std::map<int, std::vector<int> > MAP_POLY_WITH_HOLES;
    typedef struct LhAabb{
        float max_x = 0.0f;
        float max_y = 0.0f;
        float min_x = 0.0f;
        float min_y = 0.0f;
    }LH_AABB;
    LH_AABB make_aabb(std::vector<LFPoint>&);
    inline bool boundingbox(LH_AABB&, LH_AABB&);
    inline bool point_in_poly(LFPoint&, std::vector<LFPoint>&);
    void make_hole(A_CHAEACTER&, MAP_POLY_WITH_HOLES&);
    void insertpoint(float* buff, float x, float y, float z);
    void que2tri(float* buffer, LFPoint3& a, LFPoint3& b, LFPoint3& c, LFPoint3& d);
};

#endif /* lh_ptri_hpp */
