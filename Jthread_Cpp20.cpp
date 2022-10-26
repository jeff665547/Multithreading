// 編譯參數： -std=c++20
// 1. RAII design principle -> 可使用大括弧{}特性，在括弧結束時啟動destructor。
// 2. Stop token handle check -> 可用做thread間的一種溝通協作方式，如同simple thread pool中的closed機制。
#include <iostream>
#include <thread>
using namespace std::literals;

void do_something(std::stop_token token){
    int counter{0};
    while(counter < 10){
        if(token.stop_requested()){
            return;
        }
        std::this_thread::sleep_for(0.2s);
        std::cout << "This is interrruptible thread : " << counter << std::endl;
        ++counter;
    }
}

int main(){

    std::cout << std::endl;
    std::jthread interruptible{do_something};

    std::this_thread::sleep_for(1.0s);
    interruptible.request_stop();      // 在這邊對jthread請求停止，stop_token(do_something的argument)
                                       // 的值會改變，要注意thread function必須要加上std::stop_token
                                       // 的參數，然後在jthread初始化使用thread function時不用特別傳入
                                       // stop_stoken。
    std::cout << std::endl;

    return 0;
}