#include <iostream>
#include <deque>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>
#include <thread>

using namespace std::literals;

template<typename T>
class Queue{
    std::deque<T> dq_;
    std::mutex m;
    std::atomic<bool> closed{false};
    std::condition_variable cv;
public:
    void Enqueue(T val){
        {
            std::lock_guard<std::mutex> lk(m);
            dq_.push_back(val);
        }
        cv.notify_one();               // 叫醒其中一個thread起來進mutex準備拿任務。
    }
    bool WaitandDequeue(T& value){
        std::unique_lock<std::mutex> uk(m);
        cv.wait(uk, [this]{return (!dq_.empty()||closed.load());});
        // * 使用wait時，每個thread要進來搶m這個mutex，只有一個能夠搶到並且接續做後面的判斷條件確認，
        //   其他沒有搶到的thread會在wait這邊等，等有人使用notify_one或是notify_all來叫醒它，要注意
        //   這邊搶鎖的順序不是先搶先贏(FIFO)，而是他們背後自己有一套運行法則。
        // * wait加上判斷條件可以避免 lost wakeup 或是 spurious wakeup。
        // * wait後面的判斷條件當return ture時，程式碼會接續運行。
        if(dq_.empty()) return false;  // 當程試來到這邊時，代表事情都做完了，退出拿任務的迴圈。
        value = dq_.front();           // 分配任務給thread
        dq_.pop_front();               // 移除任務
        return true;                   // 叫拿到任務的thread再處理完後記得再回來排隊拿任務。
    }
    void Close(){
        closed.store(true);
        cv.notify_all();               // 叫醒所有的thread。
    }
};

struct Event{
    int from;
    int to;
};

int main(){

    int thread_num = std::thread::hardware_concurrency();
    std::cout << "Avaliable Threads: " << thread_num << std::endl;
 
    std::mutex mo;
    Queue<Event> Jobs;
    // 先把 workers (threads) 都叫過來。
    std::vector<std::thread> workers;
    for(int i = 0; i < thread_num; i++){
        workers.push_back(std::thread{[&Jobs, &mo]{
            Event event;   // launch一個空的event去Jobs.WaitandDequeue拿工作
            while(Jobs.WaitandDequeue(event)){
                std::this_thread::sleep_for(0.1s);   // Force to switch threads
                std::lock_guard<std::mutex> lk(mo);  // 此處的lock以及mutex是為了保護std::cout output stream.
                std::cout << "[" << std::this_thread::get_id() << "]";
                std::cout << " from: " << event.from << " -> to: " << event.to << std::endl;
            }            
        }});
    } 

    // 再分配工作。
    Jobs.Enqueue({10, 20});
    Jobs.Enqueue({20, 30});
    Jobs.Enqueue({30, 40});
    Jobs.Enqueue({40, 50});

    // 最後再通知大家可以休息了。
    Jobs.Close();

    for(auto& t: workers){
        t.join();
    }

    return 0;
}