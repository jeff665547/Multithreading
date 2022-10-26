// Deadlock
// * A thread is deadlock when it cannot run.
// * Deadlock often occurs when two or more threads are waiting for each other.
//   - Threads A waits for thread B to do something while B is waiting for A. 
//     (thread A and B are stuck waiting for each other.)
//   - Threads A and B are waiting for an event that can never happen.
// * Examples:
//   - Waiting for the mutex locked by another thread.
//   - Waiting for the computation result performed by another thread.
//   - Waiting for a message sent by another thread.
//   - Waiting for a GUI event produced by another thread.
// * Guidelines to avoid the deadlock.
//   - The basic rule: Never wait for a thread if there's a possibility it could wait for your thread 
//     (cycling deadlock e.g. Thread A waits for thread B, B waits for C, and thread C waits for A).
//   - Avoid nested locks.
//     1. If your thread already holds a lock, don't acquire another one (from another mutex).
//     2. If you need multiple locks, acquire them in a single operation.
//        -> (C++17) std::scoped_lock() -> the order of arguments doesn't matter -> Lock all mutexes or not lock either of them.
//        -> (C++11 or 14) lock multiple mutexes with the std::lock() and pass the std::adopt_lock/std::defer_lock option to the std::unique_lock().
//           adopt_lock is used to take the owernship of the lock (which is locked by other codes first).
//           defer_lock is used to create an unlocked lock (which will be locked by other codes later).
//   - Acquire locks in a fixed order.
//     1. If you need multiple locks and cannot acquire them in a single operation, acquire them in the
//        same order in every thread. -> May easily lose track in the large program.
//   - Avoid calling "unknown code" (where an unexpected lock will be put by the author of the unknown code) 
//     within a critical section, such as
//     1. Library functions (unless they are documented not to take locks)
//     2. Callbacks, function pointers, functors, etc.
//     3. Virtual functions
//     4. Template functions or member functions of a template class.
//     5. Plug-ins
// * Guidelines to avoid the non-locking deadlock.
//   - Deadlock can occur without locks, such as two threads call join() on each other.
//   - Avoid waiting for a thread while holding a lock. -> The other thread may need the lock to proceed.
//   - Try to avoid waiting for other threads.
//   - Call join() in the same function where the thread instance was created if possible.
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std::literals;


using namespace std;
std::mutex mutex1;
std::mutex mutex2;

/* deadlock example */
void func1(){
    std::cout << "Thread 1 is locking the mutex 1 ..." << std::endl;
    std::unique_lock<std::mutex> lk1(mutex1);  // Acquire the lock on the mutex1.
    std::cout << "Thread 1 has locked the mutex 1 ..." << std::endl;
    std::this_thread::sleep_for(50ms);         // Do some work.
    std::cout << "Thread 1 is locking the mutex 2 ..." << std::endl;
    std::unique_lock<std::mutex> lk2(mutex2);  // Wait for the lock on the mutex2.
    std::cout << "Thread 1 has locked the mutex 2 ..." << std::endl;
    std::this_thread::sleep_for(50ms);         // Do some work.
    std::cout << "Thread 1 releases locks ..." << std::endl;
}
void func2(){
    std::cout << "Thread 2 is locking the mutex 2 ..." << std::endl;
    std::unique_lock<std::mutex> lk2(mutex1);  // Acquire the lock on the mutex2.
    std::cout << "Thread 2 has locked the mutex 2 ..." << std::endl;
    std::this_thread::sleep_for(50ms);         // Do some work.
    std::cout << "Thread 2 is locking the mutex 1 ..." << std::endl;
    std::unique_lock<std::mutex> lk1(mutex2);  // Wait for the lock on the mutex1.
    std::cout << "Thread 2 has locked the mutex 1 ..." << std::endl;
    std::this_thread::sleep_for(50ms);         // Do some work.
    std::cout << "Thread 2 releases locks ..." << std::endl;
}

/* acquire multiple locks example */
void func3() {
    std::cout << "Thread 3 is locking the mutexes ..." << std::endl;
    std::scoped_lock lk1(mutex1, mutex2);
    std::cout << "Thread 3 has locked the mutexes ..." << std::endl;
    std::this_thread::sleep_for(50ms);
    std::cout << "Thread 3 releases locks ..." << std::endl;
}
void func4() {
    std::cout << "Thread 4 is locking the mutexes ..." << std::endl;
    std::scoped_lock lk1(mutex2, mutex1);
    std::cout << "Thread 4 has locked the mutexes ..." << std::endl;
    std::this_thread::sleep_for(50ms);
    std::cout << "Thread 4 releases locks ..." << std::endl;
}

/* unique_lock() with the adopt_lock option example */
void func5() {
    std::cout << "Thread 5 is locking the mutexes ..." << std::endl;
    std::lock(mutex1, mutex2);                                   // Lock both mutexes.
    std::cout << "Thread 5 has locked the mutexes ..." << std::endl;
    std::unique_lock<std::mutex> lk1(mutex1, std::adopt_lock);   // Associate each mutex with a unique_lock. Take the lock ownership from the mutex 1 and mutex 2.
    std::unique_lock<std::mutex> lk2(mutex2, std::adopt_lock);
    std::cout << "Thread 5 is releasing mutexes ..." << std::endl;
}
void func6() {
    std::cout << "Thread 6 is locking the mutexes ..." << std::endl;
    std::lock(mutex2, mutex1);                                   // Lock both mutexes.
    std::cout << "Thread 6 has locked the mutexes ..." << std::endl;
    std::unique_lock<std::mutex> lk1(mutex1, std::adopt_lock);   // Associate each mutex with a unique_lock. Take the lock ownership from the mutex 1 and mutex 2.
    std::unique_lock<std::mutex> lk2(mutex2, std::adopt_lock);
    std::cout << "Thread 6 is releasing mutexes ..." << std::endl;
}

/* unique_lock() with the defer_lock option example */
void func7() {
    std::cout << "Thread 7 is locking the mutexes ..." << std::endl;
    std::unique_lock<std::mutex> lk1(mutex1, std::defer_lock);   // Associate each mutex with a unique_lock. Wrap an unlocked lock object lk1 and lk2.
    std::unique_lock<std::mutex> lk2(mutex2, std::defer_lock);    
    std::lock(lk1, lk2);                                         // Lock both mutexes.
    std::cout << "Thread 7 has locked the mutexes ..." << std::endl;
    std::cout << "Thread 7 is releasing mutexes ..." << std::endl;
}
void func8() {
    std::cout << "Thread 8 is locking the mutexes ..." << std::endl;
    std::unique_lock<std::mutex> lk1(mutex1, std::defer_lock);   // Associate each mutex with a unique_lock. Wrap an unlocked lock object lk1 and lk2.
    std::unique_lock<std::mutex> lk2(mutex2, std::defer_lock);
    std::lock(lk2, lk1);                                         // Lock both mutexes.    
    std::cout << "Thread 8 has locked the mutexes ..." << std::endl;
    std::cout << "Thread 8 is releasing mutexes ..." << std::endl;
}

int main() {
    std::thread t1 {func1};
    std::thread t2 {func2};
    t1.join(); t2.join();

    std::thread t3 {func3};
    std::thread t4 {func4};
    t3.join(); t4.join();

    std::thread t5 {func5};
    std::thread t6 {func6};
    t5.join(); t6.join();

    std::thread t7 {func7};
    std::thread t8 {func8};
    t7.join(); t8.join();

    return 0;
}