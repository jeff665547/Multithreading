/* Jthread */
// * RAII version of traditional thread. (A thread that manages its lifecycle by itself.)
// * Its execution can be interrupted (via std::stop_token) after it is launched.
// * Check the compiler support (e.g. GCC 10 onwards) before using the jthread.
// * If we want to detach a thread, we must perform that action explicitly.
#include <thread>
#include <iostream>
#include <chrono>

using namespace std::literals;

void do_something(){
    std::cout << "Do something" << std::endl;
}

void do_something_interrupt(std::stop_token token){  // The stop_token tell the compiler 
                                                     // there will be interruptable 
                                                     // points in the function.
    int counter{0};
    while(counter < 10){
        if(token.stop_requested()){ // Introduce an interrupt point. stop_requested() 
                                    // retuns true if this thread is interrupted.
            return;
        }
        std::this_thread::sleep_for(0.2s);
        std::cerr << "This is interruptible thread: " << counter << std::endl;
        ++counter;
    }
}

void do_something_else_interrupt(){
    int counter{0};
    while(counter < 10){
        std::this_thread::sleep_for(0.2s);
        std::cerr << "This is non-interruptible thread: " << counter << std::endl;
        ++counter;
    }
}

int main(){
    std::thread thread1{do_something};
    thread1.join();

    std::jthread thread2{do_something};  // jthread manages its life cycle implicitly.
                                         // When the jthread destructor is called,
                                         // it first try to interrupt the work that 
                                         // it is carried on, then it will call 
                                         // join() on itself if the thread is joinable 
                                         // at the stage.
    
    std::jthread Interruptable{do_something_interrupt};   // No need to provide any arguments
    std::jthread nonInterruptable{do_something_else_interrupt};
    std::this_thread::sleep_for(1.0s);
    Interruptable.request_stop();        // When calling the thread.interrupt, the std::stop_token
                                         // will notify there is a request to stop the thread. Thus,
                                         // in the interruptible points (where we use the stop_token),
                                         // we can interrupt the execution.

    nonInterruptable.request_stop();     // For this thread, there is no interruption point 
                                         // (token.stop_requested()), and there is no std::stop_token 
                                         // provided. Thus, even though we call the interrupt, the 
                                         // thread still run until it finishes its execution.

    return 0;
} 