#pragma once

#include "hashiem.hpp"

template <typename DataType>
class HashTable
{
public:
    HashTable(int m);
    ~HashTable();
public:
    int find(const DataType&) const;
    bool insert(const DataType&);
    bool deleteitem(const DataType&);

    bool isIn(const DataType&);
    DataType getvalue(int i) const;
private:
    HashItem<DataType> *ht;
    int tablemaxsize;
    int currentsize;
};
