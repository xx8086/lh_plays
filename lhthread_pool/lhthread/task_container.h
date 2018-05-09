#pragma once
#include <queue>
class CTask;

class CTaskContainer{
public:
    CTaskContainer();
    virtual ~CTaskContainer();
public:
    void push(CTask *);
    CTask* top();
    void pop();
    int size();
private:
    std::priority_queue<CTask*> _task_container;
};

