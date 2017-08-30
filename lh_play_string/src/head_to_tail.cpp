//
//  head_to_tail.cpp
//  lh_play_string
//
//  Created by Liu,Han(ARL) on 2017/8/30.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "head_to_tail.hpp"

bool CHeadToRail::parameter_check(char* src, int len, int num)
{
    if (nullptr != src &&
        num > 0 &&
        len >= num)
    {
        return true;
    }
    
    return false;
}

void CHeadToRail::do_move_one(char* src, int len)
{
    char temp = src[0];
    for(int i = 0; i < len - 1; i++)
    {
        src[i] = src[i+1];
    }
    src[len-1] = temp;
}

bool CHeadToRail::movetail(char* src, int len, int num)
{
    if (!parameter_check(src, len, num))
    {
        return false;
    }
    
    while(num--)
    {
        do_move_one(src, len);
    }
    return true;
    //O(m*n)
}

bool CHeadToRail::reversal(char* src, int len, int num)
{
    if (!parameter_check(src, len, num))
    {
        return false;
    }
    do_reversal(src, num);
    do_reversal((src+num), len - num);
    do_reversal(src, len);
    return true;
}

void CHeadToRail::do_reversal(char* src, int len)
{
    char temp;
    for(int i = 0; i < len/2; i++)
    {
        temp = src[i];
        src[i] = src[len-1-i];
        src[len-1-i] = temp;
    }
    //O(m) + O(n);
}











