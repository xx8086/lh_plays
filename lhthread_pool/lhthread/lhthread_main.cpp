// lhthread.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "lhthread_pool.h"
#include "lhtask.h"
#include <iostream>

#define TestNumber 5
int main(){
    CLhThreadPool lthreadpool(10);
    LHTask lt[TestNumber];
    for (int i = 0; i < TestNumber;i++){
        lt[i].setdata(i);
    }
    for (int i = 0; i < TestNumber; i++){
        lthreadpool.addtask(&lt[i], i);
    }
    
    int i = 0;
    LHTask j;
    while (true){
        std::cin >> i;
        if (i > 100){
            LHTask * j  = new LHTask(true);
            j->setdata(i);
            lthreadpool.addtask(j, i);
        }
        if (i == -1){
            lthreadpool.endthreadpool();
            break;
        }
    }
    system("pause");
    return 0;
}

