//
//  main.cpp
//  lh_play_string
//
//  Created by Liu,Han(ARL) on 2017/8/30.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include <iostream>
#include "head_to_tail.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    

    CHeadToRail torail;
    char strmove[128] = {"abcdefghijklmn"};
    char strreversal[128] = {"abcdefghijklmn"};
    torail.movetail(strmove, strlen(strmove), 5);
    torail.reversal(strreversal, strlen(strreversal), 5);
    
    std::cout<<strmove<<std::endl;
    std::cout<<strreversal<<std::endl;

    
    return 0;
}
