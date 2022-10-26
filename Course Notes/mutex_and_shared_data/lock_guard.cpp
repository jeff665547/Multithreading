// Scenario: If an exception is thrown in the critical section, the mutex is left locked (the unlock() is never executed).
// 
// void print(std::string str){
//     for(int i = 0; i < 5; i++){
//         print_mutex.lock();                     // Lock the mutex before the critical region.
//         std::cout << str[0] << str[1] << str[2] << std::endl;
//         throw std::exception();                 // Exception thrown before calling unlock().
//         print_mutex.unlock();                   // Never gets called.
//         std::this_thread::sleep_for(50ms);
//     }
// }
// 
// Path: thorw exception -> The destrouctor are called for all the objects in the scope.
//       -> The thread jump out the loop and look for an exception handler.
//       -> No exception handler -> thread terminate -> program terminate.
// 
// Solution: Add the exception handler for the thrown in the critical section or use the lock_guard.
// 
// void print(std::string str){
//     for(int i = 0; i < 5; i++){
//         try {
//             print_mutex.lock();                     // Lock the mutex before the critical region.
//             std::cout << str[0] << str[1] << str[2] << std::endl;
//             throw std::exception();                 // Exception thrown before calling unlock().
//             print_mutex.unlock();                   // Never gets called.
//             std::this_thread::sleep_for(50ms);
            
//         } catch (const std::exception& e) {
//             print_mutex.unlock(); 
//         }
//     }
// }
// 
// * lock_guard -- A mutex wrapper class (The RAII concept).
// * Allocate the resource (lock on a mutex) in the constructer and release the resource (lock on a mutex) in the destructor.
// * Constructor: Take a mutex object as argument and locks it.
// * Destructor:  Unlocks the mutex.
// * If we create an object of the wrapper class on the stack (as a local variable), its mutex will always be unlocked when the object goes out of scope, even if an exception is thrown.
// * Before C++17: std::lock_guard<std::mutex> lk(mutex_instance);
// * In C++17: std::lock_guard lk(mutex_instance);
// 
// void print(std::string str){
//     for(int i = 0; i < 5; i++){
//            try {
//                std::lock_guard<std::mutex> lk(print_mutex);   // Create lock_guard - acquires the lock.
//                std::cout << str[0] << str[1] << str[2] << std::endl;  // Critical region.
//                // throw std::exception();                        // When the exeception is thrown, the destructor is automatically called for the lock_guard. -> release the lock.
//                std::this_thread::sleep_for(50ms);             // Non-critical region is locked as well (if this line is executed).
//            } catch (const std::exception& e) {                // The mutex has always been unlocked when we get to the catch handler.
//            }
//     }
// }
// 
// 
// * scoped_lock -- A mutex wrapper class, Lock more than one mutex at the same time.
// * Constructor: The mutexes are locked in the order given in the constructor call.
// * Destructor: The mutexes are unlock in the reverse order (relative to the constructor.) -> avoid deadlock when dealing with mutiple mutexes.
// * It can also be used when dealing with a single mutex (just like the lock_guard). -> std::scoped_lock lks(mutex_instance1)
// * Before C++17: std::scoped_lock<std::mutex> lks(mutex_instance1, mutex_instance2, ...);
// * In C++17: std::scoped_lock lks(mutex_instance1, mutex_instance2, ...);

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
        try {
            std::lock_guard<std::mutex> lk(print_mutex);
            std::cout << str[0] << str[1] << str[2] << std::endl;
            throw std::exception();                 // Exception thrown before calling unlock()
            std::this_thread::sleep_for(50ms);
            
        } catch (const std::exception& e) {
            // print_mutex.unlock(); 
        }
    }
}

int main(){
    std::thread t1{print, "abc"};
    std::thread t2{print, "def"};
    std::thread t3{print, "xyz"};
    t1.join(); t2.join(); t3.join();

    return 0;
}