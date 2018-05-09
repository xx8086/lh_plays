#pragma once
#include "Task.h"

class LHTask : public CTask{
    friend bool operator< (LHTask  &lv, LHTask &rv){
        return lv._priority < rv._priority;
    }
public:
    LHTask();
    LHTask(bool bkill) :_bkillself(bkill) {};
    virtual ~LHTask();

public:
    virtual void killself();
    virtual void run();
    virtual void callback();
    void setdata(int d);
private:
    bool _bkillself = false;
};

