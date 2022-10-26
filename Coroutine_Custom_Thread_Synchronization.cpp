// 編譯參數：-std=c++20 -fcoroutine -pthread
#include <iostream>
#include <thread>
#include <coroutine>
#include <atomic>
using namespace std::literals;

class Event{
public:
    // The following is a nested class (巢狀類別)
    // 巢狀類別的設計模式具有以下幾個特色：
    // 1. 內外層類別在程式上彼此之間是獨立的(i.e. 建構外層實例時不會順便建構內層實例)，
    //    但是在設計理念上關係是通常是高度相關的，如人體(外層)和內臟(內層)，
    //    汽車(外層)和引擎(內層)。
    // 2. 一個外層類別可以有多個內層類別。
    // 3. 此設計模式可以用來更好的組織外層類別內部的成員設計，因內層類別算是外層類別獨有的，
    //    通常情況下，會使用外層類別的成員函式來建構(初始化)內層類別的實體。
    // 4. 內層類別存取外層類別的非靜態成員時，必須透過物件、指標或是參考，而不是直接呼叫，
    //    如同下方範例，Awaiter的constructer中的event parameter。
    // 5. 若內層類別為在private的範圍內，則內層類別可以用來隱藏(封裝)一些API的實作細節，
    //    當開發者想更新時可以不用知會使用者請他們更動程式碼，因為使用介面沒有改變。
    // 6. 若內層類別為在public的範圍內，則內層類別可以用來更好地表達且組織API介面，
    //    使其更易讀，為轉接層(adapter)的概念。
    // 7. 不同於繼承，此設計模式較無法重複利用inner class的程式碼。

    struct Awaiter{ // The class name is customed.
        Awaiter(Event& event): ev(event) {}
        // * The Awaiter concept implementation requires the following three functions.
        //   (await_ready, await_suspend, await_resume)
        // await_ready: Indicates if the result is ready. 
        //              When it returns false, await_suspend is called.
        bool await_ready(){
            return false;
        }
        // await_suspend: Schedule the coroutine for the resumption or destruction.
        // - std::coroutine_handle has two kinds of template parameters (void, 
        //   std::promise_type):
        //   When to use: If we need the promise_type to do something (e.g. use 
        //   from_promise to create a coroutine handle object), 
        //   then we need to use the std::promise_type version. 
        //   Ortherwise, we use the void version. The std::coroutine_handle 
        //   with the std::promise_type template version can be implicitly 
        //   converted to the void version.
        void await_suspend(std::coroutine_handle<> han){
            handle = han;  // 這邊將coroutine handle給存起來，以供後續resume時使用。
            if(ev.notified){
                han.resume();
            }else{
                ev.waiting_cor.store(this);
            }
        }
        // await_resume: Provides the result for the co_await exp expression.
        void await_resume(){}
        Event& ev;
        std::coroutine_handle<> handle;
    };

    // The following co_await operator is used to get the Awaiter object.
    // * In the most cases, the awaiter is awaitable directly. However, here, 
    //   we define the co_await operator to get the awaiter, and that is
    //   another common transformation method.
    Awaiter operator co_await() {
        return Awaiter{*this};
    }

    void notify(){
        auto ptr = waiting_cor.load();
        if(ptr!=nullptr){
            static_cast<Awaiter*>(ptr)->handle.resume(); // C++規定，當void*要轉型成其他類別的pointer時
                                                         // 要使用static_cast的轉型方式。
        }else{
            notified.store(true);
        }
    }

    std::atomic<bool> notified{false};
    std::atomic<void*> waiting_cor{nullptr};  // 在這邊使用 void* ，因為void*可以指向任何型態的記憶體位址。 
};


struct task{
    struct promise_type{
        task get_return_object() {return {};}
        std::suspend_never initial_suspend() {return {};}
        std::suspend_never final_suspend() noexcept {return {};}
        void return_void() {}
        // 因為在這邊的 coroutine 只有使用 co_await，compiler 會自動為它
        // 補上 co_return; 來代表執行完成，因此這邊要加上 return_void()。
        void unhandled_exception() {std::terminate();}
    };
};

task receiver(Event& event){
    std::cout << "[" << std::this_thread::get_id() << "] " << "Waiting for the notification!" << std::endl;
    co_await event;  // 因為沒有明確寫出 co_return，compiler 自動在此 function 加上 co_return;
    std::cout << "[" << std::this_thread::get_id() << "] " << "Got the notification!" << std::endl;
    std::cout << "[" << std::this_thread::get_id() << "] " << "Do other things!" << std::endl;    
}

int main(){

    Event ev1{};
    std::thread t1{[&ev1]{ev1.notify();}};  // 讓t2去等t1 -> 不用等，因為已經notified.
    std::thread t2{receiver, std::ref(ev1)};

    t1.join();
    t2.join();

    Event ev2{};
    std::thread t3{[&ev2]{
        receiver(ev2);
    }};
    std::this_thread::sleep_for(2s);        // 目的是為了讓t3去等t4。
    std::thread t4{[&ev2]{ev2.notify();}};

    t3.join();
    t4.join();

    return 0;
}