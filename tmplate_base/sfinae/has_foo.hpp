#pragma once
#include <iostream>

template <typename Type>
class has_foo
{
	class yes { char m; };
	class no { yes m[2]; };
	struct BaseMixin{
		void foo() {}
	};
	struct Base : public Type, public BaseMixin {};
	template <typename T, T t>  class Helper {};
	template <typename U>
	static no deduce(U*, Helper<void (BaseMixin::*)(), &U::foo>* = 0);
	static yes deduce(...);
public:
	static const bool result = sizeof(yes) == sizeof(deduce((Base*)(0)));
};

struct A {
	void foo();
};

struct B : A {};

struct C {};

void hasFoo(){
	std::cout << std::boolalpha << has_foo<A>::result << std::endl;
	std::cout << std::boolalpha << has_foo<B>::result << std::endl;
	std::cout << std::boolalpha << has_foo<C>::result;
}