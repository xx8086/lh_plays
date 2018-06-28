#pragma once

#include "hashtable.hpp"

template <typename DataType>
HashTable<DataType>::HashTable(int m)
{
    tablemaxsize = m;
    currentsize = 0;
    ht = new HashItem<DataType>[tablemaxsize];
}

template <typename DataType>
HashTable<DataType>::~HashTable()
{
    if (nullptr != ht)
    {
        delete[] ht;
        ht = nullptr;
    }
}

template <typename DataType>
int HashTable<DataType>::find(const DataType& x) const
{
    int i = x % tablemaxsize;
    int j = i;
    //int f = 1;
    while (ht[j].info  == ActiveItem &&
        ht[j].data != x)//存在冲突
    {
        j = (j + 1) % tablemaxsize;//线性探测
       // j = (j + f*f) % tablemaxsize; //二次探测
       // f++

        if (j == i)//已遍历整个哈希表任未找到且表已填满
        {
            return -tablemaxsize;
        }
    }

    if (ht[j].info == ActiveItem)return j;

    return -j;
}

template <typename DataType>
bool HashTable<DataType>::insert(const DataType& x)
{
    int i = find(x);
    if (i > 0)return true;//元素已经存在
    else if (i != -tablemaxsize)//元素不存在且哈希表未满
    {
         ht[-i].data = x;
         ht[-i].info = ActiveItem;
         currentsize++;
         return true;
    }

    return false;
}

template <typename DataType>
bool HashTable<DataType>::deleteitem(const DataType& x)
{
    int i = find(x)
    if(i >= 0)
    {
        ht[i].info = DeleteItem;
        currentsize--;
        return true;
    }

    return false;
}

template <typename DataType>
bool HashTable<DataType>::isIn(const DataType& x)
{
    int i = fine(x);
    return i >= 0;
}

template <typename DataType>
DataType HashTable<DataType>::getvalue(int i) const
{
    return ht[i].data;
}