//
//  readlua.cpp
//  cpp_call_lua
//
//  Created by Liu,Han(ARL) on 2017/11/16.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "readlua.hpp"

CReadLua::CReadLua()
{
    _l = luaL_newstate();
}

CReadLua::~CReadLua()
{
    if (nullptr != _l)
    {
        lua_close(_l);
        _l = nullptr;
    }
}

void CReadLua::load_lua_file(const char* luaf)
{
    _str_lua_path = luaf;
}

int CReadLua::run()
{
        //1.创建Lua状态
        lua_State *L = _l;
        if (L == NULL) return 1;
        
        
        //2.加载Lua文件
        int bRet = luaL_loadfile(L, _str_lua_path.c_str());
        if (bRet){
                std::cout<<"load file error"<<std::endl;
                return 1;
            }
        
        //3.运行Lua文件
        bRet = lua_pcall(L,0,0,0);
        if (bRet){
                std::cout<<"pcall error"<<std::endl;
                return 1;
            }
        
        //4.读取变量
        lua_getglobal(L,"str");
        std::string str = lua_tostring(L,-1);
        std::cout<<"str = "<<str.c_str()<<std::endl;        //str = I am so cool~
        
        //5.读取table
        lua_getglobal(L,"tbl");
        lua_getfield(L,-1,"name");
        str = lua_tostring(L,-1);
        std::cout<<"tbl:name = "<<str.c_str()<<std::endl; //tbl:name = shun
        
        //6.读取函数
        lua_getglobal(L, "add");        // 获取函数，压入栈中
        lua_pushnumber(L, 10);          // 压入第一个参数
        lua_pushnumber(L, 20);          // 压入第二个参数
        int iRet= lua_pcall(L, 2, 1, 0);
        // 调用函数，调用完成以后，会将返回值压入栈中，2表示参数个数，1表示返回结果个数。
        if (iRet){
                const char *pErrorMsg = lua_tostring(L, -1);
                std::cout << pErrorMsg << std::endl;
                lua_close(L);
                return 1;
            }
        if (lua_isnumber(L, -1))        //取值输出
            {
                    double fValue = lua_tonumber(L, -1);
                    std::cout << "Result is " << fValue << std::endl;
                }
        
        //至此，栈中的情况是：
        //=================== 栈顶 ===================
        //  索引  类型      值
        //   4   int：      30
        //   3   string：   shun
        //   2   table:     tbl
        //   1   string:    I am so cool~
        //=================== 栈底 ===================
        
        print_stack();
        return 0;
}

int CReadLua::print_stack()
{
    std::cout<<"print_stack==================="<<std::endl;
    lua_State *L = _l;
    int nums = lua_gettop(L);
    for (int p = 1; p <= nums; p++){
        if(lua_isnumber(L, p)){
            int value = lua_tonumber(L, p);
            std::cout<<p<<"\t"<<"int\t"<<value<<std::endl;
        }
        else if (lua_isstring(L, p)){
            std::string str = lua_tostring(L,p);
            std::cout<<p<<"\t"<<"string\t"<<str<<std::endl;
        }
        else if (lua_istable(L, p)){
            std::cout<<p<<"\t"<<"table\t"<<std::endl;
        }
        else if(lua_iscfunction(L, p)){
            std::string str_fun_name = lua_tostring(L,p);
            std::cout<<p<<"\t"<<"cfunction\t"<<str_fun_name<<std::endl;
        }
    }
    
    lua_getglobal(L, "tbl");
    lua_pushstring(L, "name");
    lua_gettable(L, -2);
    std::cout<<"table:name_\t"<<lua_tostring(L, -1)<<std::endl;
    lua_pushstring(L, "id");
    lua_gettable(L, -3);
    std::cout<<"table:id_\t"<<(int)lua_tonumber(L, -1)<<std::endl;
    
    
    return 0;
}

