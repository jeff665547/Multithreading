// 編譯參數：-std=c++20 -fcoroutine -pthread
// Coroutine 就是要解決schedule的問題
// * 一般的thread比較像是kernel thread -> 統一由作業系統去管理的
//   Coroutine是由User自己管理，自己做切換的，為一種比thread更細小的單位。
// * 傳統上，一個thread所負責的工作可以變成是一個coroutine去負責，由coroutine自行設計是否需要新開一個thread。 
//   -> 效能較不會被硬體thread(core)的數目給綁住。 -> 因此就比較不需要thread pool了
//      (thread pool是因為硬體core的數目有限而設計的)。
#include <iostream>
#include <coroutine>  // 寫coroutine時，需要include coroutine的header檔


// The following is a customed class (API) wrapping and manipulating the 
// coroutine handle to interact with the coroutine.
template<typename T> // C++ compiler要求此wrapping class一定要是templated class。
class Generator{
public:
    struct promise_type;  // C++規定此類別內要有promise_type(定義或宣告) 
    // -> Nested calss design pattern for wrapping interface.
    // Compiler 會自動去 call promise_type 裡面對應的 interface.

    using coro_handle = std::coroutine_handle<promise_type>;
    // 建立此 wrapping class 時要提供 coroutine handle 當成參數。
    // 以便後續對 coroutine handle 進行操作。
    Generator(coro_handle h) : handle(h) {}
    bool resume() {
        if(!handle.done()) handle.resume();  // handle.resume() 與 handle() 意思一樣，都是接著繼續執行。
        return !handle.done(); // done() is used to checks if a suspended coroutine is suspended 
                               // at its final_suspended point.
    }
    T get_value() {
        return handle.promise().value;
    }
    ~Generator() {
        if(handle) handle.destroy();  // handle returns true when it meets its
                                      // final suspension point.
    }
    coro_handle handle;  // Use the coroutine handle to control the coroutine object.
};

// The following is the promise object.
template<typename T>
struct Generator<T>::promise_type{
    // (Essential) Returns the constructed coroutine object. (Store the coroutine 
    //             handle associated with the promise_type object in the 
    //             returned Generator (customized class) object)
    //             The return type of this method has to be the same as the 
    //             outer nested class.
    Generator get_return_object(){
        return std::coroutine_handle<promise_type>::from_promise(*this);
    }
    // (Essential) (return awaitables) Determines if the coroutine suspends 
    //             before it runs (at its beginning).
    //             如果是使用std::suspend_always則產生出來的coroutine會被暫停，
    //             需要對coroutine額外使用resume()才會使coroutine繼續。
    std::suspend_always initial_suspend() {
        return {};
    }
    // (Essential) (return awaitables) Determines if the coroutine suspends one last time
    //             after the coroutine has ended execution (at its end).
    //             - If it returns suspend_always, that means the state is saved, and users of the 
    //               coroutine can access data in this promise_type or do whatever users intend to do.
    //               In this case, the coroutine handle should be manually destroyed by the wrapping 
    //               class (i.e. Generator in this case).
    std::suspend_always final_suspend() noexcept { 
        // The noexcept keyword is required in this function by the C++ compiler and the protocol.
        // The noexcept keyword can optimize the compiler code (the concept of reducing try-catch)
        // to improve the execution speed.
        return {};
    }
    // (Optional) (return awaitables) Is invoked by co_yield val. co_yield val -> yield_value(val)
    //             將 val 存在 value 中，再透過 coroutine handle 的 promise 去取得。
    std::suspend_always yield_value(T val) {
        value = val;
        return {};
    }
    // (Essential) (return awaitables) Called when an exception happens.
    void unhandled_exception() {
        std::terminate();
    }
    T value;  // This value can be accessed by the coroutine_handle.promise().
};


// The awaitables
// * The Awaitable suspend_always always suspends.
// * The Awaitable suspend_never never suspends.
// * std::suspend_always and std::suspend_never are the basic 
//   building blocks for functions


// The coroutine frame
// * Internal, typically heap-allocated state.
// * 它包含了 promise object, the coroutine’s copied parameters,
//   the representation of the suspension points, 
//   local variables (無論已經死亡或是還活著)

