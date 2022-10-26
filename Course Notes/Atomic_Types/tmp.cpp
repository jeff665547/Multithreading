#include <iostream>
#include <deque>
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
using namespace std::literals;

template<typename T>
class Queue{
    std::atomic<bool> is_close{false};
    std::deque<T> dq_;
    std::mutex mdq;
    std::condition_variable cv;
public:
    void Enque(T val) {
      {
        std::lock_guard<std::mutex> lk(mdq);
        dq_.push_back(val);
      }
        cv.notify_one();
    }
    bool WaitAndDeque(T& val) {
        std::unique_lock<std::mutex> uk(mdq);
        cv.wait(uk, [&](){return !dq_.empty() || is_close;});
        if(dq_.empty()) return false;
        val = dq_.front();
        dq_.pop_front();
        return true;
    }
    void Close() {
        is_close = true;
        cv.notify_all();
    }
};

struct Job{
    int begin;
    int end;
};

int main(){
    
    Queue<Job> Jobs{};
    
    // Run job.
    std::mutex moutput;
    std::vector<std::thread> Workers(3);
    for(std::size_t t = 0; t < 3; t++){
        Workers[t] = std::thread{[&, t](){
                Job job;
                while(Jobs.WaitAndDeque(job)){
                    std::lock_guard<std::mutex> lk(moutput);
                    std::cout << "t: " << t << ": " << job.begin << " -> " << job.end << std::endl;
                    std::this_thread::sleep_for(500ms);
                }
            }
        };
    }

    // Set job. Sum up from 0 to 100.
    Jobs.Enque({1,  10});
    Jobs.Enque({11, 20});
    Jobs.Enque({21, 30});
    Jobs.Enque({31, 40});

    Jobs.Close();    

    for(auto& t: Workers){
        t.join();
    }
    std::cout << "end" << std::endl;

    std::promise<int> pro;
    std::future<int> f = pro.get_future();
    std::thread([](std::promise<int>&& pro ){pro.set_value(2);}, std::move(pro)).detach();

    std::cout << std::boolalpha;
    std::cout << f.valid() << std::endl;
    std::cout << f.get() << std::endl;
    // std::cout << f.get() << std::endl;
    std::thread([](std::promise<int>&& pro ){pro.set_value(2);}, std::move(pro)).detach();
    std::cout << f.valid() << std::endl;

    
    return 0;
}