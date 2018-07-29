#pragma once

#include <iostream>

template <class T>
int printarg(T t) {
	std::cout << t << std::endl;
	return t;
}

template <class ...Args>
void expand(Args... args) {
	int arr[]{ (printarg(args), 0)... };//��ʼ���б�+���ű��ʽ
}
//expand(1, 2, 3, 4, 5);
/*
���ű��ʽ�ᰴ˳��ִ�ж���ǰ��ı��ʽ��
���� d=(a=b, c)��b���ȸ�ֵ��a�����������еĶ��ű��ʽ����c��ֵ�����d������c��

ͨ����ʼ���б�����ʼ��һ���䳤����,
{(printarg(args), 0)...}����չ����((printarg(arg1),0), (printarg(arg2),0), (printarg(arg3),0),  etc... )��
���ջᴴ��һ��Ԫ��ֵ��Ϊ0������int arr[sizeof...(Args)]��
�˴������Ŀ�Ĵ�����Ϊ�������鹹��Ĺ���ʹ�ó�ʼ���б�չ����������
{(printarg(args))...}����{(0, printarg(args))...}������arr��Ϊÿһ��printarg���ص�ֵ��
*/

template<class F, class... Args>void expand_lamda(const F& f, Args&&...args) {
	std::initializer_list<int> {(f(std::forward< Args>(args)), 0)...};
	//ֱ��ʹ�ó�ʼ���б�
}
//expand_lamda([](auto i) {std::cout << i << std::endl; }, 1, 2, 3, "pomegranate");

///////////////////////////////////
//ǰ������
template<typename... Args>
struct Sum;

//��������
template<typename First, typename... Rest>
struct Sum<First, Rest...> {
	enum { value = Sum<First>::value + Sum<Rest...>::value };
};

//ƫ�ػ���ֻ��һ��������ʱ��ݹ���ֹ
template<typename Last>
struct Sum<Last> {
	enum { value = sizeof(Last) };
};

//ƫ�ػ���û�в���ʱ��Ĵ����˴��ò�����
template<>
struct Sum<> {
	enum { value = 0 };
};
//int sum_value =  Sum<int, double, short>::value;

///////////////////////////////////
//�������еĶ���
template<int...>
struct IndexSeq {};

//�̳з�ʽ����ʼչ��������
template<int N, int... Indexes>
struct MakeIndexes : MakeIndexes<N - 1, N - 2, Indexes...> {};

// ģ���ػ�����ֹչ��������������
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
struct MakeIndexes3 {//�ǵݹ�̳з�ʽ������������
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