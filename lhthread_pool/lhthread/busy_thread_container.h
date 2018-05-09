#pragma once

#include <list>
class CThread;
class BusyThreadContainer
{
public:
    BusyThreadContainer();
    virtual ~BusyThreadContainer();
public:
    void push(CThread *m);
    int size();
    void erase(CThread *m);

private:
    typedef std::list<CThread*> Container;
    typedef Container::iterator Iterator;
    Container _busy_container;
};

