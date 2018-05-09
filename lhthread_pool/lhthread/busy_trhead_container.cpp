#include "busy_thread_container.h"
#include "thread.h"


BusyThreadContainer::BusyThreadContainer(){
}


BusyThreadContainer::~BusyThreadContainer(){
}

void BusyThreadContainer::push(CThread *m){
    _busy_container.push_back(m);
}

void  BusyThreadContainer::erase(CThread *m){
    _busy_container.erase(find(_busy_container.begin(), _busy_container.end(), m));
}

int BusyThreadContainer::size(){
    return _busy_container.size();
}