// Futures and Promises
// * Goal: Help us transfer data between different threads with higher 
//         level objects (instead of using threads and locks directly).
// 
// * Background: In the traditional C++ threads, it does not provide any 
//               direct way to return a value from the thread function. 
//               In that case, we used locks to protect shared variables 
//               to transfer data from one thread to another thread.
// 
// * Shared state: It is used to communicate between a future object and
//                 a promise object to transfer data in a thread-safe way
//                 (instead of using shared data variables or any explicit
//                  locking).
// 
// * Producer-consumer model (just like the reader and writer threads): 
//   - Producer thread: 
//     # Generate some data or calculates a result.
//     # It has a promise object -- set the value (can be viewed as the return 
//       value of the thread func.) in the shared state.
//     # The promise object is like the writer thread, which updated the value
//       of a shared variable.
//   - Consumer thread: 
//     # Wait and use the returned data.
//     # It has a future object -- wait for the shared state to be updated. When
//       the shared state is updated, the future object will return (by calling 
//       get() on the future object) that return value (which is set in the promise 
//       object).
//     # The future object is like the reader thread, which calls wait() to wait
//       the notification from the writer thread. After that, the reader thread 
//       returns from wait() and being able to access the shared variable.
// 
// Future object
// * The future object not only use with the promise object, it also be used in 
//   lots of asynchronous opertations.
// * The future object (std::future()) is not usually created directly. It is 
//   obtained from other asynchronous objects and operations, so these operations
//   will go and run in the background, and when these operations finish, the 
//   future will hold the result. When we have a promise and future, we create 
//   the promise first, and that will give us a future which is associated with it.
//   (std::promise<int> p;   auto f{p.get_future()};) 
//    -> This will set up a shared state between the promise instance and the future
//       instance, and that can be used to send a result from the promise to the 
//       future in a thread-safe way.
// * Futures are move-only object (cannot be copied), so we need to initialized them by move.
// * The future object is templated. The template parameter is the type of the result
//   that is being passed from the promise to the future. 
// * The future object provide member function get() to obtain the returned value 
//   after the asynchronous operation is complete (get() blocks until the operation
//   is complete).
// * If we call get() and the operation has already finished, then it will return 
//   immediately.
// * The future object also provide wait() to block until the asynchronous operation 
//   is complete (, but without getting the value).
// * The future object also provide wait_for() and wait_until() to block with a timeout
//   (, but without getting the value).
// 
// 
// Promise object
// * get_future() will return the std::future object associated with this promise.
// * set_value() sets the argument in the shared state.
// * set_exception() indicates that an exception has occured, and that can be stored in
//   the shared state, and be rethrown in the consumer thread by the future's get() call.

#include <future>
#include <thread>
#include <iostream>

using namespace std::literals;

void produce(std::promise<int>& px){  // Producer functin with promise.
    try{
        int x{42};
        std::this_thread::sleep_for(5s);
        throw std::out_of_range("Oops");
        std::cout << "Promise sets shared state to " << x << std::endl;
        px.set_value(x);              // Set the result. (Store the value 
                                      // of x in the shared state.)
    } 
    catch (...) {
        px.set_exception(std::current_exception()); // std::current_exception()
                                                    // will return the active 
                                                    // exception.
    }
}

void consume(std::future<int>& fx){   // Consumer function with future.
    try{
        std::cout << "Future object call get()..." << std::endl;
        int x = fx.get();               // Get the result. (It will wait until 
                                        // the shared state is updated, and then
                                        // the function call (fx.get()) will return 
                                        // and the shared state will be stored in 
                                        // x.)
        std::cout << "Future object returns from calling get()" << std::endl;
        std::cout << "The answer is " << x << std::endl;
    } catch (std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
}

int main(){

    // One producer thread and one consumer thread.

    std::promise<int> p;                       // Create the promise instance.
    std::future<int> f{p.get_future()};        // Get the associated future object.

    std::thread fut{consume, std::ref(f)};     // Start consumer thread with future.
    std::thread prom{produce, std::ref(p)};    // Start producer thread with promise.

    fut.join();
    prom.join();

    // For the nomal execution, the consumer thread calls get() and goes to sleep. 
    // The producer thread sets the value in the shared state. The consumer thread 
    // then wake up, and get() returns the value, and then x is 42. 
    // For the exception handling version, after the consumer thread calls get() 
    // and goes to sleep, the provider first throw an exception, and after that, 
    // it stores that exception in the shared state through the set_exception().
    // The consumer thread then wake up, and get() thow the exception in the try
    // block of the consumer. The catch block of the consumer will then handle 
    // that exception.

    return 0;
}