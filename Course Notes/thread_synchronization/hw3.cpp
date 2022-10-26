#include <iostream>
#include <mutex>
#include <thread>
#include <string>

using namespace std::literals;

bool modified {false};
std::string sdata {"Empty"};
std::mutex mut;

void reader(){
    while(!modified){
        std::this_thread::sleep_for(50ms);
    }
    std::unique_lock<std::mutex> lk(mut, std::defer_lock);
    while(!lk.try_lock()){
        std::this_thread::sleep_for(50ms);
    }
    std::cout << "The data is " << sdata << std::endl;
}
void writer(){
    std::cout << "Modifying the data" << std::endl;
    {
        std::lock_guard<std::mutex> lk(mut);
        sdata = "Populated";
        modified = true;
    }
}

int main(){

    std::cout << "The data is " << sdata << std::endl;
    std::thread read{writer};
    std::thread write{reader};
    read.join(); write.join();

    return 0;
}