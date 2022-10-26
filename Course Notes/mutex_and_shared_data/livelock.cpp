// Livelock
// * Contrast to the deadlock, when we have a livelock, all the threads are still active, 
//   however, when we have a deadlock, there are some threads, which are no longer active. 
//   They are waiting forever. 
// * Deadlock -> Everything grinds to a halt.
// * Livelock -> Things are still happening.
// * Causes:
//   - Badly doing deadlock avoidance.
//     e.g. Instead of a thread blocking indefinitely, when it cannot get a lock, it backs off
//          and try again.
// * Guide to avoid a livelock.
//   - Use the scoped_lock or std::lock() with the std::unique_lock() 
//     (std::adopt_lock/std::defer_lock) mentioned in the deadlock.
//   - Assign different priorities to the threads through std::thread's native_handle() to pass
//     the argument to the operating system API (depending on the OS we used). 
//     Set higher priority -> higher probability run first. (OS automatically done)
//     # Problem 1. Priority Inversion -- High priority thread needs some information from a low 
//                  priority thread. -> High priority thread has to wait the low priority thread.
//                  However, since low priority thread needs to wait other higher priority threads,
//                  high priority thread behaves like a low priority thread.
//     # Problem 2. Convoying -- High priority and low priority thread may both require locks, but
//                  low priority thread acquires the locks first. -> High priority thread is forced
//                  to wait the low priority to run first.
//   - Resource starvation -- A thread cannot get the resources it needs to run.
//     e.g. System memory exhausted. Exhaustion of maximum supported number of threads. Processor time limit.
//                  

#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std::literals;
std::timed_mutex mutex1, mutex2;

void func1(){
    std::this_thread::sleep_for(10ms);
    bool locked{false};
    while(!locked){
        std::lock_guard<std::timed_mutex> lk(mutex1);  // Lock the mutex 1
        std::this_thread::sleep_for(1s);
        std::cout << "After you, Jeff" << std::endl;
        locked = mutex2.try_lock_for(5ms);   // Try to get a lock on the mutex 2
    }
}

void func2(){
    std::this_thread::sleep_for(10ms);
    bool locked{false};
    while(!locked){
        std::lock_guard<std::timed_mutex> lk(mutex2);  // Lock the mutex 2
        std::this_thread::sleep_for(1s);
        std::cout << "After you, Amy" << std::endl;
        locked = mutex1.try_lock_for(5ms);  // Try to get a lock on the mutex 1
    }
}

int main(){

    std::thread t1{func1};
    std::thread t2{func2};
    t1.join(); t2.join();

    return 0;
}