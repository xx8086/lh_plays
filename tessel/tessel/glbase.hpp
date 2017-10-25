//
//  glbase.hpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/25.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef glbase_hpp
#define glbase_hpp

#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>

class CGL
{
public:
    CGL();
    virtual ~CGL();
public:
    virtual int run(int args, char *argv[]) = 0;
};

#endif /* glbase_hpp */
