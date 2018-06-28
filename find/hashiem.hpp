#pragma once

enum KindOfItem{EmptyItem, ActiveItem, DeleteItem, };

template <typename DataType>
struct HashItem
{
    HashItem(KindOfItem i = EmptyItem) :info(i) {}
    HashItem(const DataType& D, KindOfItem i = EmptyItem) :data(D), info(i) {}

    bool operator == (const HashItem & a) { return data == a.data; }
    bool operator != (const HashItem & a) { return data != a.data; }

    DataType data;
    KindOfItem info;
};