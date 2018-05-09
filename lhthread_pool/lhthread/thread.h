#pragma once

#include <thread>

class CLhThreadPool;
class CTask;
class CThread{
public:
    CThread(CLhThreadPool *pool);
    virtual ~CThread();
public:
    friend bool operator==(CThread my1, CThread my2);
    friend bool operator!=(CThread my1, CThread my2);
public:
    void setisdetach(bool isdetach);
    void assign(CTask *Task);
    void runthread();
    void startthread();
    void taskback();
    int getthreadid();
    
private:
    static int  s_threadnumber;
private:
    bool _isdetach = false;
    int _tid = -1;
private:
    CLhThreadPool *_threadpool;
    CTask *_task;
    std::thread _thread;
};

