#pragma once

#include <thread>
#include <mutex>
#include "task.h"
#include "thread.h"
#include "busy_thread_container.h"
#include "idle_thread_container.h"
#include "task_container.h"

class CLhThreadPool{
public:
    CLhThreadPool();
    CLhThreadPool(int number);
    virtual ~CLhThreadPool();
public:
    void addtask(CTask *, int );
    void add_idle_thread(int n);
    void remove_thread_on_busy(CThread *);
    void main_start();
    void endthreadpool();
private:
    void dotask(CTask *);
private:
    BusyThreadContainer _busy_thread_container;
    IdleThreadContainer _idle_thread_container;
    CTaskContainer _task_container;

    std::thread _thread_this;
    std::mutex _busy_mutex;
    std::mutex _idle_mutex;
    std::mutex _task_mutex;
    int _thread_counts = 0;
    bool _issurvive = false;
};

