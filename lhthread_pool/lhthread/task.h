#pragma once

class CTask{
public:
	CTask();
	virtual ~CTask();

    enum  PRIORITY{
        MIN = 1, 
        NORMAL = 25, 
        MAX = 50,
    };
public:
    void setpriority(int priority);
    virtual void run() = 0;
    virtual void callback() = 0;
    virtual void killself() = 0;
    int getdata() { return _data; }
protected:
    int _priority;
    int _data = -1;
};

