#pragma once

#include<functional>
#include<chrono>
#include<thread>
#include<atomic>
#include<memory>
#include<mutex>
#include<condition_variable>

class Timer {
public:
    Timer();
    Timer(const Timer& t);
    ~Timer();
    void StartTimer(int interval, std::function<void()> task);
    void Expire();

    template<typename callable, class... arguments>
    void SyncWait(int after, callable&& f, arguments&&... args) {

        std::function<typename std::result_of<callable(arguments...)>::type()> task
        (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
        std::this_thread::sleep_for(std::chrono::milliseconds(after));
        task();
    }
    template<typename callable, class... arguments>
    void AsyncWait(int after, callable&& f, arguments&&... args) {
        std::function<typename std::result_of<callable(arguments...)>::type()> task
        (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

        std::thread([after, task]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }).detach();
    }

private:
    std::atomic<bool> expired_;
    std::atomic<bool> try_to_expire_;
    std::mutex mutex_;
    std::condition_variable expired_cond_;
};
