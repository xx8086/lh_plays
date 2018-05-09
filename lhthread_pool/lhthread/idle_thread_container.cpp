#include "idle_thread_container.h"
#include "lhthread_pool.h"

IdleThreadContainer::IdleThreadContainer(){
}

IdleThreadContainer::~IdleThreadContainer(){
}

int IdleThreadContainer::size(){
    return _idle_container.size();
}

void IdleThreadContainer::push(CThread *m){
    _idle_container.push_back(m);
}
void IdleThreadContainer::pop(){
    _idle_container.pop_back();
}
void IdleThreadContainer::erase(CThread *m){
    _idle_container.erase(find(_idle_container.begin(), _idle_container.end(), m));
}
void IdleThreadContainer::assign(int number, CLhThreadPool *m){
    for (int i = 0; i < number; i++){
        CThread *p = new CThread(m);
        _idle_container.push_back(p);
    }
}

CThread* IdleThreadContainer::top(){
    return _idle_container.back();
}