// MapReduce: 從大向量中依照可使用的執行序數量分出小向量再從每個小向量中找出最大值。
// 1. 對於一般的容器去存取不同的元素是thread-safe的(不包含改變容器大小)。
//    注意如果最後存取的容器是std::vector<bool>，則不能夠使用下列方法，
//    即利用不同的執行緒去存取std::vector<bool>不同的部分，會有問題。
//    原因是在實作中，bool實際在儲存時，編譯器可能為了優化(節省儲存空間)
//    而去將std::vector<bool>壓縮，進而導致可能會出現兩個index共同儲存
//    在相同的byte(一般記憶體模型or物件的最小存取單位，不同的byte彼此間
//    可以並行)，進而導致Data race。如果真的要使用像是
//    std::vector<bool>的資料型態來存取資料，就要使用自己做的vector版本，
//    而不是std::vector或是使用lock。

#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <vector>
#include <random>

unsigned int Max(const std::vector<unsigned int>& values){
    std::size_t numberofworkers = std::thread::hardware_concurrency();  // std::thread::hardware_concurrency();
    std::size_t sizeofrange = (values.size() + (numberofworkers-1)) / numberofworkers;

    // std::vector<std::thread> workers(numberofworkers);
    std::vector<unsigned int> results(numberofworkers);  
    // 或是使用atomic<unsigned int> result{0}; 讓所有thread來共用。
    // 當thread執行完後即可得到結果。
    {  // 使用RAII特性
        std::vector<std::jthread> workers(numberofworkers);
        for(size_t i = 0; i < numberofworkers; i++){
            workers[i] = std::jthread{[i, sizeofrange, &values, &results](){   // 此種寫法和push_back的方式一樣，都是將thread的暫時物件移動到vector內部去。
                std::size_t start = i * sizeofrange;
                std::size_t end = std::min(values.size(), (i+1) * sizeofrange);
                results[i] = 0;
                for(size_t j = start; j < end; j++){
                    if(values[j] > results[i]){
                        results[i] = values[j];
                    }
                }
            }};
        }
    }
    
    // for(auto& t: workers){
    //     t.join();
    // }

    unsigned int result{0};
    for(auto& val: results){
        if(val > result){
            result = val;
        }
    }
    return result;
}

int main(){

    std::mt19937 mt{0};       // 0 是 seed.
    std::vector<unsigned int> values;
    for(size_t i = 0; i < 10000000; i++){ 
        values.push_back(mt());
    }
    auto start = std::chrono::steady_clock::now();
    std::cout << Max(values) << std::endl;
    auto end = std::chrono::steady_clock::now();
    std::cout << (end - start).count() << std::endl;

    return 0;
}