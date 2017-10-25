//
//  main.cpp
//  tessel
//
//  Created by Liu,Han(ARL) on 2017/10/24.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "lake.hpp"
#include "quad.hpp"
int main(int args, char *argv[])
{
    CGL* cg = nullptr;
    switch (0) {
        case 0:
            cg = new CLake;
            break;
        case 1:
            cg = new CQuad;
            break;
        default:
            break;
    }
    
    cg->run(args, argv);
    
    if (nullptr != cg){
        delete cg;
        cg = nullptr;
    }
    
    return 0;
}
