#include "thread.h"
#include "lhthread_pool.h"
#include "task.h"
#include "lhcommon.h"

CThread::~CThread(){
}

int CThread::s_threadnumber = 0;
CThread::CThread(CLhThreadPool *pool) :_threadpool(pool), _isdetach(true){
    s_threadnumber++;
    _tid = s_threadnumber;
}

void CThread::setisdetach(bool detach){
    _isdetach = detach;
}
void CThread::assign(CTask *t){
    _task = t;
}
void CThread::runthread(){
    lhprintf("thread_%d_%d_%d_run->", _task->getdata(), _tid, _threadid);
    _task->run();
    _task->callback();
    _task->killself();
    _task = nullptr;
    _threadpool->remove_thread_on_busy(this);
}

int CThread::getthreadid(){
    return _tid;
}
void CThread::startthread(){
    _thread = std::thread(&CThread::runthread, this);
    _isdetach ? _thread.detach() : _thread.join();
}

void CThread::taskback(){
    _task->callback();
}

bool operator==(CThread my1, CThread my2){
    return my1._tid == my2._tid;
}

bool operator!=(CThread my1, CThread my2){
    return !(my1._tid == my2._tid);
}
