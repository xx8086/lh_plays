#pragma once
#include <string>

template <typename T, int MAXSIZE>
class Stack {
private: 
    T elems[MAXSIZE];
    int numElems;
public:
    Stack();
    void push(T const&);
    void pop();
    T top() const;
    bool empty() const {
        return 0 == numElems;
    }

    bool full() const {
        return MAXSIZE == numElems;
    }
};

template <typename T, int MAXSIZE>
Stack<T, MAXSIZE>::Stack():numElems(0){}

template <typename T, int MAXSIZE>
void Stack<T, MAXSIZE>::push(T const& elem) {
    if (full()) {
        throw std::out_of_range("Stack<>::push():stack is full");
    }
    elems[numElems++] = elem;
}

template <typename T, int MAXSIZE>
void Stack<T, MAXSIZE>::pop() {
    if (empty()) {
        std::string strerror("Stack<");
        strerror.append(typeid(T).name());
        strerror.append(">::pop():stack is empty");
        throw std::out_of_range(strerror);
    }
    numElems--;
}

template <typename T, int MAXSIZE>
T Stack<T, MAXSIZE>::top() const {
    if (empty()) {
        throw std::out_of_range("Stack<>::top():stack is empty");
    }
    return elems[numElems -1];
}


template<typename T, int VAL>
T addvalue(T const& t) {
    return t + VAL;
}

/* 1.0
template <double VAT>//error:������������Ϊ������ģ�����
double process(double v) {
    return v * VAT��
}

template <std::string name>//error:���������Ϊ������ģ�����
class MyName {
public:
    MyName() :_name(name) {}
private:
    std::string _name;
};
*/

/* .2
template <char const* name>
class MyChar {
public:
    MyChar() :_name(name) {}
private:
    std::string _name;
};
*/

/* .3
template <typename T>
class Base {
public:
    void eixt() {
        printf("base eixt\n");
    }
};

template <typename T>
class Derived : Base<T> {
public:
    void foo() {
        //exit();//error C2660: ��exit��: ���������� 0 ������
        //this->exit();//error C2039: ��exit��: ���ǡ�Derived<int>���ĳ�Ա
        //Base<T>::eixt();//OK
    }
};
*/