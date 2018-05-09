#include "lhtask.h"
#include "lhcommon.h"
#include <thread>

LHTask::LHTask(){
}


LHTask::~LHTask(){
}

void LHTask::setdata(int d){
    _data = d;
}

void LHTask::run(){
    lhprintf("lhtsak_run_%d\n", _data);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void LHTask::callback(){
    lhprintf("lhcallback_%d\n", _data);
}

void LHTask::killself(){
    if (_bkillself){
        delete this;
    }
}