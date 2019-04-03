//
// Created by liuha on 2019/4/3.
//

#ifndef TEMPLATES_STUDIO_非类型模板参数的限制_HPP
#define TEMPLATES_STUDIO_非类型模板参数的限制_HPP
/*
template<double VAT>
double process(double v){
    return v * VAT;
}
// double dv = process<12.1>(13.0);
// error: a non-type template parameter cannot have type 'double'

template<std::string name>
class TemStr{
public:
    std::string _str = name;
};
// TemStr<"test"> strt;
// error: a non-type template parameter cannot have type 'std::string'
*/

template<char const* name>
class TemPtr{
public:
    std::string _str = name;
};
//extern char const* sa = "test";//ERROR
//error: non-type template argument of type 'const char *' is not a constant expression
extern char const sa[] = "test";//OK
#endif //TEMPLATES_STUDIO_非类型模板参数的限制_H
