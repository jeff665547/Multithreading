// * Unique lock 
// * std::unique_lock has the same basic features as std::lock_guard.
// * Constructor: Locks the mutex. (the default basic one, call the lock() member function in its implementation.)
// * Destructor: Unlocks the mutex.
// * It is more powerful than lock_guard and scoped_lock, but slightly lower and requires more storage.
// * It has an unlock() function -> we can release the mutex before executing the non-critical area (avoid blocking other threads).
// * If the function returns before calling unlock() (e.g. throw an exception), the destructor will ensure the mutex is unlocked.
//   -> Guarantees that the lock is always released.
// * Other kinds of constructor (overloaded constructor): 
//   - Not lock the mutex (allows it to be locked later manually). -> std::defer_lock
//   - Transfer the ownership of an existing mutex lock from the caller (move semantics, can only be done within the same thread).
//     -> could make and return (by move) different types of lock depending on the constructor argument -> Factory desing pattern.
//   - Try to get the lock, but don't wait if unsuccessful.
//   - Try to get the lock, but only wait for a certain duration if uncessful.
//   - Try to get the lock, but only wait until a certain time point if uncessful.
// 
// * Guideline
//   - lock_guard: lock a single mutex for an entire scope.
//   - scoped_lock: lock multiple mutexes for an entire scope.
//   - unique_lock: 1. Extra flexibility to the mutex and lock is needed.
//                  2. Unlock within an entire scope is needed (not all scope is the critical section).

#include <iostream>
#include <mutex>
#include <thread>
#include <string>
#include <chrono>

using namespace std::literals;

std::mutex print_mutex;

void print(std::string str){
    for(int i = 0; i < 5; i++){
        try {
            std::unique_lock<std::mutex> lk(print_mutex);  // This is the default one (call the lock() member function internally).
            std::cout << str[0] << str[1] << str[2] << std::endl;
            throw std::exception();
            lk.unlock();
            std::this_thread::sleep_for(50ms);
        } catch(std::exception &e) {}
    }
}

int main(){

    std::thread t1 {print, "abc"};
    std::thread t2 {print, "def"};
    std::thread t3 {print, "xyz"};
    t1.join();  t2.join();  t3.join();

    return 0;
}