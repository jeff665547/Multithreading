// 1. GNU compiler needs intel TBB library support.
// 2. Compiler should support C++17 (unseq for C++20). (編譯參數：-std=c++20 or -std=c++17)
// 3. 平行(parallelism)強調不同任務可以在同一瞬間同時執行，較不會有等待或是發呆的狀態，且不同任務間的執行順序不相依(不能有先後順序)，
//    有時需要硬體支援(如：SIMD)。
//    並行(concurrency)不強迫不同任務要在同一瞬間執行，會有等待或是發呆的狀態，不同任務間容許有先後順序以及相依性。
// 4. Execution Policy:
//    a. std::execution::seq        -> 就是一般的for-loop，主要用來debug時使用。
//    b. std::execution::par        -> 使用多執行緒的方式來達到平行(同一瞬間，同時執行)的結果。
//                                     在單一執行緒中每道指令(機器碼(instruction))對容器內的一個元素進行操作，
//                                     而且在單一執行緒中的指令順序會依序執行且不會交錯(不保證不同執行緒間的先後順序，
//                                     不同執行緒間可能會交錯)，
//                                     但會使用多個執行緒對同一個容器做操作運算，
//                                     因此需要避免data race。
//    c. std::execution::par_unseq  -> 即為par以及unseq綜合起來的結果。
//                                     在單一執行緒中每道指令(機器碼(instruction))同時對容器內的多個元素進行操作，
//                                     會使用多個執行緒對同一個容器做操作運算，因此需要避免data race。
//                                     此外，在單一執行緒中的指令順序會交錯執行(interleaved)。
//                                     因此需要避免使用lock以及memory allocation (e.g. 在單一執行緒中，因為交錯執行的關係，
//                                     而使得前一次的lock在還沒有被unlock以前，就又再被用來執行下一個元素的相同操作再lock一次，
//                                     造成重複lock(deadlock)，lock要使用atomic來取代)。
//    d. std::execution::unseq      -> 即SIMD,在單一執行緒的環境中來達到平行(同一瞬間，同時執行)的結果，又被稱作向量化(vectorization)。
//                                     在單一執行緒中每道指令(機器碼(instruction))同時對容器內的多個元素進行操作，
//                                     而且在單一執行緒中的指令順序會交錯執行(interleaved)，
//                                     因此需要避免使用lock以及memory allocation (e.g. 在單一執行緒中，因為交錯執行的關係，
//                                     而使得前一次的lock在還沒有被unlock以前，就又再被用來執行下一個元素的相同操作再lock一次，
//                                     造成重複lock(deadlock)，lock要使用atomic來取代)。
// 4. Example -- reduce with add operator (須滿足交換律(x+y=y+x)與結合律((x+y)+z=x+(y+z)))
//    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
//    auto sum = reduce(seq, v.begin(), v.end(), 0);       // -> ((((((((0+1)+2)+3)+4)+5)+6)+7)+8), one thread, one instruction deal with one element.
//    auto sum = reduce(par, v.begin(), v.end(), 0);       // -> (0+((1+2)+(3+4))+((5+6)+(7+8))), thread #1, #2, #3 and #4 add at the same time.
//                                                         // thread  #1    #2    #3    #4
//    auto sum = reduce(par_unseq, v.begin(), v.end(), 0); // -> (0+(1+2+3+4)+(5+6+7+8)), thread #1 and #2 add at the same time.
//                                                         // thread    #1        #2
//    auto sum = reduce(unseq, v.begin(), v.end(), 0);     // -> 0+(1+2+3+4)+(5+6+7+8), one thread, one instruction deal with multiple elements.
//                                                         // group     A         B     在一個執行緒中先在同一瞬間同時處理A組，下一瞬間再同時處理B組，
//                                                         //                           A、B兩組為先後處理，非同時處理。
#include <algorithm>
#include <execution>
#include <cmath>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

constexpr long long size = 500'000'000;

const double pi = std::acos(-1);

template <typename Func>
void getExecutionTime(const std::string& title, Func func){

    const auto start = std::chrono::steady_clock::now();
    func();
    const std::chrono::duration<double> dur = std::chrono::steady_clock::now() - start;
    std::cout << title << ": " << dur.count() << " sec. " << '\n';

}

int main(){

    std::cout << std::endl;
    std::cout << "start!" << std::endl;

    std::vector<double> randValues;
    randValues.reserve(size);

    std::mt19937 engine;
    std::uniform_real_distribution<> uniformDist(0, pi / 2);
    for (long long i = 0 ; i < size ; ++i) randValues.push_back(uniformDist(engine));

    std::vector<double> workVec(randValues);

    getExecutionTime("std::execution::seq", [workVec]() mutable {
        std::transform(
            std::execution::seq, 
            workVec.begin(), workVec.end(), workVec.begin(),
            [](double arg){ return std::tan(arg); }
        );
    });

    getExecutionTime("std::execution::par", [workVec]() mutable {
        std::transform(
            std::execution::par, 
            workVec.begin(), workVec.end(), workVec.begin(),
            [](double arg){ return std::tan(arg); }
        );
    });

    getExecutionTime("std::execution::par_unseq", [workVec]() mutable {
        std::transform(
            std::execution::par_unseq, 
            workVec.begin(), workVec.end(), workVec.begin(),
            [](double arg){ return std::tan(arg); }
        );
    });

    std::cout << std::endl;

}