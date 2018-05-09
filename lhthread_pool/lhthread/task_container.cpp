#include "task_container.h"
#include "task.h"


CTaskContainer::CTaskContainer(){
}


CTaskContainer::~CTaskContainer(){
}

void CTaskContainer::push(CTask* t){
    _task_container.push(t);
}
CTask* CTaskContainer::top(){
    return _task_container.top();
}
void CTaskContainer::pop(){
    _task_container.pop();
}

int CTaskContainer::size(){
    return _task_container.size();
}