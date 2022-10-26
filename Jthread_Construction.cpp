#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <future>
using namespace std::literals;

thread_local std::atomic<bool> interrupt_bool;

class Jthread{
    std::thread _thread;
    std::atomic<bool>* interrupt_indicator{nullptr};
public:
    template<typename T>
    Jthread(T f){
        std::promise<std::atomic<bool>*> p;
        _thread = std::thread{[f, &p]{
            p.set_value(&interrupt_bool);
            f();
        }};
        interrupt_indicator = p.get_future().get();
    }
    ~Jthread(){
        if(_thread.joinable()){
            _thread.join();
        }
    }
    void interrupt(){
        interrupt_indicator->store(true);
    }
};

void demo(){
    int ct = 0;
    while(ct < 10){
        if(interrupt_bool.load()) return;
        std::this_thread::sleep_for(0.2s);
        std::cerr << "Non-interrupt " << ct << std::endl;
        ct++;
    }
}

int main(){

    Jthread t1{demo};
    Jthread t2{demo};

    std::this_thread::sleep_for(1s);

    t1.interrupt();

    return 0;
}