// 效能優化
// 1. 先確認在單一執行緒環境運行下，其結果為正確的，可執行的。
// 2. 產生足夠多的執行緒，執行緒個數通常與硬體可同時支援個數有關。
// 3. 將工作切割成多個"足夠大"的任務，太小會造成資源浪費。
// 4. 每個執行緒要盡量執行到沒有事情做為止，避免一直切換造成額外的負擔(overhead)。
//    每次要開啟一個新的執行緒成本是很高的(需要1000多行指令)，這也是使用thread pool
//    的一個最重要原因。
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
        cv.notify_one();
    }
    bool WaitandDequeue(T& value){
        std::unique_lock<std::mutex> uk(m);
        cv.wait(uk, [this]{return (!dq_.empty()||closed.load());});
        if(dq_.empty()) return false;
        value = dq_.front();
        dq_.pop_front();
        return true;
    }
    void Close(){
        closed.store(true);
        cv.notify_all();
    }
};

struct Event{
    int from;
    int to;
};

template<typename T>
void quick_sort(std::vector<T>& arr, int start, int end, Queue<Event>& Jobs, std::atomic<int>& ct){
    while(true){
        if(start == end) return;

        // Classification (quick_sort 以第一個元素當成是參考值(pivot)下去進行分類)
        int i = start+1;
        for(int j = i; j < end; j++){
            if(arr[j] < arr[start]){
                std::swap(arr[i], arr[j]);
                i++;
            }
        }
        // Substitution (將參考值置換擺到正確的位置(擁有正確的百分點位置(percentile)))
        int mid = i-1;
        std::swap(arr[mid], arr[start]);

        if((mid - start) > 10){             // 優化技巧：當長度大於特定的長度時再切分給別的thread去做，因為如果長度太小將它切分出去的成本將大於自己把它做完的成本。
            Jobs.Enqueue({start, mid});
            ct++;
        }else{
            quick_sort(arr, start, mid, Jobs, ct);
        }
        start = mid+1;                      // 優化技巧：當自己目前這輪運算已經做完的時候，緊接著再做下一輪的運算(while)，
                                            // 而不是丟給其他thread做，然後自己閒置。充分利用資源，自產幫忙自銷。
    }
}

int main(){
    int thread_num = std::thread::hardware_concurrency();
    std::cout << "Avaliable Threads: " << thread_num << std::endl;
    std::cout << "[" << std::this_thread::get_id() << "]";
    int num = 1000;
    std::vector<int> vec(num);

    for(int i = 0; i < num; i++){
        vec[i] = num - i;
    }

    for(auto& e: vec){
        std::cout << e << " ";
    }
    std::cout << std::endl;

    std::atomic<int> ct{0};
    Queue<Event> Jobs;
    std::vector<std::thread> workers;
    Jobs.Enqueue({0, (int)vec.size()});
    ct++;
    for(int i = 0; i < thread_num; i++){
        workers.push_back(std::thread{[&Jobs, &vec, &ct]{
            Event event;
            while(Jobs.WaitandDequeue(event)){
                quick_sort(vec, event.from, event.to, Jobs, ct);
                ct--;
                std::this_thread::sleep_for(0.001s);   // Force to switch threads
                std::cout << "[" << std::this_thread::get_id() << "]";
            }
        }});
    } 

    while(ct!=0){                     // <- This pattern is busy waiting. (ct 是用來確認工作是否已經全部做完了，
                                      //    這邊要注意在不能只看Jobs是否已經是空的來判斷工作是否已經全部做完，
                                      //    因為有可能是雖然Jobs為空，但有thread正在處理最後一份工作，
                                      //    它剛拿走去做，但是還沒做完，做到一半，可能在後續做的過程中又會再塞新的event進到Jobs裡。)
        std::this_thread::yield();    // <- This pattern will force to release the resource temporarily during busy waiting.
    }
    Jobs.Close();

    for(auto& t: workers){
        t.join();
    }

    std::cout << std::endl;

    for(auto& e: vec){
        std::cout << e << " ";
    }

    return 0;
}