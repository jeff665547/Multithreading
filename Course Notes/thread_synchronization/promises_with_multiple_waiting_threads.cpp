// Promises with multiple waiting threads
// * With the std::future, only one consumer thread can get the value from the producer.
//   - The future class is desinged to have exclusive access to the shared state with 
//     the promise.
//   - Future instances cannot be copied and can only be moved.
//   - If more than one thread calls get() on the same future, there is a data race.
//     # After the future object call get(), the shared state is no longer available. 
//       (since it is released.) -> if more than one thread calls get() -> some
//       threads will read the shared state, but some will modify it -> data race.
// 
// * std::shared_future allows multiple consumers to wait for a result from a producer.
//   - It can be multiply copied, so each thread has its own instance which has a reference
//     to the same shared state. (like the shared_ptr)
//   - Calling get() from different copies is safe.
//   - Initialization
//      - Usually, we move an existing future object into a shared_future.
//        std::promise<int> p;
//        std::future<int> f = p.get_future();
//        std::shared_future<int> sf1{std::move(f)};
//      - We can get a shared_future object from calling share() on the future object.
//        std::shared_future<int> sf1{f.share()};
//      - We can obtain a shared_future object directly from the promise object.
//        std::shared_future<int> sf1{p.get_future()};  // return a future and move it 
//                                                         into the shared future instance.
#include <future>
#include <iostream>
#include <thread>
#include <mutex>

using namespace std::literals;

std::mutex m;

void produce(std::promise<int>& px){   // Producer thread with the promise object
    int x {42};
    std::this_thread::sleep_for(5s);
    std::cout << "Promise sets the shared state to " << x << std::endl;
    px.set_value(x);      // Set the result into the shared state.
}

void consume(std::shared_future<int>& fx){   // Consumer thread with the (shared) future object
    std::cout << "Thread " << std::this_thread::get_id() << " calling get() ... " << std::endl;
    int x = fx.get();                   // Get the result.
    std::lock_guard<std::mutex> lg{m};  // Add the lock to avoid the output get scrambled up.
    std::cout << "Thread " << std::this_thread::get_id() << " returns from calling get()" << std::endl;
    std::cout << "Thread " << std::this_thread::get_id() << " has answer " << x << std::endl;
}

int main(){

    // One producer thread and multiple consumer threads
    std::promise<int> p;
    std::shared_future<int> sf1{p.get_future()};   // Move p's future into a shared future.
    std::shared_future<int> sf2 = sf1;             // Create a separate instance of shared future.

    std::thread fut1{consume, std::ref(sf1)};
    std::thread fut2{consume, std::ref(sf2)};
    std::thread prom{produce, std::ref(p)};

    fut1.join();
    fut2.join();
    prom.join();

    return 0;
}