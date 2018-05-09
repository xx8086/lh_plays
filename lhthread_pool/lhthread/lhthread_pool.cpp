#include "lhthread_pool.h"
#include "lhcommon.h"


CLhThreadPool::CLhThreadPool(){
}

CLhThreadPool::CLhThreadPool(int number){
    _issurvive = true;
    _thread_counts = number;
    _idle_thread_container.assign(number, this);
    _thread_this = std::thread(&CLhThreadPool::main_start, this);
    _thread_this.detach();
}

CLhThreadPool::~CLhThreadPool(){
}

void CLhThreadPool::endthreadpool(){
    _issurvive = false;
}

void  CLhThreadPool::add_idle_thread(int n){
    _idle_mutex.lock();
    _idle_thread_container.assign(n, this);
    _thread_counts += n;
    _idle_mutex.unlock();
}

void CLhThreadPool::main_start(){
    while (true){
        if (false == _issurvive){
            _busy_mutex.lock();
            if (_busy_thread_container.size() != 0){
                _busy_mutex.unlock();
                continue;
            }
            _busy_mutex.unlock();
            break;
        }

        _idle_mutex.lock();
        if (0 == _idle_thread_container.size()){
            _idle_mutex.unlock();
            continue;
        }
        _idle_mutex.unlock();

        _task_mutex.lock();
        if (0 == _task_container.size()){
            _task_mutex.unlock();
            continue;
        }

        CTask *task = _task_container.top();
        _task_container.pop();
        _task_mutex.unlock();
        dotask(task);
    }
}

void CLhThreadPool::dotask(CTask *task){
    _idle_mutex.lock();
    CThread * athread = _idle_thread_container.top();
    _idle_thread_container.pop();
    athread->assign(task);
    _idle_mutex.unlock();

    _busy_mutex.lock();
    _busy_thread_container.push(athread);
    _busy_mutex.unlock();
    athread->startthread();
}

void CLhThreadPool::addtask(CTask *task,  int priority = (CTask::PRIORITY::NORMAL)){
    task->setpriority(priority);
    _task_mutex.lock();
    _task_container.push(task);
    _task_mutex.unlock();
}

void CLhThreadPool::remove_thread_on_busy(CThread * _thread){
    _busy_mutex.lock();
    lhprintf("thread_%d_remove_form_buys\n", _thread->getthreadid());
    _busy_thread_container.erase(_thread);
    _busy_mutex.unlock();

    _idle_mutex.lock();
    _idle_thread_container.push(_thread);
    _idle_mutex.unlock();
}
