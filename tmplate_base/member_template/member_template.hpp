#pragma once
#include <deque>

template <typename T>
class Stack {
private:
    std::deque<T> elems;
public:
    void push(T const&);
    void pop();
    T top() const;

    template <typename T2>
    Stack<T> & operator= (Stack<T2> const&);
};

template<typename T>
template<typename T2>
Stack<T>& Stack<T>::operator= (Stack<T2> const& op2) {
    if ((void*)this == (void*)&op2) {
        return *this;
    }

    Stack<T2> tmp(op2);
    elems.clear();
    while (!tmp.empty()) {
        elems.push_front(tmp.top());
        tmp.pop();
    }

    return *this;
}

template<typename T, typename CONT = std::deque<T> >
class StackPro{
private:
    CONT elems;
public:
    template<typename T2, typename CONT2>
    StackPro<T, CONT>& operator= (StackPro<T2, CONT2>conts&);
};

template<typename T, typename CONT>
template<typename T2, typename CONT2>
StackPro<T, CONT>& 
StackPro<T, CONT>::operator= (StackPro<T2, CONT2>conts& op2) {
    return *this;
}