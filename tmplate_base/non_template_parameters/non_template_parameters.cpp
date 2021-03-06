// non_template_parameters.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "template_parameter.hpp"
#include <string>
#include <iostream>
#include <algorithm> // std::transform  
#include <cctype> // std::toupper  

int main()
{
/* 1.
    std::string s("Hello");
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::toupper(c); });

    int src[6] = { 1, 2, 3, 4, 5, 6 };
    int des[6] = { 0 };
    std::transform(src, &src[5], des, 
        (int(*)(int const&))addvalue<int, 5>);
*/
/* 1.
函数模板实列通常被看成是用来命名一组重载函数的集合，然而重载函数的集合不能用于模板参数(transform的最后一个参数)的演绎。
于是，必须将这个函数模板addvalue的实参强制类型转换为具体的类型 T addvalue (T const& t) -> int (*) (int const&)) 。
    
目前c++11已经支持此情景不需要实参强制类型转换。但是在可抑制性的情况下才需要使用这种强制转型。
*/

/* 2
    MyChar<"hello"> x;//error，字符串文字是内部链接对象(两个相同名称的字符串在不同模块，是两个完全不同的对象)

    char const* s = "hello";
    MyChar<s> y;//error，s是一个指向内部链接对象的指针

    extern char const r[6];
    MyChar<r> z;//OK，全局数组r是一个外部链接对象。
*/

/* .3
    Derived<int> d;
    //d.foo();
*/
    return 0;
}

