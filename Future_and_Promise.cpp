// * future and promise 被用來在執行緒之間建立通道傳遞資料(它不在意資料是怎麼來的)
//   而不像傳統mutex以及lock去共用資料。
// * promise 負責寫資料，future 負責讀資料，一個 promise 保證只會存一筆資料(以及其相關的例外)，
//   一次只能傳遞一個東西，傳遞過了就不能再使用(也就是p.set_value() & f.get()不能重複使用)。
// * promise 與 future 為一對一的關係。若要一(寫)對多(讀)的關係則要使用shared_future。
// * future and promise若在同一個thread內必須要先寫入在讀取，若在不同的thread則順序都可以
// * future and promise 把lock封裝在它們成員變數裡。
// * shared_future只能被copy，不像future以及promise只能被move。
// * future and promise 也可以用來傳遞通知std::promise<void>, std::future<void> & 
//   f.wait() (在promise還沒執行完set_value()以前先block)，
//   某種程度上取代condition_variable的功能(當通知功能只會使用一次時)。
#include <future>
#include <vector>
#include <thread>
#include <iostream>

void Run(std::shared_future<int> f, int i){
    int target = f.get();  // 每個thread都會卡在這邊等set_value的結果
    if(i == target){
        std::cout << "Run: " << i << std::endl;
    }
}

void GetValue(std::promise<int> p){
    p.set_value(5);
}

int main(){

    std::promise<int> p1;
    std::future<int> f1 = p1.get_future(); 
    std::thread{GetValue, std::move(p1)}.detach();

    std::vector<std::thread> threads;
    std::promise<int> p2;
    std::shared_future<int> f2 = p2.get_future();
    for(int i = 0; i < 10; i++){
        threads.push_back(std::thread{Run, f2, i});
    }
    std::cout << "Prepare" << std::endl;
    p2.set_value(f1.get());
    for(auto& t: threads) t.join();

    return 0;
}