// Lazy evaluation: Generator functions which generate a sequence of numbers based on the 
// demand from the caller function.
#include <coroutine>
#include <iostream>
template<typename T>
class Generator{
public:
    struct promise_type;
    using coro_handle = std::coroutine_handle<promise_type>;
    Generator(coro_handle handle) : handle_(handle) {};  // This constructor will be called implicitly when the coroutine object get initialized.
    Generator(Generator&) = delete;
    Generator(Generator&&) = delete;
    bool resume(){  // -> It is used in the caller function, it is user-defined API.
        handle_.resume();       // -> call resume to generate the next value of the sequence
        return !handle_.done(); // -> return whether the coroutine has anymore suspension points or not. 
                                // If true, we can use get_value to retrieve that value.
    }
    T get_value(){  // -> It is used in the caller function, it is user-defined API.
        return handle_.promise().current_value; // return the value via promise_type 
                                                // object (promise()) to the calling function.
    }
    ~Generator(){
        if(handle_) handle_.destroy();
    }
private:
    coro_handle handle_;
};

template<typename T>
struct Generator<T>::promise_type {
    using coro_handle = std::coroutine_handle<promise_type>;
    auto get_return_object() {
        return coro_handle::from_promise(*this);
    }
    auto initial_suspend() {return std::suspend_always();}
    auto final_suspend() noexcept {return std::suspend_always();}
    auto yield_value(const T val){      // val is the argument that followed the co_yield operator.
        current_value = val;            // Set the val for the promise (promise_type), and that is 
                                        // used to retrieved in the coroutine handle object.
        return std::suspend_always();   // Suspend the execution.
    }
    void unhandled_exception() {
        std::terminate();
    }
    T current_value;
};

template<typename T>
Generator<T> getNum(T start, T step){
    for(int i = 0;; i++){
        co_yield start;                 // Set the start value for the promise, and suspend the execution afterwards.
        start += step;
    }
}

Generator<int> getNum(){
    return getNum(0, 2);
}

int main(){

    auto gen = getNum();   // return a generator (a wrapper of the coroutine handle object).

    for(int i = 0; i < 1000; i++){
        gen.resume();      // Call resume function to generate the next value of the sequence 
                           // (generate the value when the caller is needed).
        std::cout << " " << gen.get_value();  // Retrieve that value from the coroutine through 
                                              // the get_value function.
    }

    return 0;
}