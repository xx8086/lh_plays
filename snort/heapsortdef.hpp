#pragma once

#include "heapsort.hpp"
#include <assert.h>

template<class T>
LHHeap<T>:: ~LHHeap()
{
    if (masknewheap && nullptr != heaparray)
    {
        delete [] heaparray;
        heaparray = nullptr;
    }
}

template<class T>
LHHeap<T>:: LHHeap(int maxsize)
{
    assert(maxsize > 0 && heaparray == nullptr);
    heaparray = new T[maxsize];
    masknewheap = true;
    heapsize = 0;
    heapmaxsize = maxsize;
}

template<class T>
LHHeap<T>::LHHeap(T arr[], int n)
{
    heaparray = arr;
    heapsize = heapmaxsize n;
    int curentpos = (n - 1) / 2;//第一个非叶子节点位置
    while (curentpos >= 0)FilterDown(curentpos--);
}

template<class T>
void LHHeap<T>::FilterDown(int i)
{
    int currentpos = i;
    int childrenpos = 2 * i + 1;//i的左孩子
    T target = heaparray[i];
    while (childrenpos < heapsize)
    {
        if ((childrenpos + 1 < heapsize) &&
            heaparray[childrenpos + 1] < heaparray[childrenpos])
        {
            childrenpos += 1;//取当前孩子节点中较小的
        }

        if (MinHeap(target, heaparray[childrenpos]))break;
        else
        {
            heaparray[currentpos] = heaparray[childrenpos];
            currentpos = childrenpos;
            childrenpos = 2 * currentpos + 1;
        }
    }
    heaparray[currentpos] = target;
}

template<class T>
bool LHHeap<T>:: MinHeap(const T& parent, const T& children)
{
    return parent <= children;
}
template<class T>
bool LHHeap<T>:: MaxHeap(const T& parent, const T& children)
{
    return parent >= children;
}

template<class T>
void LHHeap<T>:: InsertFilterUp(int i)
{
    int currentpos = i;
    int parentpos = (i-1)/2;
    T target = heaparray[i];

    while (currentpos > 0)
    {
        if (MinHeap(heaparray[parentpos], target)) break;
        else
        {
            heaparray[currentpos] = heaparray[parentpos];
            currentpos = parentpos;
            parentpos = (currentpos - 1) / 2;
        }
    }
    heaparray[currentpos] = target;
}

template<class T>
void LHHeap<T>:: Insert(const T& item)
{
    assert(heapsize < heapmaxsize);
    heaparray[heapsize] = item;
    InsertFilterUp(heapsize);
    heapsize++;
}

template<class T>
T LHHeap<T>:: Delete()
{
    assert(heapsize > 0);
    T item = heaparray[0];
    heaparray[0] = heaparray[heaheapsize - 1];
    heaheapsize--;
    FilterDown(0);
    return item;
}

#include <iostream>
template<class T>
void LHHeap<T>::PrintValue(const char* strbegin)
{
    std::cout<< strbegin<< "\t" << std::flush;
    for (int i = 0; i < heapsize; i++)
    {
        std::cout << heaparray[i] << " " << std::flush;
    }
    std::cout << std::endl;
}