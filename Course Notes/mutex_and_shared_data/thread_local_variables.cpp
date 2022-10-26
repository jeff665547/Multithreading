// thread-local variables
// * Use the thread_local keyword to declare a variable as local to a thread.
// * There is a separate instance for each thread (thread_local keyword), 
//   instead of a single instance which is shared by all threads (static keyword).
// * We can declare thread-local variables anywhere that we can declare a static variable.
//   - namespace scope
//     -> Constructed at or before the first use in a translation unit (preprocessing stage) e.g. dll file.
//   - static data members of a class
//     -> Constructed at or before the first use in a translation unit (preprocessing stage) e.g. dll file.
//   - local variables in a function
//     -> Initialized in the same way as static local variables (Only one thread can access and initialize 
//        a thread-local variable).
// * Thread-local variables are destroyed when the thread function returns.
// * The destructors are called in the reverse order of construction (the construction order is not guaranteed).
// * Application example: random number engine instance for each thread to generate same sequence for each thread.
#include <iostream>
#include <random>
#include <thread>

using namespace std::literals;

thread_local std::mt19937 mt;

void func(){
    std::uniform_real_distribution<double> dist(0, 1);  // Doubles in the range 0 to 1.

    for(int i = 0; i < 10; i++){
        std::cout << dist(mt) << std::endl;             // Generate 10 random numbers.
    }
}

int main(){

    std::cout << "Thread 1's random values: " << std::endl;
    std::thread t1{func};
    t1.join();
    std::cout << std::endl;
    std::this_thread::sleep_for(500ms);
    std::cout << "Thread 2's random values: " << std::endl;
    std::thread t2{func};
    t2.join();
    std::cout << std::endl;

    return 0;
}