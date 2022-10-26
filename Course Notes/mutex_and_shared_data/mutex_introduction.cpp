// Avoid data races -- Mutex mechanism.

// * Analogy: Bathroom Protocol
// * Request: Only one person should be able to access the bathroom at a time.
// * Solution: Put a lock on the door! 
//             The door is unlocked:
//             - Lock the door lock   -> Lock the mutex (mutual exculsion) object.
//             - Access the bathroom  -> Access the critical section.
//             - Unlock the door lock -> Unlock the mutex object.
//             The door is locked:
//             - Wait until the door is unlocked.
// * Critical section: A region of code that must execute in isolation from 
//                     the rest of the program (execute without other parts of the 
//                     program interferring with it).
// * Mutex: 
//   - A data structure that has only two states: "locked" and "unlocked".
//   - Two or more users of a resource agree to use the mutex to control access to that resource.
//   - The mutex prevents more than one user accessing that resource at a time.
//   - The ordering of users is still interleaved, but each thread's output (result) is separate from the others 
//     (prevent the threads from interfering with each other).
//   - A mutex must be defined outside any thread functions, but visible in them.
//     - global/static variable with global thread functions.
//     - class data member with member thread functions.
//   - Three member functions:
//     1. lock() tries to lock the mutex object, waits and block the execution if the mutex is not available 
//        (i.e. not return until the mutex has been locked by the calling).
//     2. try_lock() tries to lock the mutex object, returns immediately without block if the mutex is not available 
//        (returns true: the mutex has been locked by the calling, false: the mutex is still in used by other threads).
//     3. unlock() releases the lock on the mutex, so that other threads can enter the mutex.
//   - Calling lock() requires a corresponding call to unlock() even if an exception is thrown.
// 
// 
// * Process with the mutex (avoiding the data race):
//   1. Threads A, B, C are trying to lock the mutex.
//   2. Thread A locks the mutex.
//   3. Thread B, C which are trying to lock the mutex must wait.
//   4. Thread A has exclusive access to the resource and executes its "critical region".
//   5. Thread A unlocks mutex.
//   6. One of threads B, C will lock the mutex and access the resource.
//   - Thread A's access to the resource "happens before" any access by B and C (impose an ordering on the thread).
//   - If A modifies the resource, the new value will be seen by the other threads when they access it.

#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <string>
#include <chrono>

using namespace std::literals;
std::mutex print_mutex;

void print(std::string str){
    for(int i = 0; i < 5; i++){
        print_mutex.lock();
        std::cout << str[0] << str[1] << str[2] << std::endl;
        print_mutex.unlock();
        std::this_thread::sleep_for(50ms);
    }
}

// Thread-safe vector class
class Vector{
    std::mutex mu;                // mutex as private class data member.
    std::vector<int> v;           // shared data mutex protects access to it.
public:
    void push_back(const int& i){
        mu.lock();                // lock the mutex.
        v.push_back(i);           // critical region.
        mu.unlock();              // unlock the mutex.
    }
    int get(const int& i){
        return v[i];
    }
};


std::mutex the_mutex;

void task1(){
    std::cout << "Task1 trying to get the lock." << std::endl;
    the_mutex.lock();
    std::cout << "Task1 has lock. Critical section ..." << std::endl;
    std::this_thread::sleep_for(500ms);
    std::cout << "Task1 releasing the lock." << std::endl;
    the_mutex.unlock();
}

void task2(){
    std::this_thread::sleep_for(100ms);
    std::cout << "Task2 trying to get the lock." << std::endl;
    while(!the_mutex.try_lock()){
        std::cout << "Task2 could not get the lock." << std::endl;
        std::this_thread::sleep_for(100ms);
    }
    std::cout << "Task2 has lock. Critical section ..." << std::endl;
    the_mutex.unlock();
}

int main(){
    std::thread t1{print, "abc"};
    std::thread t2{print, "def"};
    std::thread t3{print, "xyz"};
    t1.join(); t2.join(); t3.join();

    Vector v1;
    std::vector<int> backup {5, 6, 7, 8, 9, 0};
    for(int i = 0; i < 6; i++){
        std::thread vt1{ &Vector::push_back, &v1, backup[i] };
        std::thread vt2{ &Vector::push_back, &v1, backup[i] };
        std::thread vt3{ &Vector::push_back, &v1, backup[i] };
        std::thread vt4{ &Vector::push_back, &v1, backup[i] };
        vt1.join(); vt2.join(); vt3.join(); vt4.join();
    }
    for(int i = 0; i < 24; i++){
        std::cout << v1.get(i) << " ";
    }
    std::cout << std::endl;


    std::thread t4{ task1 };
    std::thread t5{ task2 };
    t4.join(); t5.join();

    return 0;
}