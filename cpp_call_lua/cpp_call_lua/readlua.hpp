//
//  readlua.hpp
//  cpp_call_lua
//
//  Created by Liu,Han(ARL) on 2017/11/16.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef readlua_hpp
#define readlua_hpp
#include <iostream>
#include <string.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class CReadLua
{
public:
    CReadLua();
    ~CReadLua();
public:
    int run();
    int print_stack();
    void load_lua_file(const char* luaf);
private:
    std::string _str_lua_path;
    lua_State* _l = nullptr;
};


#endif /* readlua_hpp */
