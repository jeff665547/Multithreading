// recursive_mutex
// * Used when we want to lock a mutex which is already locked. (Generally, it leads to undefined behavior)
// * When it is used, that means the code design needs to be improved.
// * Its lock() member function can be called repeatedly.
//   - For each lock() call, there must be an unlock() call.
// * It works with lock_guard, unique_lock, scoped_lock. 
//   - std::lock_guard<std::recursive_mutex> lk(recursive_mutex_instance);

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std::literals;

std::recursive_mutex m;     // Use the recursive_mutex here since we lock the same mutex in the recursion.

int bad_factorial(int n){
    if(n <= 1) {
        std::cout << "Returning " << 1 << std::endl;
        return 1;
    }

    std::lock_guard<std::recursive_mutex> lg(m);
    int retval = n * bad_factorial(n - 1);
    std::cout << "Returning " << retval << std::endl;
    return retval;     // After the function return, the lock_guard will be unlocked.
}


// timed_mutex
// * Usage: All the functionalities of this class are owned by unique_lock. It can be a simplified version to improve the efficiency 
//          (use timed_mutex with lock_guard).
// * Three member functions:
//   - try_lock(): Return true (if the lock is available)/false (if the lock is not available) immediately.
//   - try_lock_for(): Keep trying to obtain the lock for the specified duration.
//   - try_lock_until(): Keep trying to obtain the lock until the specified time.
// * try_lock_for and try_lock_until may not return immediately since the scheduler (in the operating system) may think
//   that this thread is not sufficiently important to be woken up yet.
// * std::chrono::system_clock -- return whatever time the operating think it is (get the time from the operating system, can be changed from the operating system).
// * std::chrono::steady_clock -- return a time value always increasing steadly. Use it for intervals.
std::timed_mutex the_mutex;

void task1(){
    std::cout << "Task 1 is trying to get the lock." << std::endl;
    the_mutex.lock();
    std::cout << "Task 1 has the lock." << std::endl;
    std::this_thread::sleep_for(500ms);
    std::cout << "Task 1 is releasing the lock." << std::endl;
    the_mutex.unlock();
}

void task2(){
    std::this_thread::sleep_for(100ms);
    std::cout << "Task 2 is trying to get the lock." << std::endl;
    while(!the_mutex.try_lock_for(50ms)){
        std::cout << "Task 2 could not get the lock." << std::endl;
    }
    std::cout << "Task 2 has the lock." << std::endl;
    std::cout << "Task 2 is releasing the lock." << std::endl;
    the_mutex.unlock();
}

void task3(){
    std::this_thread::sleep_for(200ms);
    std::cout << "Task 3 is trying to get the lock." << std::endl;
    auto deadline = std::chrono::system_clock::now() + 90ms;
    while(!the_mutex.try_lock_until(deadline)){
        deadline = std::chrono::system_clock::now() + 90ms;
        std::cout << "Task 3 could not get the lock." << std::endl;
    }
    std::cout << "Task 3 has the lock." << std::endl;
    std::cout << "Task 3 is releasing the lock." << std::endl;
    the_mutex.unlock();
}

// std::try_lock() 
// * This function attempts to lock multiple mutexes and returns immediately if it cannot obtain all the locks.
//   - If failure, it returns the index of the mutex object that failed to lock. (start from 0)
//   - If success, it returns -1.

int main(){

    std::thread t1 {bad_factorial, 10};
    std::thread t2 {bad_factorial, 10};
    t1.join(); t2.join();

    std::thread t3 {task1};
    std::thread t4 {task2};
    std::thread t5 {task3};
    t3.join(); t4.join(); t5.join();

    return 0;
}