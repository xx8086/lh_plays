//
// Created by liuha on 2019/4/3.
//
#include <iostream>
#include <vector>
#include "templates.h"
#include "非类型的函数模板参数.hpp"
//#include "非类型模板参数的限制.hpp"

void templates::run(){

    nontype_template_parameters();
}

void templates::nontype_template_parameters(){
    std::vector<int> src = {1,2,3,4,5};
    std::vector<int> dst(5);
    std::vector<int> dst_auto(5);
    std::transform(src.begin(), src.end(), dst.begin(),
            (int(*)(int const&))add_value<int, 5> );
    std::transform(src.begin(), src.end(), dst_auto.begin(),
            add_value<int, 5> );
    /*
     * 模板实列通常看成用来命名一组重载函数的集合（即使只有一个函数）。
     * 重载函数的集合不能被用于模板参数演绎。需要把这个函数模板的实参强制类型转为具体类型。
     * 目前C++11已经解了决这个问题，只有在考虑可移植的情况才需要使用这种强制转换。
     * */
}