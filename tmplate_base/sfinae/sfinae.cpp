// sfinae.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "is_point.hpp"
#include "has_foo.hpp"

int main(){
	testTypeCheck();
	hasFoo();
    return 0;
}

