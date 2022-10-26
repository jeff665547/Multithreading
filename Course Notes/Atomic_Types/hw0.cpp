#include <random>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>
#include <cmath>

using namespace std::literals;

unsigned int Max(const std::vector<unsigned int>& values) {
    int threads_num = 2*std::thread::hardware_concurrency();
    std::vector<unsigned int> threads_max(threads_num);
    std::vector<std::thread> threads;
    int vec_size = values.size();
    int range = std::ceil(vec_size/threads_num);
    
    for(int i = 0; i < threads_num; i++){
        int begin = i*range;
        int end = std::min((i+1)*range, vec_size);
        unsigned int& tmp_max = threads_max[i];
        threads.push_back(std::thread{
            [&, i, begin, end] {
                for(int j = begin; j < end; j++){
                    if (values[j] > tmp_max) {
                        tmp_max = values[j];
                    }
                }
            }
        }); 
    }
    for(auto& thread: threads){
        thread.join();
    }

    unsigned int result = 0;
    for (auto& v : threads_max) {
        if (v > result) {
            result = v;
        }
    }
    
    return result;
}

int main() {
    std::mt19937 mt{0};
    std::vector<unsigned int> values;
    for (size_t i = 0; i < 100000000; ++i) {
        values.push_back(mt());
    }
    auto start = std::chrono::steady_clock::now();
    std::cout << Max(values) << std::endl;
    auto end = std::chrono::steady_clock::now();
    std::cout << (end - start) / 1ns << "ns" << std::endl;
}