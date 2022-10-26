// Lazy Initialization
// * Using timing: Use this technique when there is quite a bit of overhead involved in creating or initializing the variable.
// * Lazy initialization may go wrong (having a data race) in the multithreaded program.
//   - multiple threads check the value of ptr (!ptr)
//   - one thread modify the value of the ptr (ptr = new some_type) 
// * For the new syntax (ptr = new some_type):
//   - Allocate enough memory to store a some_type object.
//   - Construct a some_type object in this memory.  (modify the value of the memory address)
//   - Store the address of the memory in ptr.  (modify the value of the memory address)
//   - C++ allows the aboves to be performed in a different order.
//   -> So it makes the following might happed:
//      1. Thread A is suspended after allocating the memory and assigning ptr.
//         ptr = new sizeof(some_type);
//      2. Before constructing the some_type object, another thread B calls process().
//         Thread B tests ptr before trying to acquire a lock.
//      3. Thread B sees that ptr has been set to the address of the allocated memory.
//      4. Thread B will not try to acquire a lock, but jumps straight to the do_it() call.
//         However, at the same time, the some_type object has not yet been constructed.
//      5. Thread B calls a member function of an unconstructed object.
//         This leads to the undefined behavior.
//   - In C++ 17, the order is defined in allocate enough memory -> construct a some_type object -> store the address in ptr.
// * We can solve the lazy initialization multithreading program problem by the following:
//   a. Add mutex. -> every thread (read / write) needs to lock the thread -> less efficiency.
//   b. Use std::call_once() -> It ensures that a function is only called once and 
//                              guarantees that it is be done in one thread and 
//                              the thread that is doing the call cannot be interrupted 
//                              until the function call has completed. This is threadsafe and 
//                              less overhead than a. using a mutex.
//   c. Use the singleton desing pattern with the static local variable implementation. 
//      (if we don't need that variable (ptr) anywhere else.)
//   d. Use the double-checked locking algorithm technique and compilers supporting C++17.
#include <iostream>
#include <mutex>    // a. add mutex
#include <thread>
#include <vector>

class some_type{
    // ...
public:
    void do_it() {/*...*/}
};

some_type* ptr{nullptr};      // Variable needs to be lazy initialized. (first set to the uninitialized value.)
// std::mutex the_mutex;         // a. add mutex
std::once_flag ptr_flag;         // b. std::call_once() this line is to construct a global instance and 
                                 //                     is used by the program to store synchronization data.

void process() {
    // std::unique_lock<std::mutex> lk(the_mutex);    // a. add mutex
    // if(!ptr){                 // First time variable has been used. // a. add mutex
    //     ptr = new some_type;  // Initialize it.                     // a. add mutex
    // }
    // lk.unlock();                                   // a. add mutex

    std::call_once(ptr_flag, []() { ptr = new some_type; });  // b. std::call_once() std::call_one will performed the body of the callable object 
                                                              // by determining the content of the std::once_flag.

    /* d. double-checked locking but does not work before C++17. (data race since the "new" allocation performing order)
    if(!ptr){
        std::lock_guard<std::mutex> lk(the_mutex);
        if(!ptr){                                  // second check of ptr
            ptr = new some_type;
        }
    }
    */
    ptr->do_it();             // Use it.
}

int main(){
    std::vector<std::thread> threads;
    for(int i = 0; i < 10; i++){
        threads.push_back(std::thread{ process });
    }
    for(auto& t: threads){
        t.join();
    }

    return 0;
}