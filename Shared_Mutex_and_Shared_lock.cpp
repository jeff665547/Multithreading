#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <sstream>
#include <shared_mutex>

using namespace std::literals;

std::shared_mutex the_mutex;

void Write(int i){
    // lock_guard, unique_lock, scoped_lock 屬於 exclusive lock 
    //  -> 一次只允許一個thread執行critical section。 -> threads 一個接一個執行
    std::lock_guard<std::shared_mutex> lk(the_mutex);
    std::stringstream ss;
    ss << "Write thread " << i << " with exclusive lock";
    std::cout << ss.str() << std::endl;
    std::this_thread::sleep_for(5s);
}

void Read(int i){
    // shared_lock 屬於 shared lock -> 只要擁有shared lock的thread
    // 都可以執行critical section，因此不同於exclusive lock，一次只能有一個
    // thread執行critical section，shared lock 可以增進效率
    // (特別是在很多thread讀，只有一兩個thread寫的時候特別有效)。
    std::shared_lock<std::shared_mutex> sl(the_mutex);
    std::stringstream ss;
    ss << "Read thread " << i << " with shared lock";
    std::cout << ss.str() << std::endl;
}

int main(){
    std::vector<std::thread> threads;

    // read threads (#0 ~ #4) 先執行，採用 shared lock，
    // 大家一起access，沒有先後順序。
    for(int i = 0; i < 5; i++){
        threads.push_back(std::thread{Read, i});
    }
    // 由於 write thread (#5) 採用 exclusive lock，
    // write thread (#5) 會等到 read threads (#0 ~ #4)執行完
    // (release shared lock) 之後才會開始執行。
    threads.push_back(std::thread{Write, 5});
    // 由於 read thread (#6 ~ #10) 採用 shared lock，
    // read thread (#6 ~ #10) 會等到 write thread (#5) 執行完
    // (release exclusive lock) 之後才會開始執行。
    for(int i = 6; i < 11; i++){
        threads.push_back(std::thread{Read, i});
    }
    for(auto& t: threads){
        t.join();
    }

    return 0;
}