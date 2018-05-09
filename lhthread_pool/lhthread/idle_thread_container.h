#pragma once
#include <vector>

class CThread;
class CLhThreadPool;

class IdleThreadContainer{
public:
    IdleThreadContainer();
    virtual ~IdleThreadContainer();
public:
    int size();
    void push(CThread *m);
    void assign(int n, CLhThreadPool* m);
    void pop();
    void erase(CThread *m);
    CThread* top();
private:
    typedef std::vector<CThread*> Container;
    typedef Container::iterator Iterator;
    Container _idle_container;
};

