#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>

using namespace std::literals;
std::string sdata {"Empty"};
std::mutex mut;

bool ready1 {false};
bool ready2 {false};
bool start_process {false};
bool show50 {false};
std::condition_variable fetch_cv50, fetch_cv50_ct, fetch_cv100, progress_cv100;

// Modifying thread
void fetch(){
    {
        std::lock_guard<std::mutex> lk(mut);
        std::cout << "Fetching data ... " << std::endl;
        std::this_thread::sleep_for(1s);
        sdata = "data 1\n";
        ready1 = true;
    }
    fetch_cv50.notify_one();
    {
        std::unique_lock<std::mutex> lk(mut);
        fetch_cv50_ct.wait(lk, []{return show50; });
        std::cout << "Keep fetching ... " << std::endl;
        sdata += "data 2\n";
        std::this_thread::sleep_for(1s);
        ready2 = true;
    }
    fetch_cv100.notify_all();
}

// Waiting thread - progress thread ...
void progress(){
    std::unique_lock<std::mutex> lk(mut);
    fetch_cv50.wait(lk, []{ return ready1; } );
    std::cout << "50% ..." << std::endl;
    show50 = true;
    lk.unlock();
    fetch_cv50_ct.notify_one();
    lk.lock();
    fetch_cv100.wait(lk, []{ return ready2; } );
    std::cout << "100% accomplished" << std::endl;
    start_process = true;
    progress_cv100.notify_all();
}

// Waiting thread - process thread ...
void process(){
    std::unique_lock<std::mutex> lk(mut);
    fetch_cv100.wait(lk, []{ return ready2; } );
    progress_cv100.wait(lk, []{ return start_process; } );
    std::cout << "Processing the data." << std::endl;
    std::cout << sdata;
    std::this_thread::sleep_for(1.5s);
    std::cout << "Process done." << std::endl;
}

int main(){

    std::thread progress_thread {progress};
    std::thread fetching_thread {fetch};
    std::thread process_thread {process};
    fetching_thread.join(); progress_thread.join(); process_thread.join();

    return 0;
}