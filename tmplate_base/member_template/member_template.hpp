#pragma once
#include <deque>

template <typename T>
class Stack {
private:
    std::deque<T> elems;
public:
    void push(T const& t) {
        elems.push_back(t);
    }
    void pop() {
        elems.pop_front();
    }
    T top() const {
        return elems.front();
    }
    bool empty() { return elems.empty(); }
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

//---StackPro-------------------------------------------------
template<typename T, typename CONT = std::deque<T> >
class StackPro{
private:
    CONT elems;
public:
    void push(T t) {
        elems.push_back(t);
    }
    void pop() {
        elems.pop_front();
    }
    T top() const {
        return elems.front();
    }
    bool empty() {
        return elems.empty();
    }

    template<typename T2, typename CONT2>
    StackPro<T, CONT>const & operator= (StackPro<T2, CONT2> const & op2);
};

template<typename T, typename CONT>
template<typename T2, typename CONT2>
inline StackPro<T, CONT>const & StackPro<T, CONT>::operator=(StackPro<T2, CONT2> const & op2)
{
    elems.clear();
    StackPro<T2, CONT2>tmp(op2);
    while (!tmp.empty()) {
        push(tmp.top());
        tmp.pop();
    }

    return *this;
}

//---StackProS-------------------------------------------------
template<typename T,
    template<typename ELEM,
    typename ALLOC = std::allocator<ELEM> > class CONT = std::deque >
    class StackProS {
    private:
        CONT<T> elems;
    public:
        
        void pop() {
            elems.pop_front();
        }
        T top() const {
            return elems.front();
        }
        bool empty() {
            return elems.empty();
        }

        void push(T t);

        template<typename T2,
            template<typename ELEM2,
            typename ALLOC2 = std::allocator<ELEM2> > class CONT2 >
            StackProS<T, CONT>& operator= (StackProS<T2, CONT2> const&);
};

template<typename T, template<typename, typename> class CONT>
void StackProS<T, CONT>::push(T t) {
    elems.push_back(t);
}

template<typename T, template<typename, typename> class CONT >
template<typename T2, template<typename, typename>class CONT2 >
StackProS<T, CONT>&
StackProS<T, CONT>::operator= (StackProS<T2, CONT2> const& op2) {
    if ((void*)this == (void*)&op2) {
        return *this;
    }

    StackProS<T2, CONT2> tmp(op2);
    elems.clear();
    while (!tmp.empty()) {
        push(tmp.top());
        tmp.pop();
    }
    return *this;
}

