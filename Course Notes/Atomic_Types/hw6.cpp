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
        std::packaged_task<T(T*,T*,T)> task{accum<T>};
        Results.push_back(std::move(task.get_future()));
        std::thread{
            std::move(task), 
            &data[i*range_], 
            &data[std::min((i+1)*range_, vec_size)], 
            0
        }.detach();
    }
    T sum = 0.0;
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