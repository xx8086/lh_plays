// member_template.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "member_template.hpp"
#include <vector>

#include <typeinfo>
#include <iostream>

template <typename T>
void ref(T const & x) {
    std::cout << "x in ref(T const&):"
        << typeid(x).name() << std::endl;
}

template <typename T>
void noref(T const x) {
    std::cout << "x in noref(T const&):"
        << typeid(x).name() << std::endl;
}

int main()
{
    ref("hello");
    noref("hello");
/*
    StackProS<int> dStack;
    for (int i = 0; i < 10; i++) {
        dStack.push(i*100);
    }
    
    StackProS<float> dStack2;
    StackProS<float, std::vector> vStack;
    dStack2 = dStack;
    vStack = dStack;
*/
    return 0;
}

