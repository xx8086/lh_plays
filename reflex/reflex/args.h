#pragma once

#include <iostream>

template <class T>
int printarg(T t) {
	std::cout << t << std::endl;
	return t;
}

template <class ...Args>
void expand(Args... args) {
	int arr[]{ (printarg(args), 0)... };//初始化列表+逗号表达式
}
//expand(1, 2, 3, 4, 5);
/*
逗号表达式会按顺序执行逗号前面的表达式，
对于 d=(a=b, c)：b会先赋值给a，接着括号中的逗号表达式返回c的值，因此d将等于c。

通过初始化列表来初始化一个变长数组,
{(printarg(args), 0)...}将会展开成((printarg(arg1),0), (printarg(arg2),0), (printarg(arg3),0),  etc... )，
最终会创建一个元素值都为0的数组int arr[sizeof...(Args)]。
此处数组的目的纯粹是为了在数组构造的过程使用初始化列表展开参数包。
{(printarg(args))...}或者{(0, printarg(args))...}则数组arr内为每一个printarg返回的值。
*/

template<class F, class... Args>void expand_lamda(const F& f, Args&&...args) {
	std::initializer_list<int> {(f(std::forward< Args>(args)), 0)...};
	//直接使用初始化列表
}
//expand_lamda([](auto i) {std::cout << i << std::endl; }, 1, 2, 3, "pomegranate");

///////////////////////////////////
//前向声明
template<typename... Args>
struct Sum;

//基本定义
template<typename First, typename... Rest>
struct Sum<First, Rest...> {
	enum { value = Sum<First>::value + Sum<Rest...>::value };
};

//偏特化，只有一个参数的时候递归终止
template<typename Last>
struct Sum<Last> {
	enum { value = sizeof(Last) };
};

//偏特化，没有参数时候的处理，此处用不到。
template<>
struct Sum<> {
	enum { value = 0 };
};
//int sum_value =  Sum<int, double, short>::value;

///////////////////////////////////
//整型序列的定义
template<int...>
struct IndexSeq {};

//继承方式，开始展开参数包
template<int N, int... Indexes>
struct MakeIndexes : MakeIndexes<N - 1, N - 2, Indexes...> {};

// 模板特化，终止展开参数包的条件
template<int... Indexes>
struct MakeIndexes<-3, Indexes...> {
	typedef IndexSeq<Indexes...> type;
};
/*
using T = MakeIndexes<3>::type;
T t;
MakeIndexes<3> t3;
std::cout << typeid(T).name() << std::endl;
std::cout << typeid(t3).name() << std::endl;
*/

////////////////////////
template<int N, int... Indexes>
struct MakeIndexes3 {//非递归继承方式生成整形序列
	using type = typename MakeIndexes3<N - 1, N - 2, Indexes...>::type;
};

template<int... Indexes>
struct MakeIndexes3<-3, Indexes...> {
	typedef IndexSeq<Indexes...> type;
};

/*
using T = MakeIndexes3<3>::type;
T t;
MakeIndexes3<3> t3;
std::cout << typeid(T).name() << std::endl;
std::cout << typeid(t3).name() << std::endl;
*/

/////////////////
template <class T, int M, int N, T... args>
struct Martix {
	Martix() : _m{ (args)... } {
	}
	T _m[M][N];
};

//Martix<int, 2, 3,
//	1, 2, 3, 4, 5, 6> test;