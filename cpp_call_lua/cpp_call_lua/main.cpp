

#include "readlua.hpp"

int main()
{
    CReadLua readload;
    readload.load_lua_file("/Users/baidu/lh_plays/cpp_call_lua/cpp_call_lua/hello.lua");
    readload.run();
    return 0;
}
