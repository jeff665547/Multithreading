/* Corountines */
// * Sudo-asynchronous programming
// * NOTICE: Only a few compiler support the implementation of the corountines.
// * Compile with -std=c++20 -fcoroutines -pthread
// * Subroutines -- Traditional functions
//   - When caller invokes function, it has to wait (i.e. be blocked) until the 
//     call function finishes its execution.
//   - Two stages (operations) in subroutines.
//     a. Invoke -- Initialize the function call
//     b. Finalize -- Resource deallocation
// * Coroutines -- Functions that contain co_return, co_yield, co_await.
//   - Functions that allow the function itself to be suspended and then later
//     resumed.
//   - Four stages (operations) in coroutines.
//     a. Invoke
//     b. Suspends -- Coroutine suspends its execution.
//     c. Resume -- Caller (who has the handle of coroutine) has the ability to 
//                  resume the coroutine execution.
//     d. Finalize
//   - Construct the coroutine object (coro state object) in the heap memory to 
//     store the information that is used to resume the coroutine.
//   - When the coroutine is suspended, the information in the coroutine object 
//     (coro state object) will be updated, and that can be used for the next 
//     restoration of the coroutine.
//   - Coroutines are stackless -- It does not initiate any stack for itself.
//   - Coroutines do not require any context switching. -> More efficiently.
// 
// * Detail internal structure of the coroutine:
//   - 在C++裡，coroutine 需要搭配額外的實作才能使用。
//   - Coroutine 包含以下物件:
//     a. A promise object (這邊的promise是指廣義的promise)
//        # 此物件是從coroutine內部去操作它
//        # 此物件為coroutine向外傳遞結果的橋樑
//        # 此物件的定義架構在promise_type物件上，其中promise_type為一個使用者自定義的類別。
//     b. A Coroutine handle
//        # 此物件為一個沒有所有權的物件(意即可以被當成參數一樣傳來傳去)。
//        # 此物件被用來從coroutine外部恢復coroutine的執行或是去摧毀coroutine。
//     c. A Coroutine state
//        # 此物件被部署在Heap記憶體上。
//        # 此物件主要紀錄coroutine暫停時的內部情況，包含promise object, 
//          arguments to coroutine, 區域變數(包含已經結束生命週期與還沒結束生命週期的區域變數)
//   - 在coroutine中，雖然沒有明確的 return 語句，但編譯器將會替coroutine隱性地創造回傳物件，
//     而此回傳物件也就是Coroutine handle物件。
//   - std::suspend_always(): 在coroutine中用來表示一個暫停點(等待caller來恢復執行)。
//   - Caller 如果要恢復執行一個coroutine必須要擁有該coroutine的coroutine handle，
//     才能夠透過該coroutine handle來對coroutine恢復執行。實務上，此coroutine handle會另外
//     被包裝進一個使用者自定義類別(如下面的resumable class)中。
//   - 一般來說，使用者自定義類別(resumable class, promise_type)裡面的內容
//     幾乎在所有情況下都是大同小異的。
//   - promise_type裡的成員函式大部分是被規範強制要求的，如下面範例中的各個成員函式。在這之中，
//     使用return_void是因為此coroutine沒有回傳任何東西，如果有回傳東西則必需要使用其他函數。
//     例如：若在coroutine中使用co_yield來回傳東西，則是需要使用yield_value成員函式，而不是
//     使用return_void成員函式。
#include <cassert>
#include <iostream>
#include <coroutine>

class resumable{
public:
    struct promise_type;
    using coroutine_handle = std::coroutine_handle<promise_type>;
    resumable(coroutine_handle handle): handle_(handle) { assert(handle); };  
    // The above constructor will be called implicitly when the coroutine object 
    // get initialized.
    resumable(resumable&) = delete;
    resumable(resumable&&) = delete;
    bool resume() {  // -> caller use, its name is user-defined API.
        if(!handle_.done()){  // handle_.done() checks if the coroutine is 
                              // finished or it is in the suspend state.
             handle_.resume();
        }
        return !handle_.done();  
        // -> If it finally return true -> the coroutine is in the suspend state.
    }
    ~resumable(){ handle_.destroy(); }
private:
    coroutine_handle handle_;
};

struct resumable::promise_type{
    using coroutine_handle = std::coroutine_handle<promise_type>;
    auto get_return_object() {
        // return the coroutine handle object.
        return coroutine_handle::from_promise(*this);
    }
    auto initial_suspend() {
        return std::suspend_always();
    }
    auto final_suspend() noexcept {
        return std::suspend_always();
    }
    void return_void() {}
    void unhandled_exception() {
        std::terminate();
    }
};

resumable foo(){
    std::cout << "a" << std::endl;
    co_await std::suspend_always(); // -> This expression makes the coroutine 
                                    // to pause its execution.
    std::cout << "b" << std::endl;
    co_await std::suspend_always();
    std::cout << "c" << std::endl;
}

int main(){
    resumable res1 = foo();  // -> Construct a coroutine handle object. 
                             // When a coroutine (foo) is constructed, 
                             // it will be an initial suspended state.

    res1.resume();           // -> Users has to explicitly resume execution 
                             // (by calling resume on the coroutine handle object) to 
                             // initiate the coroutine execution in the first place. 
                             // The resume function is an user-defined function.
                             // print "a" (from the initial suspended state to the
                             // first suspend point.)

    res1.resume();           // print "b"
    res1.resume();           // print "c"
    return 0;
}