// The following is the coroutine factory.
// * A coroutine factory that returns a coroutine (generator) object.
// * A function becomes a coroutine factory if it uses co_return, co_await, 
//   co_yied, or a co_await expression in a range-based for loop.
// * The co_await operator is used to suspend execution until resumed.
// * The co_return keyword is used to complete execution and optionally return a value.
// * The co_yield keyword is used to suspend execution and return a value.
template<typename T>
Generator<T> infinite_seq(T begin, T step){
    T value = begin;
    for(int i = 0;; i++){
        co_yield value;
        value += step;
    }
}

// The workflow of the coroutine
// * The transformed coroutine (the compiler automatically runs).
// {
//     Promise prom;  // -> the promise obejct.
//     co_await prom.initial_suspend();  // -> the awaiter workflow.
//     try {
//         <function body having co_return, co_yield, or co_await>
//     }
//     catch (...) {
//         prom.unhandled_exception();
//     }
// // FinalSuspend:
//     co_await prom.final_suspend();    // -> the awaiter workflow.
// }
// Explanation
// • Coroutine begins execution
//   - allocates the coroutine frame if necessary.
//   - copies all function parameters to the coroutine frame.
//   - creates the promise object prom.
//   - calls prom.get_return_object() to create the coroutine handle, 
//     keeps it in a local variable (in the customed class), and 
//     construct an instance of the customed class. The 
//     result (the instance) of the call will be returned to the 
//     caller when the coroutine first suspends.
//   - calls prom.initial_suspend() and co_awaits its result. 
//     The promise type typically returns suspend_never for 
//     eagerly-started coroutines or suspend_always for 
//     lazily-started coroutines.
//   - the body (function body) of the coroutine is executed after co_await 
//     prom.initial_suspend() resumes.
// • Coroutine reaches a suspension point
//   - the return object (prom.get_return_object(), the customed class) 
//     is returned to the caller which resumed the coroutine.
// • Coroutine reaches co_return
//   - calls prom.return_void() for co_return or co_return expression, 
//     where expression has type void.
//   - calls prom.return_value(expression) for co_return expression, 
//     where expression has non-void type.
//   - destroys all stack-created variables.
//   – calls prom.final_suspend() and co_awaits its result.
// • Coroutine is destroyed (by terminating via co_return an uncaught 
//   exception, or via the coroutine handle)
//   - calls the destruction of the promise object.
//   – calls the destructor of the function parameters. 
//   – frees the memory used by the coroutine frame. 
//   – transfers control back to the caller.
// When a coroutine ends with an uncaught exception, the following happens:
// • catches the exception and calls prom.unhandled_exception() from the 
//   catch block.
// • calls prom.final_suspend() and co_awaits the result.
// 
// 
// * The transformed awaiter workflow
// awaiter.await_ready() returns 
//     false: // (A) -> suspend coroutine & copy the coroutine (execution) state 
//                      into the coroutine frame (on the heap). It's an 
//                      optimization parameter. 
//     awaiter.await_suspend(coroutineHandle) returns:
//         void: // (a)
//             awaiter.await_suspend(coroutineHandle); 
//             // -> coroutine keeps suspended
//             return to caller
//         bool: // (b)
//             bool result = awaiter.await_suspend(coroutineHandle); 
//             if result:
//                 // -> coroutine keep suspended
//                 return to caller 
//             else:
//                 // -> go to resumptionPoint (but the suspension has occured. -> wasting resources.)
//         another coroutine handle: // (c)
//             auto anotherCoroutineHandle = awaiter.await_suspend(coroutineHandle); 
//             anotherCoroutineHandle.resume();
//             return to caller
//     true: // (B) -> go to the resumptionPoint, no suspension occur (the cost of copying the local
//                     variables to the coroutine frame can be avoided and the coroutine state will
//                     not be saved -> optimize the process).
// // resumptionPoint:
// return awaiter.await_resume();
// 

int main() {

    // gen is the coroutine (generator) object (the customed class wrapping the coroutine_handle).
    auto gen = infinite_seq(-10, 2);
    for(std::size_t i = 0; i < 20; i++){
        gen.resume();
        std::cout << gen.get_value() << " ";
    }

    return 0;
}