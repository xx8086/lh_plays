//
//  lh2tri.hpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/25.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef lh2tri_hpp
#define lh2tri_hpp


#include "glbase.hpp"
#include "poly2tri/poly2tri.h"
#include <vector>


class CLH2tri :public CGL
{
public:
    CLH2tri();
    ~CLH2tri();
public:
    int run(int args, char *argv[]);
    
};

#endif /* lh2tri_hpp */
