//
// Created by liuha on 2019/4/3.
//
#ifndef TEMPLATES_STUDIO_非类型的函数模板参数_H
template  <typename T = int, int MAXSIZE = 100>
class Stack{
public:
    T elems[MAXSIZE];
};
//Stack<int, 20> int20stack;
//Stack<int, 40> int40stack;
//Stack<std::string, 40> str40stack;
    /*
     * 每个模板实列都有自己的类型，int20stack和int40stack也是不同类型，
     * 而且这种类型之间不存在显式或隐式类型转换，
     * 所以他们之间不能相互替换，更不能相互赋值。
     * */

template<typename T, int VAL>
T add_value(T const& x){
    return x + VAL;
}

#define TEMPLATES_STUDIO_非类型的函数模板参数_H

#endif //TEMPLATES_STUDIO_非类型的函数模板参数_H
