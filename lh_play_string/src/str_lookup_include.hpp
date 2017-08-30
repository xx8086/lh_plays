//
//  str_lookup_include.hpp
//  lh_play_string
//
//  Created by Liu,Han(ARL) on 2017/8/31.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//
/*
 查找字符串b中字母是否在字符串a中都包含
 a          b       return
 abcd       bad     ture
 bce     false
 aa      true
 */

#ifndef str_lookup_include_hpp
#define str_lookup_include_hpp

#include <stdio.h>


class CLookInclude
{
public:
    bool sortlookup(const char*, const char*);
    bool hashlookup(const char*, const char*);
};


#endif /* str_lookup_include_hpp */
