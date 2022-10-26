#include <iostream>
#include <deque>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <cmath>
using namespace std::literals;
template<typename T>
class Queue {
    std::deque<T> dq_;
    std::atomic<bool> is_close{false};
    std::mutex mq;
    std::condition_variable cv;
public:
    void Enque(T value) {
        std::lock_guard lk(mq);
        dq_.push_back(value);
        cv.notify_one();
    }
    void Close() {
        is_close.store(true);
        cv.notify_all();
    }
    bool WaitAndDeque(T& value) {
        std::unique_lock<std::mutex> uk(mq);
        cv.wait(uk, [&](){return !dq_.empty() || is_close;});
        if(dq_.empty()) return false;
        value = dq_.front();
        dq_.pop_front();
        return true;
    }
};

struct Job{
    int from;
    int to;
};

int main() {

    std::this_thread::sleep_for(500ms);
    int thread_no = std::thread::hardware_concurrency();
    int n = 100;

    Queue<Job> Jobs;
    Queue<int> Results;

    std::mutex moutput;
    std::vector<std::thread> Workers;
    for(int t = 1; t <= thread_no; t++){
        Workers.push_back(std::thread{
            [&, t](){
                Job job;
                while(Jobs.WaitAndDeque(job)){
                    int res{0};
                    std::lock_guard<std::mutex> lk(moutput);
                    std::cout << "Thread " << t << ": " << job.from << " -> " << job.to << " = ";
                    for(int i = job.from; i <= job.to; i++){
                        res+=i;
                    }
                    std::cout << res << std::endl;
                    Results.Enque(res);                    
                }
            }
        });
    }

    std::this_thread::sleep_for(500ms);
    int range = std::ceil((double) n / (double) thread_no);
    for(int i = 1; i <= thread_no; i++){
        Jobs.Enque({1 + (i-1)*range, std::min((i)*range, n)});
    }
    Jobs.Close();

    for(auto& t: Workers) t.join();
    
    Results.Close();
    int res{0}, final_result{0};
    while(Results.WaitAndDeque(res)){
        final_result += res;
    }
    std::cout << "The sum is " << final_result << std::endl;

    return 0;
}