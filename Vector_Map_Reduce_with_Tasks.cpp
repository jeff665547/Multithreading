// 1. 因為thread無法得到回傳物件，雖然可以傳reference，但若要使用 A.getval().B() 
//    之類的functional programming串接特徵，會變得很不方便，因此使用async function
//    可以解決此問題。此外std::thread是一個類別，但std::async本身是一個function，
//    呼叫該function以後會得到一個叫做future的物件(其模板參數為std::async內function
//    的回傳值類別)。
// 2. async可以決定是否要啟動一個新的thread來完成交辦的任務(function)，
//    std::launch::async為執行async當下強制開新的thread去執行，
//    std::launch::deffered為先把function給儲存起來，到最後利用future物件去get的時候
//    才會去執行(同一個thread)，如果都沒有明確寫，代表交由編譯器自行決定。
//    另外，std::future<T>::get 會等到結果算出來為止(類似std::thread的join的概念)。
// 3. 如果沒有使用變數去將async的future給bound起來，則程式雖然會啟動另一個執行緒去執行
//    交辦的任務，但卻會原地等待那個async要執行的任務(i.e. 原地join的概念)，執行完才會
//    接著執行後面的程式，如此一來就會失去並行的意義。
// 4. std::packaged_task是一個用來實做std::async的一個更底層實作，可以用來包裝一個
//    callable物件。
//    其使用方式為 a. Wrap the task (callable) b. Create the future 
//    c. Perform the calculation d. Pickup the result.
// 5. 使用std::async的時機為不在意被包起來的任務什麼時候完成，單純只是要把事情給完成。
//    使用std::packaged_task的時機為把任務給包起來並且明確的指派任務給其他的執行緒
//    std::thread{std::move(task)}.detach()做或是在接下來的特定時間點自行處理
//    (單純call task())。

#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include <thread>
#include <future>

template<typename T>
T accum(T *beg, T *end, T init) {
    std::cout << "Thread ID " << std::this_thread::get_id() << std::endl;
    return std::accumulate(beg, end, init);
}

template<typename T>
T async_res(std::vector<T>& data, const int& thread_num) {
    int vec_size = data.size();
    int range_ = std::ceil((double) data.size() / (double) thread_num);
    std::vector<std::future<T>> Results;
    for(int i = 0; i < thread_num; i++){
        // a. Wrap the task (callable) + b. Create the future + c. Perform the calculation.
        std::future<T> f = std::async(
            std::launch::async, 
            accum<T>, 
            &data[i*range_], 
            &data[std::min((i+1)*range_, vec_size)], 
            0.0
        );
        Results.push_back(std::move(f));        
    }
    T sum = 0.0;
    // (c. Perform the calculation (with std::launch::deffered)) + d. Pick up the result.
    for(auto& res: Results){
        sum += res.get();
    }
    return sum;
}

template<typename T>
T packaged_task_res(std::vector<T>& data, const int& thread_num) {
    int vec_size = data.size();
    int range_ = std::ceil((double) data.size() / (double) thread_num);
    std::vector<std::future<T>> Results;
    for(int i = 0; i < 4; i++){
        // a. Wrap the task (callable).
        std::packaged_task<T(T*,T*,T)> task{accum<T>};
        // b. Create the future.
        Results.push_back(std::move(task.get_future()));
        // c. Perform the calculation.
        std::thread{
            std::move(task), 
            &data[i*range_], 
            &data[std::min((i+1)*range_, vec_size)], 
            0
        }.detach(); // 交給背景做，不在這邊等它
    }
    T sum = 0.0;
    // d. Pick up the result.
    for(auto& res: Results){
        sum += res.get();
    }
    return sum;
}

int main(){
    // Vector Data Initialization
    int vec_sz{10000};
    std::vector<double> vec(10000);
    std::mt19937 mt{};
    for(int i = 0; i < vec_sz; i++){
        vec[i] = mt();
    }

    // Test Data Initialization
    std::vector<int> test_vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    // Test Cases
    std::cout << async_res(test_vec, 4) << std::endl;

    std::cout << "====================================" << std::endl;

    std::cout << packaged_task_res(test_vec, 4) << std::endl;
    
    std::cout << "====================================" << std::endl;

    // Application
    std::cout << async_res(vec, 4) << std::endl;

    std::cout << "====================================" << std::endl;

    std::cout << packaged_task_res(vec, 4) << std::endl;

    return 0;
}