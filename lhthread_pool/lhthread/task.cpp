#include "task.h"

CTask::CTask(){
}

CTask::~CTask(){
}

void CTask::setpriority(int priority){
    if (priority > (PRIORITY::MAX)){
        _priority = (PRIORITY::MAX);
    }
    else if (priority < (PRIORITY::MIN)){
        _priority = (PRIORITY::MIN);
    }
    else{
        _priority = priority;
    }
}