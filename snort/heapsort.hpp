#pragma once

template<class T>
class LHHeap
{
public:
    LHHeap(int);
    LHHeap(T[], int );
    ~LHHeap();
public:
    void PrintValue(const char*);
    void Insert(const T& item);
    T Delete();

private:
    bool MinHeap(const T& parent, const T& children);
    bool MaxHeap(const T& parent, const T& children);
    void InsertFilterUp(int i);
    void FilterDown(int i);
private:
    T *heaparray = nullptr;
    int heapsize;
    int heapmaxsize;
    bool masknewheap = false;
};

